#include <iostream>
#include <vector>

using namespace std;


void findTwoSum(const vector<int>& v, int k) {
    for (size_t i = 0; i < v.size(); i++) {
        for (size_t j = i + 1; j < v.size(); j++) {
            if (v[i] + v[j] == k) {
                cout << "Found  " << v[i] << " + " << v[j] << " = " << k << endl;
                return; 
            }
        }
    }
    cout << "No two elements sum up to " << k << endl;
}

int main() {
    vector<int> v;
    int count;

    cout << "How many numbers do you want to enter? ";
    cin >> count;

    cout << "Enter " << count << " numbers:" << endl;
    for (int i = 0; i < count; i++) {
        int num;
        cin >> num;
        v.push_back(num);
    }

    int k;
    cout << "Enter the target sum (K): ";
    cin >> k;

    findTwoSum(v, k);

    return 0;
}