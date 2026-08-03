#ifndef SAVINGSACCOUNT_H
#define SAVINGSACCOUNT_H

#include "Account.h"

using namespace std;

class SavingsAccount : public Account {
private:
    static constexpr double MIN_BALANCE = 100.0; 

public:
    SavingsAccount(const string& id, const string& owner, double initialBalance = 0.0);

    string getAccountType() const override;
    bool withdraw(double amount) override;
    void display() const override;
    string serialize() const override;
};

#endif 