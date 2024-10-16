#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>

int main() {
    std::string source_file_name;
    std::cout << "Enter the name of the source file: ";
    std::cin >> source_file_name;

    std::string dest_file_name;
    std::cout << "Enter the name of the destination file: ";
    std::cin >> dest_file_name;

    int first_offset, second_offset, size;

    std::cout << "Enter the initial offset in the source file (in bytes): ";
    std::cin >> first_offset;

    std::cout << "Enter the initial offset in the destination file (in bytes): ";
    std::cin >> second_offset;

    std::cout << "Enter the number of bytes to copy: ";
    std::cin >> size;

    int fd_src = open(source_file_name.c_str(), O_RDONLY);
    if (fd_src < 0) {
        std::cerr << "Cannot open file " << source_file_name << std::endl;
        exit(1);
    }

    int fd_dest = open(dest_file_name.c_str(), O_WRONLY | O_CREAT, 0644);
    if (fd_dest < 0) {
        std::cerr << "Cannot open file " << dest_file_name << std::endl;
        close(fd_src);
        exit(1);
    }

    if (lseek(fd_src, first_offset, SEEK_SET) == -1) {
        std::cerr << "Error seeking in source file" << std::endl;
        close(fd_src);
        close(fd_dest);
        exit(1);
    }

    if (lseek(fd_dest, second_offset, SEEK_SET) == -1) {
        std::cerr << "Error seeking in destination file" << std::endl;
        close(fd_src);
        close(fd_dest);
        exit(1);
    }

    char* buffer = new char[size];
    ssize_t bytes_read = read(fd_src, buffer, size);
    if (bytes_read < 0) {
        std::cerr << "Error reading from source file" << std::endl;
        delete[] buffer;
        close(fd_src);
        close(fd_dest);
        exit(1);
    }

    ssize_t bytes_written = write(fd_dest, buffer, bytes_read);
    if (bytes_written < 0) {
        std::cerr << "Error writing to destination file" << std::endl;
        delete[] buffer;
        close(fd_src);
        close(fd_dest);
        exit(1);
    }

    std::cout << "Successfully copied " << bytes_written << " bytes from " << source_file_name << " to " << dest_file_name << "." << std::endl;

    delete[] buffer;
    close(fd_src);
    close(fd_dest);
    return 0;
}
