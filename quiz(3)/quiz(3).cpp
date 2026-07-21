
#include <iostream>
#include <string>
using namespace std;

int main() {
    int n;
    cout << "Enter n (how many digits in your streaam numbers):  ";
    cin >> n;

    bool present[100] = { false };

    cout << "Enter " << n << " (should be stream of numbers from 0 to " << n << " without spaces) : ";
    string input;
    cin >> input;

    for (int i = 0; i < input.length(); i++) {
        int digit = input[i] - '0';
        present[digit] = true;
    }

    for (int i = 0; i <= n; i++) {
        if (present[i] == false) {
            cout << "Missing number is: " << i << endl;
        }
    }

    return 0;
}