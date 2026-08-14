#include "SystemInfo.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sys/utsname.h>
#include <sys/statvfs.h>

void SystemInfo::displayOSInfo() const {
    struct utsname buffer;
    if (uname(&buffer) != 0) {
        perror("uname failed");
        return;
    }
    
    std::cout << "\n========================================\n";
    std::cout << "          SYSTEM INFORMATION            \n";
    std::cout << "========================================\n";
    std::cout << "Operating System    : " << buffer.sysname << "\n";
    std::cout << "Hostname            : " << buffer.nodename << "\n";
    std::cout << "Kernel Version      : " << buffer.release << "\n";
    std::cout << "Version             : " << buffer.version << "\n";
    std::cout << "Machine Architecture: " << buffer.machine << "\n";
}

void SystemInfo::displayMemoryInfo() const {
    std::ifstream meminfo("/proc/meminfo");
    if (!meminfo.is_open()) {
        std::cerr << "Error: Could not open /proc/meminfo\n";
        return;
    }

    std::string line;
    std::cout << "\n========================================\n";
    std::cout << "          MEMORY INFORMATION            \n";
    std::cout << "========================================\n";
    while (std::getline(meminfo, line)) {
        if (line.find("MemTotal:") == 0 || line.find("MemAvailable:") == 0) {
            std::cout << line << "\n";
        }
    }
}

void SystemInfo::displayDiskInfo() const {
    struct statvfs buffer;
    if (statvfs("/", &buffer) != 0) {
        perror("statvfs failed");
        return;
    }

    unsigned long long total = buffer.f_blocks * buffer.f_frsize;
    unsigned long long free = buffer.f_bfree * buffer.f_frsize;
    unsigned long long used = total - free;

    const double GB = 1024.0 * 1024.0 * 1024.0;

    std::cout << "\n========================================\n";
    std::cout << "          DISK INFORMATION (/)          \n";
    std::cout << "========================================\n";
    std::cout << "Total Disk Space : " << (total / GB) << " GB\n";
    std::cout << "Used Disk Space  : " << (used / GB) << " GB\n";
    std::cout << "Free Disk Space  : " << (free / GB) << " GB\n";
}