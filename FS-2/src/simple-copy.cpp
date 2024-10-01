#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
    if(argc != 3) {
      std::cerr << "Usage: " << argv[0] << " <source file> <destination file>\n";
      return 1;
    }

    std::ifstream srcFile(argv[1], std::ios::binary);
    if(!srcFile) {
      std::cerr << "Could not open source file " << argv[1] << ".\n";
      return 1;
    }

    std::ofstream destFile(argv[2], std::ios::binary);
    if(!destFile) {
      std::cerr << "Could not open destination file " << argv[2] << ".\n";
      return 1;
    }

    destFile << srcFile.rdbuf();
    std::cout << "File copied from " << argv[1] << " to " << argv[2] << ".\n";

    return 0;
}
