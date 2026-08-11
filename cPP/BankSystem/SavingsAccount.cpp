#include "SavingsAccount.h"
#include "Exceptions.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

SavingsAccount::SavingsAccount(const string& id, const string& owner, double initialBalance)
    : Account(id, owner, initialBalance) {
    if (initialBalance < MIN_BALANCE) {
        throw InvalidAmountException(
            "Savings account requires minimum balance of " + to_string(static_cast<int>(MIN_BALANCE)));
    }
}

string SavingsAccount::getAccountType() const {
    return "Savings";
}

bool SavingsAccount::withdraw(double amount) {
    if (amount <= 0.0) {
        throw InvalidAmountException("Withdrawal amount must be positive");
    }
    if (balance - amount < MIN_BALANCE) {
        throw InsufficientFundsException(
            "Savings account cannot go below minimum balance of " +
            to_string(static_cast<int>(MIN_BALANCE)));
    }
    balance -= amount;
    return true;
}

void SavingsAccount::display() const {
    cout << left << setw(12) << accountId
        << setw(20) << ownerName
        << setw(12) << "Savings"
        << right << fixed << setprecision(2) << setw(12) << balance << endl;
}

string SavingsAccount::serialize() const {
    ostringstream oss;
    oss << "Savings|" << accountId << "|" << ownerName << "|" << fixed << setprecision(2) << balance;
    return oss.str();
}