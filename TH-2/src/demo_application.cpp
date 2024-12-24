#include "parallel_scheduler.h"
#include <iostream>
#include <chrono>

void task_function(int id) {
    std::cout << "Task " << id << " started." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Task " << id << " finished." << std::endl;
}

int main() {
    parallel_scheduler scheduler(4);

    for (int i = 0; i < 10; ++i) {
        scheduler.run([i]() { task_function(i); });
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));
    return 0;
}
