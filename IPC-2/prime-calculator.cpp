#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>

int nth_prime(int n) {
    std::vector<int> primes;
    for(int i = 2; primes.size() < n; ++i) {
        bool is_prime = true;
        for(int p : primes) {
           if(i % p == 0) {
               is_prime = false;
                break;
            }
        }
        if(is_prime) {
            primes.push_back(i);
        }
    }
    return primes[n - 1];
}

int main() {
    int pipe_parent_to_child[2];
    int pipe_child_to_parent[2];

    if (pipe(pipe_parent_to_child) == -1 || pipe(pipe_child_to_parent) == -1) {
        perror("Pipe creation failed");
        return 1;
    }

    pid_t pid = fork();

    if(pid < 0) {
        perror("Fork failed");
        return 1;
    }

    if(pid == 0) {
        close(pipe_parent_to_child[1]);
        close(pipe_child_to_parent[0]);

        while (true) {
            int m;
            if(read(pipe_parent_to_child[0], &m, sizeof(m)) <= 0) {
                break;
            }

            std::cout << "[Child] Calculating " << m << "-th prime number..." << std::endl;
            int result = nth_prime(m);
            std::cout << "[Child] Sending" << m << ")..." << std::endl;

            write(pipe_child_to_parent[1], &result, sizeof(result));
        }

        close(pipe_parent_to_child[0]);
        close(pipe_child_to_parent[1]);
    }
    else {
        close(pipe_parent_to_child[0]);
        close(pipe_child_to_parent[1]);

        std::string input;
        while(true) {
            std::cout << "[Parent] enter the number: ";
            std::getline(std::cin, input);

            if(input == "exit") {
                break;
            }

            int m;
            std::istringstream iss(input);
            if (!(iss >> m)) {
                std::cerr << "[Parent] Invalid input" << std::endl;
                continue;
            }

            std::cout << "[Parent] Sending " << m << " to the child process" << std::endl;
            write(pipe_parent_to_child[1], &m, sizeof(m));

            std::cout << "[Parent] Waiting for the response" << std::endl;
            int result;
            if (read(pipe_child_to_parent[0], &result, sizeof(result)) > 0) {
                std::cout << "[Parent] Received  result" << m << ") = " << result << "..." << std::endl;
            } else {
                std::cerr << "[Parent] Failed to receive" << std::endl;
            }
        }

        close(pipe_parent_to_child[1]);
        close(pipe_child_to_parent[0]);

        std::cout << "[Parent] ExitinG" << std::endl;
    }

    return 0;
}
