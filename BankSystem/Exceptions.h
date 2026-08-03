#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

using namespace std;


class BankException : public runtime_error {
public:
    explicit BankException(const string& message) : runtime_error(message) {}
};

class InvalidAmountException : public BankException {
public:
    explicit InvalidAmountException(const string& message = "Amount must be positive")
        : BankException(message) {
    }
};

class AccountNotFoundException : public BankException {
public:
    explicit AccountNotFoundException(const string& id)
        : BankException("Account not found: " + id) {
    }
};

class InsufficientFundsException : public BankException {
public:
    explicit InsufficientFundsException(const string& message = "Insufficient funds")
        : BankException(message) {
    }
};

class InvalidOperationException : public BankException {
public:
    explicit InvalidOperationException(const string& message)
        : BankException(message) {
    }
};

class FileException : public BankException {
public:
    explicit FileException(const string& message)
        : BankException(message) {
    }
};

#endif 