#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
int countVowels(const string& s) {
    int count = 0;
    for (char c : s) {
        char lower = tolower(c);
        if (lower == 'a' || lower == 'e' || lower == 'i' || lower == 'o' || lower == 'u') {
            count++;
        }
    }
    return count;
}
string reverseString(const string& s) {
    string result = s;
    int left = 0;
    int right = result.size() - 1;

    while (left < right) {
        swap(result[left], result[right]);
        left++;
        right--;
    }
    return result;
}
int main() {
    string str;
    cout << "Enter a string: ";
    getline(cin, str);
    int vowelCount = countVowels(str);
    cout << "\nNumber of vowels in \"" << str << "\": " << vowelCount << endl;
    string reversed = reverseString(str);
    cout << "Reversed string: " << reversed << endl;
    return 0;
}