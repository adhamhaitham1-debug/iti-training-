#include <iostream>
#include <iomanip>
using namespace std;

class ATM {
private:
    double balance;

public:
    ATM(double initialBalance = 0.0) {
        balance = initialBalance;
    }

    void checkBalance() {
        cout << fixed << setprecision(2);
        cout << "\nYour current balance is: $" << balance << endl;
    }

    void deposit() {
        double amount;
        cout << "\nEnter amount to deposit: $";
        cin >> amount;

        if (amount <= 0) {
            cout << "Invalid amount. Deposit must be positive.\n";
            return;
        }

        balance += amount;
        cout << fixed << setprecision(2);
        cout << "Deposit successful! New balance: $" << balance << endl;
    }

    void withdraw() {
        double amount;
        cout << "\nEnter amount to withdraw: $";
        cin >> amount;

        if (amount <= 0) {
            cout << "Invalid amount. Withdrawal must be positive.\n";
            return;
        }

        if (amount > balance) {
            cout << "Insufficient funds. Your balance is $" << fixed << setprecision(2) << balance << endl;
            return;
        }

        balance -= amount;
        cout << fixed << setprecision(2);
        cout << "Withdrawal successful! New balance: $" << balance << endl;
    }

    void displayMenu() {
        cout << "\n========================\n";
        cout << "     ATM SIMULATION\n";
        cout << "========================\n";
        cout << "1. Check Balance\n";
        cout << "2. Deposit\n";
        cout << "3. Withdraw\n";
        cout << "4. Exit\n";
        cout << "========================\n";
        cout << "Enter your choice: ";
    }

    void run() {
        int choice;

        do {
            displayMenu();
            cin >> choice;

            switch (choice) {
            case 1:
                checkBalance();
                break;
            case 2:
                deposit();
                break;
            case 3:
                withdraw();
                break;
            case 4:
                cout << "\nThank you for using the ATM. Goodbye!\n";
                break;
            default:
                cout << "\nInvalid choice. Please select a number between 1 and 4.\n";
            }

        } while (choice != 4);
    }
};

int main() {
    double startingBalance;

    cout << "Enter starting balance: $";
    cin >> startingBalance;

    ATM myATM(startingBalance);
    myATM.run();

    return 0;
}