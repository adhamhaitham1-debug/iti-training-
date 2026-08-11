#include "Account.h"
#include "SavingsAccount.h"
#include "CheckingAccount.h"
#include "Exceptions.h"
#include <iostream>
#include <iomanip>

using namespace std;

Account::Account(const string& id, const string& owner, double initialBalance)
    : accountId(id), ownerName(owner), balance(initialBalance) {
    if (initialBalance < 0.0) {
        throw InvalidAmountException("Initial balance cannot be negative");
    }
}

void Account::deposit(double amount) {
    if (amount <= 0.0) {
        throw InvalidAmountException("Deposit amount must be positive");
    }
    balance += amount;
}

string Account::getAccountId() const {
    return accountId;
}

string Account::getOwnerName() const {
    return ownerName;
}

double Account::getBalance() const {
    return balance;
}

unique_ptr<Account> Account::createFromData(const string& type,
    const string& id,
    const string& owner,
    double bal) {
    if (type == "Savings") {
        return make_unique<SavingsAccount>(id, owner, bal);
    }
    if (type == "Checking") {
        return make_unique<CheckingAccount>(id, owner, bal);
    }
    throw InvalidOperationException("Unknown account type: " + type);
}