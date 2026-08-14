# 🐧 Linux System Manager

[![Language: C++17](https://img.shields.io/badge/Language-C%2B%2B17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![Platform: Linux](https://img.shields.io/badge/Platform-Linux%20%2F%20POSIX-orange.svg)](https://www.kernel.org/)
[![Build: Makefile](https://img.shields.io/badge/Build-Makefile-green.svg)](https://www.gnu.org/software/make/)

A high-performance, modular terminal application built in C++ to inspect, monitor, and interact with the Linux operating system. This project directly utilizes **POSIX APIs**, **native Linux system calls**, and `/proc` pseudo-filesystem parsing—avoiding unsafe `system()` wrappers.

---

## 📸 Menu Preview

```text
========================================
          LINUX SYSTEM MANAGER          
========================================
SYSTEM
 1. System Information
 2. Memory Information
 3. Disk Information

PROCESSES
 4. List Processes
 5. Process Information
 6. Create Child Process
 7. Send Signal to Process

FILES
 8. File Information
 9. Change File Permissions

NETWORK
10. Network Information
11. Ping Host
12. DNS Lookup

THREADS
13. Multi-Thread Demo

 0. Exit
========================================


