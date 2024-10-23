#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctime>

void do_command(char** argv) {
    pid_t pid = fork();

    if(pid == -1) {
       perror("failed");
       exit(1);
    }

    if(pid == 0) {
      execvp(argv[0], argv);
      perror("fail");
      exit(1);
    } 
    
    else {
      int status;
      time_t start_time = time(NULL);
      waitpid(pid, &status, 0);
      time_t end_time = time(NULL);
      double duration = difftime(end_time, start_time);
      std::cout << "Command completed with exit code " << WEXITSTATUS(status)
                << "took " << duration << " seconds" << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if(argc < 2) {
      std::cerr << "Usage:" << argv[0] << " <command> [arguments]" << std::endl;
        return 1;
    }

    do_command(argv + 1);

    return 0;
}
