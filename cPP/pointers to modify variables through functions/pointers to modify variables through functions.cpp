#include <iostream>
using namespace std;


void increment(int* num);
void decrement(int* num);
void reset(int* num);
void printNumber(int* num);

int main() {
    int number;
    int choice;

    cout << "Enter number: ";
    cin >> number;

    do {
        cout << "\n1. Increment\n";
        cout << "2. Decrement\n";
        cout << "3. Reset\n";
        cout << "4. Show\n";
        cout << "5. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            increment(&number);
            printNumber(&number);
            break;
        case 2:
            decrement(&number);
            printNumber(&number);
            break;
        case 3:
            reset(&number);
            printNumber(&number);
            break;
        case 4:
            printNumber(&number);
            break;
        case 5:
            cout << "Exiting program...\n";
            break;
        default:
            cout << "Invalid choice! Please try again.\n";
        }

    } while (choice != 5);

    return 0;
}


void increment(int* num) {
    (*num)++;
}

void decrement(int* num) {
    (*num)--;
}

void reset(int* num) {
    *num = 0;
}

void printNumber(int* num) {
    cout << "Current Number = " << *num << endl;
}