#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <algorithm> 

int main(int argc, char* argv[]) {
    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input-file>\n";
        return 1;
    }

    int srcFile = open(argv[1], O_RDONLY);
    if(srcFile == -1) {
        perror("Cannot open file");
        return 1;
    }

    char input[256];
    ssize_t bytesRead = read(srcFile, input, sizeof(input) - 1);
    if(bytesRead == -1) {
        perror("Error reading file");
        close(srcFile);
        return 1;
    }

    input[bytesRead] = '\0';
    std::string reversed(input, bytesRead);
    std::reverse(reversed.begin(), reversed.end());

    if(write(STDOUT_FILENO, reversed.c_str(), reversed.size()) != reversed.size()) {
        perror("Error writing");
        close(srcFile);
        return 1;
    }

    write(STDOUT_FILENO, "\n", 1);
    close(srcFile);
    return 0;
}

