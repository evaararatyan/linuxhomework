#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <fcntl.h>

void executeCommand(const std::vector<std::string>& args, bool silent, const std::string& outputFile, bool append) {
    pid_t pid = fork();
    if(pid == -1) {
       perror("Failed to fork");
       return;
    }

    if(pid == 0) {
       // Child process
       if(silent) {
          std::string logFile = std::to_string(getpid()) + ".log";
          int logFd = open(logFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
           dup2(logFd, STDOUT_FILENO);
           dup2(logFd, STDERR_FILENO);
            close(logFd);
        } 
       else if(!outputFile.empty()) {
           int flags = O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC);
            int fd = open(outputFile.c_str(), flags, 0644);
            if(fd == -1) {
                perror("Failed to open output file");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            dup2(fd, STDERR_FILENO);
            close(fd);
        }

         std::vector<char*> execArgs;
        for(const auto& arg : args) {
           execArgs.push_back(const_cast<char*>(arg.c_str()));
        }
        execArgs.push_back(nullptr);

           if(execvp(execArgs[0], execArgs.data()) == -1) {
            perror("Failed to execute command");
            exit(EXIT_FAILURE);
        }
    }
    else {
       int status;
        waitpid(pid, &status, 0);
    }
}

void parseAndExecute(const std::string& input) {
    std::istringstream iss(input);
    std::string token;
    bool silent = false;
    bool shouldExecute = true;
    bool append = false;
    std::string outputFile;

    std::vector<std::string> currentCommand;

    while(iss >> token) {
        if(token == ";") {
          if(!currentCommand.empty() && shouldExecute) {
             executeCommand(currentCommand, silent, outputFile, append);
            }
            currentCommand.clear();
            silent = false;
            outputFile.clear();
            append = false;
            shouldExecute = true;
        }
       	else if(token == "&&") {
            if(!currentCommand.empty() && shouldExecute) {
               executeCommand(currentCommand, silent, outputFile, append);
               shouldExecute = true;
            }
	    else {
               shouldExecute = false;
            }
            currentCommand.clear();
            silent = false;
            outputFile.clear();
            append = false;
        } else if(token == "||") {
            if(!currentCommand.empty() && !shouldExecute) {
               executeCommand(currentCommand, silent, outputFile, append);
               shouldExecute = true;
            } 
	    else {
                shouldExecute = false;
            }
            currentCommand.clear();
            silent = false;
            outputFile.clear();
            append = false;
	} 
	else if(token == ">") {
            iss >> outputFile;
            append = false;
        }
       	else if(token == ">>") {
            iss >> outputFile;
            append = true;
        }
       	else if(token == "silent") {
            silent = true;
        }
       	else {
            currentCommand.push_back(token);
        }
    }

      if(!currentCommand.empty() && shouldExecute) {
        executeCommand(currentCommand, silent, outputFile, append);
    }
}

int main() {
    std::string input;
    char* path = getenv("PATH");
    if(path == nullptr) {
        std::cerr << "Error: PATH environment variable not found." << std::endl;
        return 1;
    }

    std::string currentDir = ".";
    setenv("PATH", (currentDir + ":" + path).c_str(), 1);

    while(true) {
       std::cout << "> ";
       std::getline(std::cin, input);

        if(input == "exit") {
           break;
        }
parseAndExecute(input);
    }

    return 0;
}
