#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

int main() {
    string employeeName;
    int employeeID;
    double workingHrs;
    double hourlyRate;
    char position;
    double overtimeHrs = 0;
    char addBonus;
    double bonusAmount = 0;

    cout << fixed << setprecision(2);

    cout << "Employee name: ";
    getline(cin, employeeName);

    cout << "Employee ID: ";
    cin >> employeeID;

    cout << "Working Hrs: ";
    cin >> workingHrs;

    cout << "Hourly Rate: ";
    cin >> hourlyRate;

    cout << "Position (m = manager, e = employee): ";
    cin >> position;
    while (position != 'm' && position != 'e') {
        cout << "Invalid position. Enter m or e: ";
        cin >> position;
    }

    cout << "OverTime hrs: ";
    cin >> overtimeHrs;
    cout << "\n";

    double salaryBeforeTax = (workingHrs * hourlyRate) + (overtimeHrs * hourlyRate * 1.5);
    cout << "Salary before TAX: " << salaryBeforeTax << " $\n\n";

    cout << "Add Bouns ? (y/n): ";
    cin >> addBonus;

    if (addBonus == 'y' || addBonus == 'Y') {
        cout << "Bouns added.\n";
        bonusAmount = (position == 'm') ? 100.00 : 50.00;
    }

    double tax = (salaryBeforeTax + bonusAmount) * 0.10;
    double finalSalary = (salaryBeforeTax + bonusAmount) - tax;

    cout << "===================PayRoll===================\n";
    cout << "Employee: " << employeeName << "\n";
    cout << "ID : " << employeeID << "\n";
    cout << "TAX: " << tax << " $\n";
    cout << "Final Salary: " << finalSalary << " $\n";

    return 0;
}
