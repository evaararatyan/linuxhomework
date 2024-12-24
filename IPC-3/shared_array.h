#ifndef SHARED_ARRAY_H
#define SHARED_ARRAY_H

#include <iostream>
#include <string>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <semaphore.h>

class shared_array {
public:
    shared_array(const std::string& name, size_t size)
        : name(name), size(size) {
        shm_fd = shm_open(name.c_str(), O_CREAT | O_RDWR, 0666);
        if(shm_fd == -1) {
            perror("shm_open failed");
            exit(1);
        }

        if(ftruncate(shm_fd, size * sizeof(int)) == -1) {
            perror("ftruncate failed");
            exit(1);
        }

        data = static_cast<int*>(mmap(nullptr, size * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));
        if(data == MAP_FAILED) {
            perror("mmap failed");
            exit(1);
        }

        semaphore = sem_open((name + "_sem").c_str(), O_CREAT, 0666, 1);
        if(semaphore == SEM_FAILED) {
            perror("sem_open failed");
            exit(1);
        }
    }

    ~shared_array() {
        munmap(data, size * sizeof(int));
        close(shm_fd);
        sem_close(semaphore);
        sem_unlink((name + "_sem").c_str());
        shm_unlink(name.c_str());
    }

    int& operator[](size_t index) {
        if(index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    void lock() {
        sem_wait(semaphore);
    }

    void unlock() {
        sem_post(semaphore);
    }

private:
    std::string name;
    size_t size;
    int* data;
    int shm_fd;
    sem_t* semaphore;
};

#endif
