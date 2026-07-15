#include <iostream>
#include <vector>
#include <algorithm> 
#include <sstream>   

using namespace std;

vector<int> readVector(const string& name) {
    cout << "Enter elements for " << name << " (space-separated), then press Enter: ";

    string line;
    getline(cin, line);

    vector<int> vec;
    stringstream ss(line);
    int value;
    while (ss >> value) {
        vec.push_back(value);
    }
    return vec;
}

int main() {
    vector<int> vec1 = readVector("vec1");
    vector<int> vec2 = readVector("vec2");

    int maxElem = *max_element(vec1.begin(), vec1.end());
    cout << "Max element in vec1: " << maxElem << endl;

    int minElem = *min_element(vec1.begin(), vec1.end());
    cout << "Min element in vec1: " << minElem << endl;

    int maxElem2 = *max_element(vec2.begin(), vec2.end());
    cout << "Max element in vec2: " << maxElem2 << endl;

    int minElem2 = *min_element(vec2.begin(), vec2.end());
    cout << "Min element in vec2: " << minElem2 << endl;

    if (vec1 == vec2) {
        cout << "vec1 and vec2 are equal" << endl;
    }
    else {
        cout << "vec1 and vec2 are NOT equal" << endl;
    }

    cout << endl;
    int smaller = min(vec1.size(), vec2.size());

    for (int i = 0; i < smaller; i++) {
        cout << "vec1[" << i << "] = " << vec1[i]
            << "  vs  vec2[" << i << "] = " << vec2[i] << "  ->  ";

        if (vec1[i] == vec2[i]) {
            cout << "equal";
        }
        else if (vec1[i] > vec2[i]) {
            cout << "vec1 is bigger";
        }
        else {
            cout << "vec2 is bigger";
        }
        cout << endl;
    }

    reverse(vec1.begin(), vec1.end());
    cout << "\nvec1 after reverse: ";
    for (int i = 0; i < vec1.size(); i++) {
        cout << vec1[i] << " ";
    }
    cout << endl;


    reverse(vec2.begin(), vec2.end());
    cout << "vec2 after reverse: ";
    for (int i = 0; i < vec2.size(); i++) {
        cout << vec2[i] << " ";
    }
    cout << endl;

    return 0;
}