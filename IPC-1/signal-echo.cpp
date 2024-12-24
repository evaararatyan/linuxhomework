#include <iostream>
#include <csignal>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <ucontext.h>

void signalHandler(int signal, siginfo_t* info, void* context) {
    if (signal == SIGUSR1) {
        std::cout << "Received signal " 
                  << info->si_pid << " executed by " 
                  << info->si_uid << " (";

        struct passwd* pw = getpwuid(info->si_uid);
        if(pw) {
            std::cout << pw->pw_name;
        } 
	else {
            std::cout << "unknown user";
        }

        std::cout << ")." << std::endl;

        ucontext_t* uctx = (ucontext_t*)context;
        std::cout << "State of the context: "
                  << "RIP = " << std::hex << uctx->uc_mcontext.gregs[REG_RIP]
                  << ", RAX = " << uctx->uc_mcontext.gregs[REG_RAX]
                  << ", RBX = " << uctx->uc_mcontext.gregs[REG_RBX]
                  << std::endl;
    }
}

int main() {
    std::cout << "Process ID: " << getpid() << std::endl;

    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO; 
    sa.sa_sigaction = signalHandler;

    sigaction(SIGUSR1, &sa, nullptr);

    while(true) {
        sleep(10);
    }

    return 0;
}
