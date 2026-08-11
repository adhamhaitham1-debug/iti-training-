#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <memory>
#include <stdexcept>

using namespace std;

class Account {
protected:
    string accountId;
    string ownerName;
    double balance;

public:
    Account(const string& id, const string& owner, double initialBalance = 0.0);
    virtual ~Account() = default;

    virtual string getAccountType() const = 0;
    virtual bool withdraw(double amount) = 0;
    virtual void display() const = 0;
    virtual string serialize() const = 0;

  
    void deposit(double amount);
    string getAccountId() const;
    string getOwnerName() const;
    double getBalance() const;

  
    static unique_ptr<Account> createFromData(const string& type,
        const string& id,
        const string& owner,
        double balance);
};

#endif 