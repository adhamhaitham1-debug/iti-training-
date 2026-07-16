#include <iostream>
#include <vector>
using namespace std;

// 1) Merge two sorted vectors into one sorted vector
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

// 2) Bonus: Remove negative numbers from a vector
void removeNegatives(vector<int>& v) {
    vector<int> filtered;
    for (int x : v) {
        if (x >= 0) filtered.push_back(x);
    }
    v = filtered;
}

// 3) Check if two vectors have the same sum
bool haveSameSum(const vector<int>& a, const vector<int>& b) {
    int sumA = 0, sumB = 0;
    for (int x : a) sumA += x;
    for (int x : b) sumB += x;
    return sumA == sumB;
}

// Helper to read a vector from the user
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

// Helper to print a vector
void printVector(const vector<int>& v) {
    cout << "[ ";
    for (int x : v) cout << x << " ";
    cout << "]" << endl;
}

int main() {
    // ---- Task 1: Merge two sorted vectors ----
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

    // ---- Task 2 (Bonus): Remove negative numbers from v1 and v2 ----
    removeNegatives(v1);
    removeNegatives(v2);

    cout << "\nVector 1 without negatives: ";
    printVector(v1);
    cout << "Vector 2 without negatives: ";
    printVector(v2);

    // ---- Task 3: Check if two vectors have same sum ----
    cout << "\n--- Enter two vectors to compare sums ---" << endl;
    vector<int> a = readVector("Vector A");
    vector<int> b = readVector("Vector B");

    cout << "\nVector A: ";
    printVector(a);
    cout << "Vector B: ";
    printVector(b);

    if (haveSameSum(a, b))
        cout << "The two vectors have the SAME sum." << endl;
    else
        cout << "The two vectors have DIFFERENT sums." << endl;

    return 0;
}