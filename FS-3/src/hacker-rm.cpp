#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstring>

int main(int argc, char* argv[]) {
  if(argc != 2) {
     std::cerr << "Usage " << argv[0] << " <file-to-erase>\n";
     return 1;
    }

    const char* filePath = argv[1];

    struct stat fileStat;
    if(stat(filePath, &fileStat) != 0) {
      std::cerr << "File does not exist" << strerror(errno) << "\n";
      return 1;
    }

    int fd = open(filePath, O_WRONLY);
    if(fd < 0) {
      std::cerr << "Could not open file: " << strerror(errno) << "\n";
      return 1;
    }

    char nullByte = '\0';
    for(off_t i = 0; i < fileStat.st_size; ++i) {
      if(write(fd, &nullByte, 1) != 1) {
        std::cerr << "Could not overwrite file " << strerror(errno) << "\n";
         close(fd);
         return 1;
        }
    }

    close(fd);

    if(unlink(filePath) != 0) {
      std::cerr << " Could not delete file " << strerror(errno) << "\n";
      return 1;
    }

    std::cout << "File " << filePath << "erased and removed.\n";
    return 0;
}
