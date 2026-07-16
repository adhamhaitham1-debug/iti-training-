#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

void reverseInPlace(vector<int>& v);
void countOccurrences(vector<int>& v);
int findSecondLargest(vector<int>& v);
void removeDuplicates(vector<int>& v);
void printVector(vector<int>& v);

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> v(n);
    cout << "Enter " << n << " elements:\n";
    for (int i = 0; i < n; i++) {
        cin >> v[i];
    }

    int choice;
    do {
        cout << "\n----- Vector Problem Menu -----\n";
        cout << "1. Reverse in place\n";
        cout << "2. Count of number occurrences (frequencies)\n";
        cout << "3. Find second largest element\n";
        cout << "4. Remove duplicates\n";
        cout << "5. Show current vector\n";
        cout << "6. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            reverseInPlace(v);
            cout << "Vector reversed.\n";
            printVector(v);
            break;
        case 2:
            countOccurrences(v);
            break;
        case 3: {
            int result = findSecondLargest(v);
            if (result == -1)
                cout << "Not enough distinct elements.\n";
            else
                cout << "Second largest element = " << result << endl;
            break;
        }
        case 4:
            removeDuplicates(v);
            cout << "Duplicates removed.\n";
            printVector(v);
            break;
        case 5:
            printVector(v);
            break;
        case 6:
            cout << "Exiting program...\n";
            break;
        default:
            cout << "Invalid choice! Try again.\n";
        }

    } while (choice != 6);

    return 0;
}

void reverseInPlace(vector<int>& v) {
    int left = 0;
    int right = v.size() - 1;

    while (left < right) {
        swap(v[left], v[right]);
        left++;
        right--;
    }
}

void countOccurrences(vector<int>& v) {
    map<int, int> freq;

    for (int num : v) {
        freq[num]++;
    }

    cout << "\nElement Frequencies:\n";
    for (auto& pair : freq) {
        cout << pair.first << " -> " << pair.second << " time(s)\n";
    }
}

int findSecondLargest(vector<int>& v) {
    if (v.size() < 2) return -1;

    
    int largest = v[0];
    int secondLargest = -1;  

    for (int i = 1; i < v.size(); i++) {
        int num = v[i];

        if (num > largest) {
            secondLargest = largest;
            largest = num;
        }
        else if (num > secondLargest && num != largest) {
            secondLargest = num;
        }
    }

    return secondLargest;
}

void removeDuplicates(vector<int>& v) {
    vector<int> result;
    map<int, bool> seen;

    for (int num : v) {
        if (!seen[num]) {
            result.push_back(num);
            seen[num] = true;
        }
    }

    v = result;
}

void printVector(vector<int>& v) {
    cout << "Vector: ";
    for (int num : v) {
        cout << num << " ";
    }
    cout << endl;
}