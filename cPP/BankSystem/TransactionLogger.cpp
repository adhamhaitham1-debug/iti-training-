#include "TransactionLogger.h"
#include "Exceptions.h"
#include <iostream>
#include <filesystem>

using namespace std;

TransactionLogger::TransactionLogger(const string& path) : logFilePath(path) {

}

string TransactionLogger::getCurrentTimestamp() const {
    auto now = chrono::system_clock::now();
    time_t time = chrono::system_clock::to_time_t(now);
    tm localTime{};
#ifdef _WIN32
    localtime_s(&localTime, &time);
#else
    localtime_r(&time, &localTime);
#endif
    ostringstream oss;
    oss << put_time(&localTime, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void TransactionLogger::log(const string& sourceAccount,
    const string& destAccountOrType,
    double amount,
    const string& description) {
    ofstream logFile(logFilePath, ios::app);
    if (!logFile.is_open()) {
  
        cerr << "[Warning] Could not open transaction log file: " << logFilePath << endl;
        return;
    }

    logFile << getCurrentTimestamp() << " | "
        << "Source: " << sourceAccount << " | "
        << "Dest/Op: " << destAccountOrType << " | "
        << "Amount: " << fixed << setprecision(2) << amount << " | "
        << "Desc: " << description << endl;
}