#include <iostream>
using namespace std;

int main() {
    double num1, num2, result;
    char op;
    cout << "calculator" << endl;

    cout << "Enter operator (+, -, *, /): ";
    cin >> op;

    cout << "Enter first number: ";
    cin >> num1;

    cout << "Enter second number: ";
    cin >> num2;

    switch (op) {
    case '+':
        result = num1 + num2;
        cout << "Result: " << result << endl;
        break;
    case '-':
        result = num1 - num2;
        cout << "Result: " << result << endl;
        break;
    case '*':
        result = num1 * num2;
        cout << "Result: " << result << endl;
        break;
    case '/':
        if (num2 != 0) {
            result = num1 / num2;
            cout << "Result: " << result << endl;
        }
        else {
            cout << "Error: Division by zero is not allowed." << endl;
        }
        break;
    default:
        cout << "Error: Invalid operator." << endl;
    }

    return 0;
}
