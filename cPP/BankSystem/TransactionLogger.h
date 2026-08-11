#ifndef TRANSACTIONLOGGER_H
#define TRANSACTIONLOGGER_H

#include <string>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>

using namespace std;

class TransactionLogger {
private:
    string logFilePath;

    string getCurrentTimestamp() const;

public:
    explicit TransactionLogger(const string& path = "transactions.log");

    void log(const string& sourceAccount,
        const string& destAccountOrType,
        double amount,
        const string& description);
};

#endif 