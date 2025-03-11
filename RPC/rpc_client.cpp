#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[256];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    memset((char *)&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(12345);

    if(connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("ERROR connecting");
        exit(1);
    }

    cout << "Enter operation (ADD, SUB, MUL, DIV): ";
    cin.getline(buffer, sizeof(buffer));

    write(sock, buffer, strlen(buffer));

    memset(buffer, 0, sizeof(buffer));
    read(sock, buffer, sizeof(buffer) - 1);
    cout << "Server response: " << buffer << endl;

    close(sock);
    return 0;
}
