#ifndef THREAD_DEMO_H
#define THREAD_DEMO_H

class ThreadDemo {
public:
    void runDemo() const;
private:
    static void threadWorker(int id, int sleepTimeSec);
};

#endif // THREAD_DEMO_H