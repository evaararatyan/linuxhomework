#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "error\n";
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if(fd == -1) {
       std::cerr << "cannot open the file: " << strerror(errno) << "\n";
       return 1;
    }

    
    char buffer[1024];
    ssize_t bytesRead;
    while((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        if(write(STDOUT_FILENO, buffer, bytesRead) == -1) {
           std::cerr << "error\n";
           close(fd);
           return 1;
        }
    }


 if(bytesRead == -1) {
   std::cerr << "cannot read file: " << strerror(errno) << "\n";
   close(fd);
   return 1;
  }

close(fd);
return 0;
}
