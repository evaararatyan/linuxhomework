#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

void handle_request(int client_socket) {
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    int n = read(client_socket, buffer, sizeof(buffer) - 1);
    if (n < 0) {
        perror("ERROR reading from socket");
        return;
    }

    cout << "Received request: " << buffer << endl;

    char operation[10];
    int num1, num2;
    if(sscanf(buffer, "%s %d %d", operation, &num1, &num2) != 3) {
        const char *error_msg = "Invalid command format\n";
        write(client_socket, error_msg, strlen(error_msg));
        return;
    }

    int result = 0;
    bool valid = true;

    if(strcmp(operation, "ADD") == 0) {
        result = num1 + num2;
    }
   
    else if(strcmp(operation, "SUB") == 0) {
        result = num1 - num2;
    }
   
    else if(strcmp(operation, "MUL") == 0) {
        result = num1 * num2;
    }
   
    else if(strcmp(operation, "DIV") == 0) {
        if(num2 == 0) {
            const char *error_msg = "Division by zero error\n";
            write(client_socket, error_msg, strlen(error_msg));
            valid = false;
        }
       
	else {
            result = num1 / num2;
        }
    } 
    else {
       const char *error_msg = "Unknown operation\n";
        write(client_socket, error_msg, strlen(error_msg));
        valid = false;
    }

    if(valid) {
        //sending result to clienttt
        snprintf(buffer, sizeof(buffer), "Result: %d\n", result);
        write(client_socket, buffer, strlen(buffer));
    }
}

int main() {
    int server_socket, client_socket;
    sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    //create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    //initializing address socket
    memset((char *)&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(12345);

    // bind to socket address
    if(bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    //wait for connection
    listen(server_socket, 5);
    cout << "Server listening on port 12345..." << endl;

    while (true) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if(client_socket < 0) {
           perror("ERROR on accept");
            continue;
        }

        //hаndelling request
        handle_request(client_socket);

        //close client connection
        close(client_socket);
    }

    //close server socket
    close(server_socket);
    return 0;
}
