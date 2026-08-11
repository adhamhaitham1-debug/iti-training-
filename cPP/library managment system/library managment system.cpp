

#include <iostream>
#include <vector>
#include <string>
#include <memory>

using namespace std;

class Borrowable {
public:
    virtual void borrowBook() = 0;
    virtual void returnBook() = 0;
    virtual bool checkAvailability() const = 0;
    virtual ~Borrowable() {}
};

class Book : public Borrowable {
private:
    string title;
    string author;
    int bookId;
    bool isAvailable;

public:
    Book(string title, string author, int bookId)
        : title(move(title)), author(move(author)), bookId(bookId), isAvailable(true) {
    }

    void displayBookInfo() const {
        cout << "  [" << bookId << "] \"" << title << "\" by " << author
            << " - " << (isAvailable ? "Available" : "Borrowed") << "\n";
    }

    void borrowBook() override {
        if (isAvailable) {
            isAvailable = false;
            cout << "You have borrowed \"" << title << "\".\n";
        }
        else {
            cout << "\"" << title << "\" is currently not available.\n";
        }
    }

    void returnBook() override {
        if (!isAvailable) {
            isAvailable = true;
            cout << "You have returned \"" << title << "\". Thank you!\n";
        }
        else {
            cout << "\"" << title << "\" was not borrowed.\n";
        }
    }

    bool checkAvailability() const override {
        return isAvailable;
    }

    int getId() const { return bookId; }
    string getTitle() const { return title; }
};

class User {
protected:
    string name;
    int userId;

public:
    User(string name, int userId) : name(move(name)), userId(userId) {}
    virtual void displayInfo() const = 0;
    int getId() const { return userId; }
    virtual ~User() {}
};

class Student : public User {
public:
    Student(string name, int userId) : User(move(name), userId) {}
    void displayInfo() const override {
        cout << "  [Student] ID: " << userId << ", Name: " << name << "\n";
    }
};

class Teacher : public User {
public:
    Teacher(string name, int userId) : User(move(name), userId) {}
    void displayInfo() const override {
        cout << "  [Teacher] ID: " << userId << ", Name: " << name << "\n";
    }
};

class Engineer : public User {
public:
    Engineer(string name, int userId) : User(move(name), userId) {}
    void displayInfo() const override {
        cout << "  [Engineer] ID: " << userId << ", Name: " << name << "\n";
    }
};

class Library {
private:
    vector<Book> books;
    vector<unique_ptr<User>> users;

public:
    void addBook(const Book& book) {
        books.push_back(book);
    }

    void addUser(unique_ptr<User> user) {
        users.push_back(move(user));
    }

    void displayBooks() const {
        cout << "\n--- Library Books ---\n";
        if (books.empty()) {
            cout << "  (no books in the library)\n";
            return;
        }
        for (const auto& book : books) {
            book.displayBookInfo();
        }
    }

    void displayUsers() const {
        cout << "\n--- Registered Users ---\n";
        if (users.empty()) {
            cout << "  (no users registered)\n";
            return;
        }
        for (const auto& user : users) {
            user->displayInfo();
        }
    }

    Book* findBookById(int bookId) {
        for (auto& book : books) {
            if (book.getId() == bookId) return &book;
        }
        return nullptr;
    }
};

static int readInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(1000, '\n'); 
            return value;
        }
        cout << "Invalid number, try again.\n";
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

static string readLine(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

static void printMenu() {
    cout << "\n==== Library Menu ====\n"
        << "1. Add Book\n"
        << "2. Add User\n"
        << "3. Display Books\n"
        << "4. Display Users\n"
        << "5. Borrow Book\n"
        << "6. Return Book\n"
        << "0. Exit\n"
        << "Choose an option: ";
}

int main() {
    Library library;

    library.addBook(Book("The C++ Programming Language", "Bjarne Stroustrup", 101));
    library.addBook(Book("Clean Code", "Robert C. Martin", 102));
    library.addUser(make_unique<Student>("Ahmed", 1));
    library.addUser(make_unique<Teacher>("Mona", 2));

    bool running = true;
    while (running) {
        printMenu();

        int choice;
        if (!(cin >> choice)) {
            cout << "Invalid input, please enter a number.\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        cin.ignore(1000, '\n'); 

        switch (choice) {
        case 1: {
            string title = readLine("Enter book title: ");
            string author = readLine("Enter author: ");
            int id = readInt("Enter book ID: ");
            library.addBook(Book(title, author, id));
            cout << "Book added.\n";
            break;
        }
        case 2: {
            string name = readLine("Enter user name: ");
            int id = readInt("Enter user ID: ");
            int type = readInt("User type (1=Student, 2=Teacher, 3=Engineer): ");

            if (type == 1) {
                library.addUser(make_unique<Student>(name, id));
            }
            else if (type == 2) {
                library.addUser(make_unique<Teacher>(name, id));
            }
            else if (type == 3) {
                library.addUser(make_unique<Engineer>(name, id));
            }
            else {
                cout << "Unknown user type, not added.\n";
                break;
            }
            cout << "User added.\n";
            break;
        }
        case 3:
            library.displayBooks();
            break;
        case 4:
            library.displayUsers();
            break;
        case 5: {
            int id = readInt("Enter book ID to borrow: ");
            if (Book* book = library.findBookById(id)) {
                book->borrowBook();
            }
            else {
                cout << "No book found with ID " << id << ".\n";
            }
            break;
        }
        case 6: {
            int id = readInt("Enter book ID to return: ");
            if (Book* book = library.findBookById(id)) {
                book->returnBook();
            }
            else {
                cout << "No book found with ID " << id << ".\n";
            }
            break;
        }
        case 0:
            cout << "Goodbye!\n";
            running = false;
            break;
        default:
            cout << "Invalid option, please try again.\n";
        }
    }

    return 0;
}