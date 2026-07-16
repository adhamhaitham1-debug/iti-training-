#include <iostream>
#include <vector>
using namespace std;


vector<int> mergeSortedVectors(const vector<int>& a, const vector<int>& b) {
    vector<int> result;
    size_t i = 0, j = 0;

    while (i < a.size() && j < b.size()) {
        if (a[i] <= b[j]) {
            result.push_back(a[i++]);
        }
        else {
            result.push_back(b[j++]);
        }
    }
    while (i < a.size()) result.push_back(a[i++]);
    while (j < b.size()) result.push_back(b[j++]);

    return result;
}


void removeNegatives(vector<int>& v) {
    vector<int> filtered;
    for (int x : v) {
        if (x >= 0) filtered.push_back(x);
    }
    v = filtered;
}


int vectorSum(const vector<int>& v) {
    int sum = 0;
    for (int x : v) sum += x;
    return sum;
}


bool haveSameSum(int sumA, int sumB) {
    return sumA == sumB;
}


vector<int> readVector(const string& name) {
    int n;
    cout << "Enter the number of elements in " << name << ": ";
    cin >> n;

    vector<int> v(n);
    cout << "Enter " << n << " elements for " << name << ": ";
    for (int i = 0; i < n; i++) {
        cin >> v[i];
    }
    return v;
}

void printVector(const vector<int>& v) {
    cout << "[ ";
    for (int x : v) cout << x << " ";
    cout << "]" << endl;
}

int main() {

    cout << "--- Enter first sorted vector ---" << endl;
    vector<int> v1 = readVector("Vector 1");

    cout << "\n--- Enter second sorted vector ---" << endl;
    vector<int> v2 = readVector("Vector 2");

    cout << "\nVector 1: ";
    printVector(v1);
    cout << "Vector 2: ";
    printVector(v2);

    vector<int> merged = mergeSortedVectors(v1, v2);
    cout << "Merged sorted vector: ";
    printVector(merged);


    removeNegatives(v1);
    removeNegatives(v2);

    cout << "\nVector 1 without negatives: ";
    printVector(v1);
    cout << "Vector 2 without negatives: ";
    printVector(v2);

    int sum1 = vectorSum(v1);
    int sum2 = vectorSum(v2);

    cout << "\nSum of Vector 1: " << sum1 << endl;
    cout << "Sum of Vector 2: " << sum2 << endl;

    if (haveSameSum(sum1, sum2))
        cout << "The two vectors have the SAME sum." << endl;
    else
        cout << "The two vectors have DIFFERENT sums." << endl;

    return 0;
}