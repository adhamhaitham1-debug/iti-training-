#include "ProcessManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <cctype>
#include <unistd.h>
#include <sys/wait.h>
#include <csignal>
#include <iomanip>

static bool isNumber(const std::string& str) {
    for (char const &c : str) {
        if (!std::isdigit(c)) return false;
    }
    return !str.empty();
}

void ProcessManager::listProcesses() const {
    DIR* dir = opendir("/proc");
    if (!dir) {
        perror("Failed to open /proc");
        return;
    }

    struct dirent* entry;
    std::cout << "\n=================================================================\n";
    std::cout << std::left << std::setw(10) << "PID" 
              << std::setw(25) << "NAME" 
              << std::setw(18) << "STATE" 
              << std::setw(10) << "PPID" << "\n";
    std::cout << "=================================================================\n";

    int count = 0;
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type == DT_DIR && isNumber(entry->d_name)) {
            std::string pidStr = entry->d_name;
            std::string statusPath = "/proc/" + pidStr + "/status";
            std::ifstream statusFile(statusPath);

            if (statusFile.is_open()) {
                std::string line;
                std::string name = "N/A", state = "N/A", ppid = "N/A";

                while (std::getline(statusFile, line)) {
                    if (line.rfind("Name:", 0) == 0) {
                        name = line.substr(5);
                        size_t start = name.find_first_not_of(" \t");
                        name = (start != std::string::npos) ? name.substr(start) : name;
                    } else if (line.rfind("State:", 0) == 0) {
                        state = line.substr(6);
                        size_t start = state.find_first_not_of(" \t");
                        state = (start != std::string::npos) ? state.substr(start) : state;
                    } else if (line.rfind("PPid:", 0) == 0) {
                        ppid = line.substr(5);
                        size_t start = ppid.find_first_not_of(" \t");
                        ppid = (start != std::string::npos) ? ppid.substr(start) : ppid;
                    }
                }

                std::cout << std::left << std::setw(10) << pidStr 
                          << std::setw(25) << name 
                          << std::setw(18) << state 
                          << std::setw(10) << ppid << "\n";
                
                count++;
                if (count >= 20) { // Limiting display to keep console clean
                    std::cout << "... (Showing first 20 processes)\n";
                    break;
                }
            }
        }
    }
    closedir(dir);
}

void ProcessManager::displayProcessInfo(pid_t pid) const {
    std::string statusPath = "/proc/" + std::to_string(pid) + "/status";
    std::ifstream statusFile(statusPath);

    if (!statusFile.is_open()) {
        std::cout << "Error: Process with PID " << pid << " does not exist or permission denied.\n";
        return;
    }

    std::cout << "\n========================================\n";
    std::cout << "      PROCESS DETAILS (PID: " << pid << ")\n";
    std::cout << "========================================\n";
    std::string line;
    while (std::getline(statusFile, line)) {
        if (line.rfind("Name:", 0) == 0 || line.rfind("State:", 0) == 0 ||
            line.rfind("Pid:", 0) == 0  || line.rfind("PPid:", 0) == 0  ||
            line.rfind("Threads:", 0) == 0 || line.rfind("VmSize:", 0) == 0 ||
            line.rfind("VmRSS:", 0) == 0) {
            std::cout << line << "\n";
        }
    }
}

void ProcessManager::createChildProcess() const {
    std::cout << "\n--- Creating Child Process using fork() ---\n";
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
    } else if (pid == 0) {
        // Child Process
        std::cout << " [CHILD] Hello from Child Process!\n";
        std::cout << " [CHILD] Child PID  : " << getpid() << "\n";
        std::cout << " [CHILD] Parent PID : " << getppid() << "\n";
        std::cout << " [CHILD] Child exiting with exit status 42...\n";
        _exit(42); // Immediate exit for child
    } else {
        // Parent Process
        std::cout << " [PARENT] Parent PID             : " << getpid() << "\n";
        std::cout << " [PARENT] Spawned Child with PID: " << pid << "\n";
        std::cout << " [PARENT] Waiting for child process to terminate...\n";

        int status;
        waitpid(pid, &status, 0); // Reaping child process

        if (WIFEXITED(status)) {
            std::cout << " [PARENT] Child process exited cleanly with status code: " 
                      << WEXITSTATUS(status) << "\n";
        } else {
            std::cout << " [PARENT] Child process ended abnormally.\n";
        }
    }
}

void ProcessManager::sendSignal(pid_t pid, int signalOption) const {
    int sig = 0;
    switch (signalOption) {
        case 1: sig = SIGTERM; break;
        case 2: sig = SIGSTOP; break;
        case 3: sig = SIGCONT; break;
        case 4: sig = SIGKILL; break;
        default:
            std::cout << "Invalid signal choice!\n";
            return;
    }

    if (kill(pid, sig) == 0) {
        std::cout << "Signal successfully sent to process " << pid << ".\n";
    } else {
        perror("Failed to send signal");
    }
}