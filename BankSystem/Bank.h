#ifndef BANK_H
#define BANK_H

#include "Account.h"
#include "SavingsAccount.h"
#include "CheckingAccount.h"
#include "TransactionLogger.h"
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <stdexcept>

using namespace std;

class Bank {
private:
    vector<unique_ptr<Account>> accounts;
    TransactionLogger logger;
    string accountsFilePath;

    vector<unique_ptr<Account>>::iterator findAccount(const string& accountId);
    vector<unique_ptr<Account>>::const_iterator findAccount(const string& accountId) const;

    string generateAccountId() const;
    void validatePositiveAmount(double amount) const;

public:
    explicit Bank(const string& accountsFile = "accounts.dat",
        const string& logFile = "transactions.log");

    void loadAccounts();
    void saveAccounts() const;

    void createAccount(const string& type, const string& ownerName, double initialBalance);
    void deposit(const string& accountId, double amount);
    void withdraw(const string& accountId, double amount);
    void transfer(const string& fromId, const string& toId, double amount);
    void displayAllAccounts() const;
    void searchAccount(const string& query) const; 
    size_t getAccountCount() const;
};

#endif 