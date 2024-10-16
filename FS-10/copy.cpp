#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <algorithm>

void print_usage() {
   std::cerr << "Usage: ./copy source-file destination-file" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        print_usage();
        return 1;
    }

    const char* source_path = argv[1];
    const char* dest_path = argv[2];

    int source_fd = open(source_path, O_RDONLY);
    if (source_fd == -1) {
        perror("Error");
        return 1;
    }

    int dest_fd = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1) {
        perror("Error opening file");
        close(source_fd);
        return 1;
    }

    char buffer[4096];
    ssize_t bytes_read;
    size_t total_copied = 0, total_holes = 0;

    while((bytes_read = read(source_fd, buffer, sizeof(buffer))) > 0) {
        if(bytes_read < 0) {
           perror("Error");
           close(source_fd);
           close(dest_fd);
           return 1;
        }

        ssize_t bytes_written = write(dest_fd, buffer, bytes_read);
        if (bytes_written < 0) {
            perror("Error");
            close(source_fd);
            close(dest_fd);
            return 1;
        }

        total_copied += bytes_written;
        total_holes += std::count(buffer, buffer + bytes_read, 0);
    }

    close(source_fd);
    close(dest_fd);

    std::cout << "copied " << total_copied << " bytes (data: " 
              << total_copied - total_holes << ", hole: " << total_holes << ")." << std::endl;

    return 0;
}
