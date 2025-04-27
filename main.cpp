#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <ctime>
#include <limits>
#include <cstring>    
#include <algorithm>  

using namespace std;

class Book {
public:
    int id;
    string title;
    string author;
    bool isAvailable;
    time_t dueDate;

    void displayBookDetails() const {
        cout << "Book ID: " << id << "\nTitle: " << title << "\nAuthor: " << author << "\n";
        cout << "Status: " << (isAvailable ? "Available" : "Checked Out") << "\n";
        if (!isAvailable) {
            char buffer[80];
            strncpy(buffer, ctime(&dueDate), sizeof(buffer));
            buffer[sizeof(buffer) - 1] = '\0'; 
            cout << "Due Date: " << buffer;
        }
        cout << "---------------------------\n";
    }
};

class Patron {
public:
    int id;
    string name;
    vector<int> borrowedBooks;
};

class LibraryManagementSystem {
private:
    map<int, Book> books;
    map<int, Patron> patrons;

public:
    void addBook() {
        Book book;
        cout << "Enter Book ID: ";
        cin >> book.id;
        cin.ignore();
        cout << "Enter Title: ";
        getline(cin, book.title);
        cout << "Enter Author: ";
        getline(cin, book.author);
        book.isAvailable = true;
        books[book.id] = book;
        cout << "Book added successfully.\n";
    }

    void addPatron() {
        Patron patron;
        cout << "Enter Patron ID: ";
        cin >> patron.id;
        cin.ignore();
        cout << "Enter Name: ";
        getline(cin, patron.name);
        patrons[patron.id] = patron;
        cout << "Patron added successfully.\n";
    }

    void displayAllBooks() const {
        for (const auto& pair : books) {
            pair.second.displayBookDetails();
        }
    }

    void searchBook() const {
        int bookId = getNumericInput("Enter Book ID to search: ");
        auto it = books.find(bookId);
        if (it != books.end()) {
            it->second.displayBookDetails();
        } else {
            cout << "Book not found.\n";
        }
    }

    void checkoutBook() {
        int patronId = getNumericInput("Enter Patron ID: ");
        int bookId = getNumericInput("Enter Book ID to checkout: ");
        auto bookIt = books.find(bookId);
        auto patronIt = patrons.find(patronId);
        if (bookIt != books.end() && patronIt != patrons.end()) {
            if (bookIt->second.isAvailable) {
                bookIt->second.isAvailable = false;
                time(&bookIt->second.dueDate); 
                patronIt->second.borrowedBooks.push_back(bookId);
                char buffer[80];
                strncpy(buffer, ctime(&bookIt->second.dueDate), sizeof(buffer));
                buffer[sizeof(buffer) - 1] = '\0'; 
                cout << "Book checked out successfully. Due date: " << buffer;
            } else {
                cout << "Book is already checked out.\n";
            }
        } else {
            cout << "Invalid Patron ID or Book ID.\n";
        }
    }

    void checkinBook() {
        int patronId = getNumericInput("Enter Patron ID: ");
        int bookId = getNumericInput("Enter Book ID to return: ");
        auto bookIt = books.find(bookId);
        auto patronIt = patrons.find(patronId);
        if (bookIt != books.end() && patronIt != patrons.end()) {
            auto& booksVec = patronIt->second.borrowedBooks;
            auto it = find(booksVec.begin(), booksVec.end(), bookId);
            if (it != booksVec.end()) {
                bookIt->second.isAvailable = true;
                booksVec.erase(it);
                cout << "Book returned successfully.\n";
            } else {
                cout << "This patron did not borrow this book.\n";
            }
        } else {
            cout << "Invalid Patron ID or Book ID.\n";
        }
    }

    void viewPatronBooks() const {
        int patronId = getNumericInput("Enter Patron ID: ");
        auto patronIt = patrons.find(patronId);
        if (patronIt != patrons.end()) {
            cout << "Books borrowed by " << patronIt->second.name << ":\n";
            for (int bookId : patronIt->second.borrowedBooks) {
                auto bookIt = books.find(bookId);
                if (bookIt != books.end()) {
                    cout << "- " << bookIt->second.title << "\n";
                }
            }
        } else {
            cout << "Patron not found.\n";
        }
    }

    int getNumericInput(const string& prompt) const {
        int num;
        while (true) {
            cout << prompt;
            cin >> num;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a number.\n";
            } else {
                return num;
            }
        }
    }
};

int main() {
    LibraryManagementSystem library;
    int choice;

    do {
        cout << "\n----- Library Management System -----\n";
        cout << "1. Add Book\n";
        cout << "2. Add Patron\n";
        cout << "3. Display All Books\n";
        cout << "4. Search Book\n";
        cout << "5. Checkout Book\n";
        cout << "6. Return Book\n";
        cout << "7. View Patron's Borrowed Books\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: library.addBook(); break;
            case 2: library.addPatron(); break;
            case 3: library.displayAllBooks(); break;
            case 4: library.searchBook(); break;
            case 5: library.checkoutBook(); break;
            case 6: library.checkinBook(); break;
            case 7: library.viewPatronBooks(); break;
            case 8 : cout << "Exiting the Library Management System. Thank you!\n"; break;
            default: cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 11);

    return 0;
}
