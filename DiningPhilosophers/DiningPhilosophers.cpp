#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <cstdlib>
#include <ctime>

sem_t *sticks[5];
void philosopher(int id) {srand(time(nullptr) + id);
    while(true) {
        std::cout << "Философ " << id << " размышляет\n";
        sleep(rand() % 3 + 1);

       if(id == 0) {
           sem_wait(sticks[(id + 1) % 5]);
           sem_wait(sticks[id]);
        }
       else {
           sem_wait(sticks[id]);
           sem_wait(sticks[(id + 1) % 5]);
        }

       std::cout << "Философ " << id << " ест\n";
        sleep(rand() % 3 + 1);

        sem_post(sticks[id]);
        sem_post(sticks[(id + 1) % 5]);
    }
}

int main() {for(int i = 0; i < 5; ++i) {
       std::string sem_name = "/stick" + std::to_string(i);
       sticks[i] = sem_open(sem_name.c_str(), O_CREAT, 0666, 1);
    }

     for(int i = 0; i < 5; ++i) {
       pid_t pid = fork();
        if(pid == 0) {
           philosopher(i);
           exit(0);
        }
    }

     for(int i = 0; i < 5; ++i) {
        wait(nullptr);
    }

      for(int i = 0; i < 5; ++i) {
        sem_close(sticks[i]);
    }

    return 0;
}
