#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>

int main(int argc, char* argv[]) {
  if(argc != 3) {
    std::cerr << "Usage:" << argv[0] << " <source file> <destination file>\n";
    return 1;
    }

    int srcFile = open(argv[1], O_RDONLY);
    if(srcFile == -1) {
      perror("cannot open source file");
      return 1;
    }

    int destFile = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(destFile == -1) {
      perror("cannot open destination file");
      close(srcFile);
      return 1;
    }

    char buffer[1024];
    ssize_t bytesRead;
    while((bytesRead = read(srcFile, buffer, 1024)) > 0) {
       if(write(destFile, buffer, bytesRead) != bytesRead) {
         perror("Error writing");
          close(srcFile);
          close(destFile);
          return 1;
      }
    }
    
    if(bytesRead == -1) {
      perror("Error reading");
    }

    close(srcFile);
    close(destFile);

    std::cout << "File copied from " << argv[1] << " to " << argv[2] << ".\n";
    return 0;
}
