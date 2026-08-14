#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <sys/types.h>

class ProcessManager {
public:
    void listProcesses() const;
    void displayProcessInfo(pid_t pid) const;
    void createChildProcess() const;
    void sendSignal(pid_t pid, int signalOption) const;
};

#endif // PROCESS_MANAGER_H