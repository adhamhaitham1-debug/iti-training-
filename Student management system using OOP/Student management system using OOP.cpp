#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>

using namespace std;


class Person {
protected:
    int id;
    string name;
    int age;

public:
    Person() : id(0), name(""), age(0) {}
    Person(int id, const string& name, int age)
        : id(id), name(name), age(age) {
    }

    virtual ~Person() {} 

    int getId() const { return id; }
    string getName() const { return name; }
    int getAge() const { return age; }

    void setName(const string& n) { name = n; }
    void setAge(int a) { age = a; }

    virtual void display() const {
        cout << "ID: " << id << " | Name: " << name << " | Age: " << age;
    }

    virtual string serialize() const {
        return to_string(id) + "," + name + "," + to_string(age);
    }
};

class Student : public Person {
private:
    string department;
    vector<pair<string, double>> grades; 

public:
    Student() : Person(), department("") {}

    Student(int id, const string& name, int age, const string& department)
        : Person(id, name, age), department(department) {
    }

    string getDepartment() const { return department; }
    const vector<pair<string, double>>& getGrades() const { return grades; }

    void setDepartment(const string& d) { department = d; }

    void addGrade(const string& course, double grade) {
        for (auto& g : grades) {
            if (g.first == course) {
                g.second = grade; 
                return;
            }
        }
        grades.push_back({ course, grade }); 
    }

    double getAverageGrade() const {
        if (grades.empty()) return 0.0;
        double sum = 0.0;
        for (const auto& g : grades) sum += g.second;
        return sum / grades.size();
    }

    void display() const override {
        Person::display();
        cout << " | Department: " << department
            << " | Avg Grade: " << getAverageGrade();
        cout << "\n    Courses: ";
        if (grades.empty()) {
            cout << "(none)";
        }
        else {
            for (size_t i = 0; i < grades.size(); ++i) {
                cout << grades[i].first << "=" << grades[i].second;
                if (i + 1 < grades.size()) cout << ", ";
            }
        }
        cout << "\n";
    }


    string serialize() const override {
        string line = Person::serialize() + "," + department + "|";
        for (size_t i = 0; i < grades.size(); ++i) {
            line += grades[i].first + ":" + to_string(grades[i].second);
            if (i + 1 < grades.size()) line += ";";
        }
        return line;
    }


    static Student deserialize(const string& line) {
        string mainPart, gradesPart;
        size_t barPos = line.find('|');
        if (barPos != string::npos) {
            mainPart = line.substr(0, barPos);
            gradesPart = line.substr(barPos + 1);
        }
        else {
            mainPart = line;
            gradesPart = "";
        }

        stringstream ss(mainPart);
        string idStr, name, ageStr, department;
        getline(ss, idStr, ',');
        getline(ss, name, ',');
        getline(ss, ageStr, ',');
        getline(ss, department, ',');

        Student s(stoi(idStr), name, stoi(ageStr), department);

        if (!gradesPart.empty()) {
            stringstream gs(gradesPart);
            string entry;
            while (getline(gs, entry, ';')) {
                size_t colonPos = entry.find(':');
                if (colonPos != string::npos) {
                    string course = entry.substr(0, colonPos);
                    double grade = stod(entry.substr(colonPos + 1));
                    s.addGrade(course, grade);
                }
            }
        }

        return s;
    }
};


class SchoolManager {
private:
    vector<Student> students;
    string filename;
    int nextId;

public:
    explicit SchoolManager(const string& filename = "students.txt")
        : filename(filename), nextId(1) {
        load();
    }

    ~SchoolManager() {
        save(); 
    }


    int addStudent(const string& name, int age, const string& department) {
        int id = nextId++;
        students.emplace_back(id, name, age, department);
        return id;
    }


    Student* findById(int id) {
        for (auto& s : students) {
            if (s.getId() == id) return &s;
        }
        return nullptr;
    }

    vector<Student*> findByName(const string& name) {
        vector<Student*> results;
        for (auto& s : students) {
            if (s.getName() == name) results.push_back(&s);
        }
        return results;
    }

    const vector<Student>& getAll() const { return students; }

    bool updateStudent(int id, const string& name, int age, const string& department) {
        Student* s = findById(id);
        if (!s) return false;
        if (!name.empty()) s->setName(name);
        if (age > 0) s->setAge(age);
        if (!department.empty()) s->setDepartment(department);
        return true;
    }

    bool addGradeToStudent(int id, const string& course, double grade) {
        Student* s = findById(id);
        if (!s) return false;
        s->addGrade(course, grade);
        return true;
    }

    bool removeStudent(int id) {
        auto it = remove_if(students.begin(), students.end(),
            [id](const Student& s) { return s.getId() == id; });
        if (it == students.end()) return false;
        students.erase(it, students.end());
        return true;
    }

    void listAll() const {
        if (students.empty()) {
            cout << "No students on record.\n";
            return;
        }
        for (const auto& s : students) {
            s.display();
            cout << "-------------------------------------------\n";
        }
    }

    void sortByName() {
        sort(students.begin(), students.end(),
            [](const Student& a, const Student& b) { return a.getName() < b.getName(); });
    }

    void sortByAverageGrade(bool descending = true) {
        sort(students.begin(), students.end(),
            [descending](const Student& a, const Student& b) {
                return descending ? a.getAverageGrade() > b.getAverageGrade()
                    : a.getAverageGrade() < b.getAverageGrade();
            });
    }

    void save() const {
        ofstream out(filename);
        if (!out) {
            cerr << "Warning: could not open " << filename << " for saving.\n";
            return;
        }
        for (const auto& s : students) {
            out << s.serialize() << "\n";
        }
    }

    void load() {
        ifstream in(filename);
        if (!in) return; 

        students.clear();
        string line;
        int maxId = 0;
        while (getline(in, line)) {
            if (line.empty()) continue;
            Student s = Student::deserialize(line);
            students.push_back(s);
            maxId = max(maxId, s.getId());
        }
        nextId = maxId + 1;
    }
};


void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int readInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            clearInput();
            return value;
        }
        cout << "Invalid number, try again.\n";
        clearInput();
    }
}

double readDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            clearInput();
            return value;
        }
        cout << "Invalid number, try again.\n";
        clearInput();
    }
}

string readLine(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

void printMenu() {
    cout << "\n========== STUDENT MANAGEMENT SYSTEM ==========\n";
    cout << "1.  Add student\n";
    cout << "2.  List all students\n";
    cout << "3.  Search student by ID\n";
    cout << "4.  Search student by name\n";
    cout << "5.  Update student info\n";
    cout << "6.  Add/update a grade for a student\n";
    cout << "7.  Remove student\n";
    cout << "8.  Sort by name\n";
    cout << "9.  Sort by average grade (highest first)\n";
    cout << "10. Save now\n";
    cout << "0.  Exit\n";
    cout << "================================================\n";
}


int main() {
    SchoolManager manager("students.txt");
    bool running = true;

    cout << "Welcome! Loaded " << manager.getAll().size() << " student(s) from file.\n";

    while (running) {
        printMenu();
        int choice = readInt("Choose an option: ");

        switch (choice) {
        case 1: {
            string name = readLine("Name: ");
            int age = readInt("Age: ");
            string dept = readLine("Department: ");
            int id = manager.addStudent(name, age, dept);
            cout << "Student added with ID " << id << "\n";
            break;
        }
        case 2: {
            manager.listAll();
            break;
        }
        case 3: {
            int id = readInt("Enter ID: ");
            Student* s = manager.findById(id);
            if (s) s->display();
            else cout << "No student found with ID " << id << "\n";
            break;
        }
        case 4: {
            string name = readLine("Enter name: ");
            auto results = manager.findByName(name);
            if (results.empty()) {
                cout << "No student found with name \"" << name << "\"\n";
            }
            else {
                for (auto* s : results) s->display();
            }
            break;
        }
        case 5: {
            int id = readInt("Enter ID to update: ");
            cout << "Leave blank / enter 0 to keep current value.\n";
            string name = readLine("New name: ");
            int age = readInt("New age (0 to skip): ");
            string dept = readLine("New department: ");
            if (manager.updateStudent(id, name, age, dept))
                cout << "Student updated.\n";
            else
                cout << "No student found with ID " << id << "\n";
            break;
        }
        case 6: {
            int id = readInt("Enter student ID: ");
            string course = readLine("Course name: ");
            double grade = readDouble("Grade: ");
            if (manager.addGradeToStudent(id, course, grade))
                cout << "Grade recorded.\n";
            else
                cout << "No student found with ID " << id << "\n";
            break;
        }
        case 7: {
            int id = readInt("Enter ID to remove: ");
            if (manager.removeStudent(id))
                cout << "Student removed.\n";
            else
                cout << "No student found with ID " << id << "\n";
            break;
        }
        case 8: {
            manager.sortByName();
            cout << "Sorted by name.\n";
            break;
        }
        case 9: {
            manager.sortByAverageGrade(true);
            cout << "Sorted by average grade (descending).\n";
            break;
        }
        case 10: {
            manager.save();
            cout << "Saved to file.\n";
            break;
        }
        case 0: {
            running = false;
            cout << "Saving and exiting...\n";
            break;
        }
        default:
            cout << "Invalid option, try again.\n";
        }
    }

    return 0; 
}