#include <iostream>
#include <string>
using namespace std;


void inputMarks(int* math, int* physics, int* english);
double calculateAverage(int* math, int* physics, int* english);
char calculateGrade(double average);
void printReport(string name, int* math, int* physics, int* english);


int getMark(string subject) {
    int mark;
    bool valid = false;

    do {
        cout << "Enter " << subject << " marks: ";
        cin >> mark;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input! Please enter a number.\n";
        }
        else if (mark < 0 || mark > 100) {
            cout << "Marks must be between 0 and 100! Try again.\n";
        }
        else {
            valid = true;
        }

    } while (!valid);

    return mark;
}

int main() {
    string name;
    int math, physics, english;
    double average;
    char grade;
    int choice;

    cout << "Enter student name: ";
    getline(cin, name);

    inputMarks(&math, &physics, &english);

    average = calculateAverage(&math, &physics, &english);
    grade = calculateGrade(average);

    printReport(name, &math, &physics, &english);

    do {
        cout << "\n----- Menu -----\n";
        cout << "1. Update Math\n";
        cout << "2. Update Physics\n";
        cout << "3. Update English\n";
        cout << "4. Recalculate Average\n";
        cout << "5. Print Report\n";
        cout << "6. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            math = getMark("new Math");
            break;
        case 2:
            physics = getMark("new Physics");
            break;
        case 3:
            english = getMark("new English");
            break;
        case 4:
            average = calculateAverage(&math, &physics, &english);
            grade = calculateGrade(average);
            cout << "Average recalculated: " << average << " | Grade: " << grade << endl;
            break;
        case 5:
            printReport(name, &math, &physics, &english);
            break;
        case 6:
            cout << "Exiting program...\n";
            break;
        default:
            cout << "Invalid choice! Please try again.\n";
        }

    } while (choice != 6);

    return 0;
}


void inputMarks(int* math, int* physics, int* english) {
    *math = getMark("Math");
    *physics = getMark("Physics");
    *english = getMark("English");
}

double calculateAverage(int* math, int* physics, int* english) {
    return (*math + *physics + *english) / 3.0;
}

char calculateGrade(double average) {
    if (average >= 90) return 'A';
    else if (average >= 80) return 'B';
    else if (average >= 70) return 'C';
    else if (average >= 60) return 'D';
    else return 'F';
}

void printReport(string name, int* math, int* physics, int* english) {
    double average = calculateAverage(math, physics, english);
    char grade = calculateGrade(average);

    cout << "\n----- Student Report -----\n";
    cout << "Student: " << name << endl;
    cout << "Math    : " << *math << endl;
    cout << "Physics : " << *physics << endl;
    cout << "English : " << *english << endl;
    cout << "Average : " << average << endl;
    cout << "Grade   : " << grade << endl;
}