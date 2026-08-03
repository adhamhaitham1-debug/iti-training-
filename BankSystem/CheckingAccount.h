#ifndef CHECKINGACCOUNT_H
#define CHECKINGACCOUNT_H

#include "Account.h"

using namespace std;

class CheckingAccount : public Account {
private:
    static constexpr double OVERDRAFT_LIMIT = 500.0;

public:
    CheckingAccount(const string& id, const string& owner, double initialBalance = 0.0);

    string getAccountType() const override;
    bool withdraw(double amount) override;
    void display() const override;
    string serialize() const override;
};

#endif 