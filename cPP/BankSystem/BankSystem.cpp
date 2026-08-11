#include "Bank.h"
#include "Exceptions.h"
#include <iostream>
#include <limits>
#include <string>

using namespace std;


void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int readMenuChoice() {
    int choice;
    cout << "Enter choice: ";
    if (!(cin >> choice)) {
        clearInput();
        return -1; 
    }
    return choice;
}

double readAmount(const string& prompt) {
    double amount;
    cout << prompt;
    if (!(cin >> amount)) {
        clearInput();
        throw InvalidAmountException("Invalid number entered for amount");
    }
    return amount;
}

string readString(const string& prompt) {
    string value;
    cout << prompt;

    if (cin.peek() == '\n') cin.ignore();
    getline(cin, value);
    return value;
}

void printMenu() {
    cout << "\n========== BANK SYSTEM MENU ==========\n"
        << "1. Create Account\n"
        << "2. Deposit Money\n"
        << "3. Withdraw Money\n"
        << "4. Transfer Money\n"
        << "5. Display All Accounts\n"
        << "6. Search Account (by ID or Owner)\n"
        << "7. Save & Exit\n"
        << "======================================\n";
}

int main() {
    cout << "=== Welcome to the Bank System ===\n";
    cout << "Loading data...\n";

    Bank bank("accounts.dat", "transactions.log");

    bool running = true;
    while (running) {
        try {
            printMenu();
            int choice = readMenuChoice();

            switch (choice) {
            case 1: {
                cout << "Account type (Savings / Checking): ";
                string type;
                cin >> type;
                string owner = readString("Owner name: ");
                double initial = readAmount("Initial deposit: ");
                bank.createAccount(type, owner, initial);
                break;
            }
            case 2: {
                string id = readString("Account ID: ");
                double amount = readAmount("Amount to deposit: ");
                bank.deposit(id, amount);
                break;
            }
            case 3: {
                string id = readString("Account ID: ");
                double amount = readAmount("Amount to withdraw: ");
                bank.withdraw(id, amount);
                break;
            }
            case 4: {
                string fromId = readString("From Account ID: ");
                string toId = readString("To Account ID: ");
                double amount = readAmount("Amount to transfer: ");
                bank.transfer(fromId, toId, amount);
                break;
            }
            case 5: {
                bank.displayAllAccounts();
                break;
            }
            case 6: {
                string query = readString("Enter Account ID or Owner name: ");
                bank.searchAccount(query);
                break;
            }
            case 7: {
                bank.saveAccounts();
                cout << "Thank you for using the Bank System. Goodbye!\n";
                running = false;
                break;
            }
            default:
                cout << "Invalid choice. Please select 1-7.\n";
                break;
            }
        }
        catch (const BankException& e) {
            cout << "[Error] " << e.what() << endl;
        }
        catch (const exception& e) {
            cout << "[Unexpected Error] " << e.what() << endl;
        }
        catch (...) {
            cout << "[Unknown Error] An unexpected problem occurred.\n";
        }
    }

    return 0;
}