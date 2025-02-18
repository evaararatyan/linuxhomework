#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <cstdlib>
#include <ctime>
#include <csignal>
#include <mutex>

sem_t *tobacco, *paper, *matches, *agent;
std::mutex print_mutex;

void smoker(int id, sem_t *needed_item, char output) {
    while(true) {
        sem_wait(needed_item);
        print_mutex.lock();
        std::cout << output << std::flush;
        print_mutex.unlock();
        sleep(1);
        sem_post(agent);
    }
}

void bartender() {
    char item;
    while(std::cin >> item) {
        sem_wait(agent);
        switch (item) {
           case 't': sem_post(tobacco); break;
           case 'p': sem_post(paper); break;
           case 'm': sem_post(matches); break;
        }
    }
    kill(0, SIGTERM);
}

int main() {
    tobacco = sem_open("/tobacco", O_CREAT, 0666, 0);
    paper = sem_open("/paper", O_CREAT, 0666, 0);
    matches = sem_open("/matches", O_CREAT, 0666, 0);
    agent = sem_open("/agent", O_CREAT, 0666, 1);

    pid_t smokers[3];
    smokers[0] = fork();
    if(smokers[0] == 0) smoker(0, tobacco, 'T');
    smokers[1] = fork();
    if(smokers[1] == 0) smoker(1, paper, 'P');
    smokers[2] = fork();
    if(smokers[2] == 0) smoker(2, matches, 'M');

    bartender();
    
    for(int i = 0; i < 3; ++i) {
       wait(nullptr);
    }

    sem_close(tobacco);
    sem_close(paper);
    sem_close(matches);
    sem_close(agent);
    
    return 0;
}
