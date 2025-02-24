#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

std::mutex cout_mutex;

void handle_client(int client_socket, sockaddr_in client_address) {
    char buffer[1001];

    while (true) {
        int rs = recv(client_socket, buffer, 1000, 0);
        if (rs <= 0) {
            break; // клиент отключился
        }

        buffer[rs] = '\0';

        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "[" << inet_ntoa(client_address.sin_addr) << "] " << buffer << "\n";
    }

    close(client_socket);
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket creation error");
        exit(errno);
    }

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(8888);

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("bind failed");
        exit(errno);
    }

    if (listen(server_socket, 10) < 0) {
        perror("listen failed");
        exit(errno);
    }

    std::cout << "Server is listening on port 8888...\n";

    std::vector<std::thread> client_threads;

    while (true) {
        sockaddr_in client_address;
        socklen_t client_addr_len = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_addr_len);
        
        if (client_socket < 0) {
            perror("accept failed");
            continue;
        }

        std::cout << "New client connected: " << inet_ntoa(client_address.sin_addr) << "\n";
        client_threads.emplace_back(handle_client, client_socket, client_address);
    }

    for (auto& thread : client_threads) {
        thread.join();
    }

    close(server_socket);
    return 0;
}
