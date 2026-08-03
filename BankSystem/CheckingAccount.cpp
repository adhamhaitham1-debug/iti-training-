#include "CheckingAccount.h"
#include "Exceptions.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

CheckingAccount::CheckingAccount(const string& id, const string& owner, double initialBalance)
    : Account(id, owner, initialBalance) {
}

string CheckingAccount::getAccountType() const {
    return "Checking";
}

bool CheckingAccount::withdraw(double amount) {
    if (amount <= 0.0) {
        throw InvalidAmountException("Withdrawal amount must be positive");
    }
    if (balance - amount < -OVERDRAFT_LIMIT) {
        throw InsufficientFundsException(
            "Checking account overdraft limit of " +
            to_string(static_cast<int>(OVERDRAFT_LIMIT)) + " exceeded");
    }
    balance -= amount;
    return true;
}

void CheckingAccount::display() const {
    cout << left << setw(12) << accountId
        << setw(20) << ownerName
        << setw(12) << "Checking"
        << right << fixed << setprecision(2) << setw(12) << balance << endl;
}

string CheckingAccount::serialize() const {
    ostringstream oss;
    oss << "Checking|" << accountId << "|" << ownerName << "|" << fixed << setprecision(2) << balance;
    return oss.str();
}