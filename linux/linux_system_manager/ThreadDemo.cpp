#include "ThreadDemo.h"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

void ThreadDemo::threadWorker(int id, int sleepTimeSec) {
    std::cout << "  [Thread " << id << "] Started (Work duration: " << sleepTimeSec << "s)\n";
    std::this_thread::sleep_for(std::chrono::seconds(sleepTimeSec));
    std::cout << "  [Thread " << id << "] Working...\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "  [Thread " << id << "] Finished!\n";
}

void ThreadDemo::runDemo() const {
    std::cout << "\n========================================\n";
    std::cout << "          MULTI-THREAD DEMO             \n";
    std::cout << "========================================\n";
    std::cout << "Spawning 3 concurrent worker threads...\n\n";

    std::vector<std::thread> threads;

    // Launch 3 threads with different work times
    threads.emplace_back(threadWorker, 1, 2);
    threads.emplace_back(threadWorker, 2, 3);
    threads.emplace_back(threadWorker, 3, 1);

    std::cout << "Main thread waiting for all child threads to join()...\n\n";

    // Wait for all threads to finish
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    std::cout << "\nAll 3 threads have completed execution successfully!\n";
}