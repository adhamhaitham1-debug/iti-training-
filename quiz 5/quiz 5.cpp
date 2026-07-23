#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <cstring>

// ---------------------------------------------------------------
// Problem 1: Count each character in a string
// Example: banana -> a:3, b:1, n:2
// ---------------------------------------------------------------
void problem1()
{
    std::cout << "\n===== Problem 1: Character Count =====\n";
    std::cout << "Enter a word: ";
    std::string word;
    std::cin >> word; // reads the whole word (stops at whitespace)

    // Allocate a raw char buffer managed by a smart pointer
    // unique_ptr<char[]> owns the dynamically allocated array
    std::unique_ptr<char[]> buffer(new char[word.size() + 1]);
    std::strcpy(buffer.get(), word.c_str());

    // Use a raw pointer to walk through the smart-pointer-owned array
    char* p = buffer.get();

    std::map<char, int> counts; // will store letter -> frequency, sorted alphabetically

    while (*p != '\0')
    {
        counts[*p]++;
        ++p; // pointer arithmetic to move to next character
    }

    std::cout << "Result: ";
    bool first = true;
    for (const auto& pair : counts)
    {
        if (!first) std::cout << ", ";
        std::cout << pair.first << ":" << pair.second;
        first = false;
    }
    std::cout << "\n";
}
/*
// ---------------------------------------------------------------
// Problem 2: Find intersection of two vectors (arrays)
// Ask the user for the number of elements in each vector,
// then read the elements themselves.
// ---------------------------------------------------------------

// Helper: fills a dynamic array (owned by a unique_ptr) with user input
std::unique_ptr<int[]> readVector(const std::string& label, int size)
{
    std::unique_ptr<int[]> arr(new int[size]);
    std::cout << "Enter " << size << " elements for " << label << ": ";
    for (int i = 0; i < size; ++i)
    {
        std::cin >> arr[i]; // operator[] works fine on unique_ptr<int[]>
    }
    return arr; // ownership moved out (return-value optimization / move)
}

void problem2()
{
    std::cout << "\n===== Problem 2: Vector Intersection =====\n";

    int n1 = 0, n2 = 0;
    std::cout << "Enter number of elements in V1: ";
    std::cin >> n1;
    std::cout << "Enter number of elements in V2: ";
    std::cin >> n2;

    std::unique_ptr<int[]> v1 = readVector("V1", n1);
    std::unique_ptr<int[]> v2 = readVector("V2", n2);

    // Result buffer: worst case every element of the smaller vector matches
    int maxPossible = std::min(n1, n2);
    std::unique_ptr<int[]> result(new int[maxPossible]);
    int resultCount = 0;

    // Raw pointers used to walk through the smart-pointer-owned arrays
    int* p1 = v1.get();
    int* p2 = v2.get();

    for (int i = 0; i < n1; ++i, ++p1)
    {
        p2 = v2.get(); // reset pointer to start of v2 for each i
        for (int j = 0; j < n2; ++j, ++p2)
        {
            if (*p1 == *p2)
            {
                // avoid adding duplicates already found
                bool alreadyAdded = false;
                for (int k = 0; k < resultCount; ++k)
                {
                    if (result[k] == *p1) { alreadyAdded = true; break; }
                }
                if (!alreadyAdded)
                {
                    result[resultCount] = *p1;
                    ++resultCount;
                }
                break;
            }
        }
    }

    std::cout << "out = ";
    for (int i = 0; i < resultCount; ++i)
    {
        std::cout << result[i];
        if (i != resultCount - 1) std::cout << ", ";
    }
    std::cout << "\n";
}

int main()
{
    problem1();
    problem2();
    return 0;
}
*/