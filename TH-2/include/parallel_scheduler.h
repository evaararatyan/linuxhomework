#ifndef
#define

#include <functional>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

class parallel_scheduler {
public:
    explicit parallel_scheduler(size_t capacity);
    ~parallel_scheduler();

    void run(const std::function<void()>& func);

private:
    void worker_thread();

    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

#endif
