#include "Bank.h"
#include "Exceptions.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <random>
#include <filesystem>
#include <string>
#include <vector>
#include <memory>      
#include <algorithm>   
#include <stdexcept>   


Bank::Bank(const string& accountsFile, const string& logFile)
    : logger(logFile), accountsFilePath(accountsFile) {

    loadAccounts();
}

void Bank::validatePositiveAmount(double amount) const {
    if (amount <= 0.0) {
        throw InvalidAmountException();
    }
}

string Bank::generateAccountId() const {
    
    int maxNum = 1000;
    for (const auto& acc : accounts) {
        const string& id = acc->getAccountId();
        if (id.size() > 3 && id.substr(0, 3) == "ACC") {
            try {
                int num = stoi(id.substr(3));
                if (num >= maxNum) maxNum = num + 1;
            }
            catch (...) {}
        }
    }
    ostringstream oss;
    oss << "ACC" << maxNum;
    return oss.str();
}

vector<unique_ptr<Account>>::iterator Bank::findAccount(const string& accountId) {
    return find_if(accounts.begin(), accounts.end(),
        [&accountId](const unique_ptr<Account>& acc) {
            return acc->getAccountId() == accountId;
        });
}

vector<unique_ptr<Account>>::const_iterator Bank::findAccount(const string& accountId) const {
    return find_if(accounts.begin(), accounts.end(),
        [&accountId](const unique_ptr<Account>& acc) {
            return acc->getAccountId() == accountId;
        });
}

void Bank::loadAccounts() {
    ifstream file(accountsFilePath);
    if (!file.is_open()) {
        cout << "[Info] No existing accounts file found. Starting with empty bank.\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        stringstream ss(line);
        string type, id, owner, balanceStr;
        if (!getline(ss, type, '|') || !getline(ss, id, '|') ||
            !getline(ss, owner, '|') || !getline(ss, balanceStr)) {
            cerr << "[Warning] Skipping malformed line: " << line << endl;
            continue;
        }

        try {
            double bal = stod(balanceStr);
            accounts.push_back(Account::createFromData(type, id, owner, bal));
        }
        catch (const exception& e) {
            cerr << "[Warning] Failed to load account: " << e.what() << endl;
        }
    }
    cout << "[Info] Loaded " << accounts.size() << " account(s) from file.\n";
}

void Bank::saveAccounts() const {
    ofstream file(accountsFilePath);
    if (!file.is_open()) {
        throw FileException("Cannot open accounts file for writing: " + accountsFilePath);
    }

    file << "# type|id|owner|balance\n";
    for (const auto& acc : accounts) {
        file << acc->serialize() << "\n";
    }
    cout << "[Info] Accounts saved successfully (" << accounts.size() << " records).\n";
}

void Bank::createAccount(const string& type, const string& ownerName, double initialBalance) {
    if (ownerName.empty()) {
        throw InvalidOperationException("Owner name cannot be empty");
    }

    string id = generateAccountId();
    unique_ptr<Account> newAcc;

    string lowerType = type;
    for (char& c : lowerType) c = static_cast<char>(tolower(c));

    if (lowerType == "savings" || lowerType == "s") {
        newAcc = make_unique<SavingsAccount>(id, ownerName, initialBalance);
    }
    else if (lowerType == "checking" || lowerType == "c") {
        newAcc = make_unique<CheckingAccount>(id, ownerName, initialBalance);
    }
    else {
        throw InvalidOperationException("Invalid account type. Use 'Savings' or 'Checking'");
    }

    accounts.push_back(move(newAcc));
    logger.log(id, "CREATE", initialBalance, "New " + type + " account for " + ownerName);
    cout << "Account created successfully. ID: " << id << endl;
}

void Bank::deposit(const string& accountId, double amount) {
    validatePositiveAmount(amount);

    auto it = findAccount(accountId);
    if (it == accounts.end()) {
        throw AccountNotFoundException(accountId);
    }

    (*it)->deposit(amount);
    logger.log(accountId, "DEPOSIT", amount, "Deposit into account");
    cout << "Deposit successful. New balance: "
        << fixed << setprecision(2) << (*it)->getBalance() << endl;
}

void Bank::withdraw(const string& accountId, double amount) {
    validatePositiveAmount(amount);

    auto it = findAccount(accountId);
    if (it == accounts.end()) {
        throw AccountNotFoundException(accountId);
    }

    (*it)->withdraw(amount);
    logger.log(accountId, "WITHDRAW", amount, "Withdrawal from account");
    cout << "Withdrawal successful. New balance: "
        << fixed << setprecision(2) << (*it)->getBalance() << endl;
}

void Bank::transfer(const string& fromId, const string& toId, double amount) {
    if (fromId == toId) {
        throw InvalidOperationException("Cannot transfer to the same account");
    }
    validatePositiveAmount(amount);

    auto fromIt = findAccount(fromId);
    if (fromIt == accounts.end()) {
        throw AccountNotFoundException(fromId);
    }
    auto toIt = findAccount(toId);
    if (toIt == accounts.end()) {
        throw AccountNotFoundException(toId);
    }

    (*fromIt)->withdraw(amount);
    (*toIt)->deposit(amount);

    logger.log(fromId, toId, amount, "Transfer between accounts");
    cout << "Transfer successful.\n"
        << "  From " << fromId << " new balance: "
        << fixed << setprecision(2) << (*fromIt)->getBalance() << "\n"
        << "  To   " << toId << " new balance: "
        << (*toIt)->getBalance() << endl;
}

void Bank::displayAllAccounts() const {
    if (accounts.empty()) {
        cout << "No accounts in the system.\n";
        return;
    }

    cout << "\n" << string(60, '=') << "\n";
    cout << left << setw(12) << "Account ID"
        << setw(20) << "Owner"
        << setw(12) << "Type"
        << right << setw(12) << "Balance" << "\n";
    cout << string(60, '-') << "\n";

    for (const auto& acc : accounts) {
        acc->display(); 
    }
    cout << string(60, '=') << "\n";
    cout << "Total accounts: " << accounts.size() << "\n";
}

void Bank::searchAccount(const string& query) const {
    bool found = false;
    cout << "\nSearch results for \"" << query << "\":\n";
    cout << string(60, '-') << "\n";

    for (const auto& acc : accounts) {
        if (acc->getAccountId() == query ||
            acc->getOwnerName().find(query) != string::npos) {
            acc->display();
            found = true;
        }
    }

    if (!found) {
        cout << "No matching accounts found.\n";
    }
}

size_t Bank::getAccountCount() const {
    return accounts.size();
}
