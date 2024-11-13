#include <iostream>
#include <vector>
#include <pthread.h>
#include <cstdlib>
#include <sys/time.h>

struct ThreadData {
    const int* arr;
    size_t start;
    size_t end;
    long long result;
};

void* threadFunction(void* arg) {
   ThreadData* data = static_cast<ThreadData*>(arg);
   long long sum = 0;
    for(size_t i = data->start; i < data->end; ++i) {
       sum += data->arr[i];
    }
    data->result = sum;
    return nullptr;
}


long getCurrentTimeMillis() {
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " N M" << std::endl;
        return 1;
    }
   size_t N = std::atoi(argv[1]);
    size_t M = std::atoi(argv[2]);
    std::vector<int> arr(N);
    for (auto& val : arr) val = std::rand() % 100;

    long startTime = getCurrentTimeMillis();
    long long singleThreadSum = 0;
    for (const auto& val : arr) singleThreadSum += val;
    long endTime = getCurrentTimeMillis();
    std::cout << "Time without threads: " << (endTime - startTime) << " ms" << std::endl;

    std::vector<pthread_t> threads(M);
    std::vector<ThreadData> threadData(M);
    size_t partSize = N / M;
    startTime = getCurrentTimeMillis();

    for(size_t i = 0; i < M; ++i) {
        threadData[i] = {arr.data(), i * partSize, (i == M - 1) ? N : (i + 1) * partSize, 0};
        pthread_create(&threads[i], nullptr, threadFunction, &threadData[i]);
     }

    long long multiThreadSum = 0;
    for(size_t i = 0; i < M; ++i) {
       pthread_join(threads[i], nullptr);
       multiThreadSum += threadData[i].result;
    }
    endTime = getCurrentTimeMillis();
    std::cout << "Time " << M << " threads " << (endTime - startTime) << " ms" << std::endl;

    return 0;
}
