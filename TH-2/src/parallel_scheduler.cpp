#include "parallel_scheduler.h"

parallel_scheduler::parallel_scheduler(size_t capacity) : stop(false) {
    for (size_t i = 0; i < capacity; ++i) {
        workers.emplace_back(&parallel_scheduler::worker_thread, this);
    }
}

parallel_scheduler::~parallel_scheduler() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for (auto& worker : workers) {
        worker.join();
    }
}

void parallel_scheduler::run(const std::function<void()>& func) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        tasks.emplace(func);
    }
    condition.notify_one();
}

void parallel_scheduler::worker_thread() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            condition.wait(lock, [this]() { return stop || !tasks.empty(); });
            if (stop && tasks.empty()) return;
            task = std::move(tasks.front());
            tasks.pop();
        }
        task();
    }
}
