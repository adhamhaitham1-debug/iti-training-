#include <iostream>
#include <string>
#include "SystemInfo.h"
#include "ProcessManager.h"
#include "FileManager.h"
#include "NetworkManager.h"
#include "ThreadDemo.h"

void displayMenu() {
    std::cout << "\n========================================\n";
    std::cout << "          LINUX SYSTEM MANAGER          \n";
    std::cout << "========================================\n";
    std::cout << "SYSTEM\n";
    std::cout << " 1. System Information\n";
    std::cout << " 2. Memory Information\n";
    std::cout << " 3. Disk Information\n\n";
    std::cout << "PROCESSES\n";
    std::cout << " 4. List Processes\n";
    std::cout << " 5. Process Information\n";
    std::cout << " 6. Create Child Process\n";
    std::cout << " 7. Send Signal to Process\n\n";
    std::cout << "FILES\n";
    std::cout << " 8. File Information\n";
    std::cout << " 9. Change File Permissions\n\n";
    std::cout << "NETWORK\n";
    std::cout << "10. Network Information\n";
    std::cout << "11. Ping Host\n";
    std::cout << "12. DNS Lookup\n\n";
    std::cout << "THREADS\n";
    std::cout << "13. Multi-Thread Demo\n\n";
    std::cout << " 0. Exit\n";
    std::cout << "========================================\n";
    std::cout << "Enter choice: ";
}

int main() {
    SystemInfo sysInfo;
    ProcessManager procManager;
    FileManager fileManager;
    NetworkManager netManager;
    ThreadDemo threadDemo;

    int choice;
    bool running = true;

    while (running) {
        displayMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1: sysInfo.displayOSInfo(); break;
            case 2: sysInfo.displayMemoryInfo(); break;
            case 3: sysInfo.displayDiskInfo(); break;

            case 4: procManager.listProcesses(); break;
            case 5: {
                pid_t pid;
                std::cout << "Enter PID: ";
                if (std::cin >> pid) procManager.displayProcessInfo(pid);
                break;
            }
            case 6: procManager.createChildProcess(); break;
            case 7: {
                pid_t pid;
                int sigOpt;
                std::cout << "Enter target PID: ";
                if (std::cin >> pid) {
                    std::cout << "Select Signal:\n";
                    std::cout << " 1. SIGTERM (15)\n 2. SIGSTOP (19)\n 3. SIGCONT (18)\n 4. SIGKILL (9)\nChoice: ";
                    if (std::cin >> sigOpt) procManager.sendSignal(pid, sigOpt);
                }
                break;
            }

            case 8: {
                std::string path;
                std::cout << "Enter file path: ";
                std::cin >> path;
                fileManager.displayFileInfo(path);
                break;
            }
            case 9: {
                std::string path, octalMode;
                std::cout << "Enter file path: ";
                std::cin >> path;
                std::cout << "Enter permission (e.g. 755, 644, 700): ";
                std::cin >> octalMode;
                fileManager.changePermissions(path, octalMode);
                break;
            }

            case 10: netManager.displayNetworkInfo(); break;
            case 11: {
                std::string host;
                std::cout << "Enter host or IP: ";
                std::cin >> host;
                netManager.pingHost(host);
                break;
            }
            case 12: {
                std::string hostname;
                std::cout << "Enter hostname (e.g. google.com): ";
                std::cin >> hostname;
                netManager.dnsLookup(hostname);
                break;
            }

            case 13: threadDemo.runDemo(); break;

            case 0:
                std::cout << "Exiting Linux System Manager...\n";
                running = false;
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}