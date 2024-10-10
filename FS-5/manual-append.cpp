#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

int main(int argc, char* argv[]) {
    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <output-file>\n";
        return 1;
    }

    int fileDescriptor = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(fileDescriptor == -1) {
        perror("Cannot open file");
        return 1;
    }

    int fd1 = dup(fileDescriptor);
    int fd2 = dup(fileDescriptor);
    
    if(fd1 == -1 || fd2 == -1) {
        perror("Failed to duplicate");
        close(fileDescriptor);
        return 1;
    }

    const char* line1 = "first line\n";
    const char* line2 = "second line\n";

     if(write(fd1, line1, strlen(line1)) == -1) {
        perror("Error writing");
        close(fd1);
        close(fd2);
        close(fileDescriptor);
        return 1;
    }

    if(write(fd2, line2, strlen(line2)) == -1) {
        perror("Error writing");
        close(fd1);
        close(fd2);
        close(fileDescriptor);
        return 1;
    }

    close(fd1);
    close(fd2);
    close(fileDescriptor);

    return 0;
}

