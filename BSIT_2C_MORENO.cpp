#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class Book {
private:
    string title;
    string author;
    string ISBN;
    bool available;

public:
    Book(string t = "", string a = "", string i = "", bool avail = true)
        : title(t), author(a), ISBN(i), available(avail) {}

    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getISBN() const { return ISBN; }
    bool isAvailable() const { return available; }

    void setAvailability(bool status) { available = status; }
};

class LibraryUser {
private:
    string userID;
    string name;
    vector<string> borrowedBooks;

public:
    LibraryUser(string id = "", string n = "") : userID(id), name(n) {}

    string getUserID() const { return userID; }
    string getName() const { return name; }

    void borrowBook(string isbn) {
        if (find(borrowedBooks.begin(), borrowedBooks.end(), isbn) != borrowedBooks.end()) {
            cout << "Error: Book already borrowed by this user.\n";
            return;
        }
        borrowedBooks.push_back(isbn);
    }

    void returnBook(string isbn) {
        auto it = find(borrowedBooks.begin(), borrowedBooks.end(), isbn);
        if (it == borrowedBooks.end()) {
            cout << "Error: This book was not borrowed by this user.\n";
            return;
        }
        borrowedBooks.erase(it);
    }

    void displayBorrowedBooks() const {
        cout << "Borrowed books of " << name << " (UserID: " << userID << "):\n";
        if (borrowedBooks.empty()) cout << "  None\n";
        else for (const auto& isbn : borrowedBooks) cout << "  ISBN: " << isbn << endl;
    }
};

class Library {
private:
    vector<Book> books;
    vector<LibraryUser> users;

public:
    Library() {
        // Preload some books
        books.push_back(Book("The Great Gatsby", "F. Scott Fitzgerald", "9780743273565"));
        books.push_back(Book("Pride and Prejudice", "Jane Austen", "9781503290563"));
        books.push_back(Book("The Catcher in the Rye", "J.D. Salinger", "9780316769488"));
    }

    void addBook(string t, string a, string i) {
        books.push_back(Book(t, a, i));
        cout << "Book added successfully!\n";
    }

    void removeBook(string isbn) {
        auto oldSize = books.size();
        books.erase(remove_if(books.begin(), books.end(), [&](Book& b){ return b.getISBN() == isbn; }), books.end());
        if (books.size() == oldSize) cout << "Error: Book not found.\n";
        else cout << "Book removed successfully!\n";
    }

    void registerUser(string id, string n) {
        users.push_back(LibraryUser(id, n));
        cout << "User registered successfully!\n";
    }

    void removeUser(string id) {
        auto oldSize = users.size();
        users.erase(remove_if(users.begin(), users.end(), [&](LibraryUser& u){ return u.getUserID() == id; }), users.end());
        if (users.size() == oldSize) cout << "Error: User not found.\n";
        else cout << "User removed successfully!\n";
    }

    void displayAllBooks() const {
        cout << "\nBooks in Library:\n";
        for (auto& b : books)
            cout << b.getTitle() << " by " << b.getAuthor() << " (ISBN: " << b.getISBN() << ") - "
                 << (b.isAvailable() ? "Available" : "Borrowed") << "\n";
    }

    void displayAllUsers() const {
        cout << "\nLibrary Users:\n";
        for (auto& u : users)
            cout << "UserID: " << u.getUserID() << " - " << u.getName() << "\n";
    }

    void displayBorrowedBooks(string userID) {
        for (auto& u : users) if (u.getUserID() == userID) { u.displayBorrowedBooks(); return; }
        cout << "Error: User not found.\n";
    }

    void borrowBook(string isbn, string userID) {
        auto bIt = find_if(books.begin(), books.end(), [&](Book& b){ return b.getISBN() == isbn; });
        if (bIt == books.end()) { cout << "Error: Book not found.\n"; return; }
        if (!bIt->isAvailable()) { cout << "Error: Book is already borrowed.\n"; return; }

        auto uIt = find_if(users.begin(), users.end(), [&](LibraryUser& u){ return u.getUserID() == userID; });
        if (uIt == users.end()) { cout << "Error: User not found.\n"; return; }

        bIt->setAvailability(false);
        uIt->borrowBook(isbn);
        cout << uIt->getName() << " borrowed " << bIt->getTitle() << endl;
    }

    void returnBook(string isbn, string userID) {
        auto bIt = find_if(books.begin(), books.end(), [&](Book& b){ return b.getISBN() == isbn; });
        if (bIt == books.end()) { cout << "Error: Book not found.\n"; return; }

        auto uIt = find_if(users.begin(), users.end(), [&](LibraryUser& u){ return u.getUserID() == userID; });
        if (uIt == users.end()) { cout << "Error: User not found.\n"; return; }

        uIt->returnBook(isbn);
        bIt->setAvailability(true);
        cout << uIt->getName() << " returned " << bIt->getTitle() << endl;
    }
};

int main() {
    Library library;
    int choice;
    do {
        cout << "\n=== Library Management System ===\n";
        cout << "1. Add Book\n2. Remove Book\n3. Register User\n4. Remove User\n";
        cout << "5. Display All Books\n6. Display All Users\n7. Borrow Book\n8. Return Book\n9. Display Borrowed Books\n0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        string title, author, isbn, id, name;
        switch (choice) {
            case 1:
                cout << "Enter title: "; getline(cin, title);
                cout << "Enter author: "; getline(cin, author);
                cout << "Enter ISBN: "; getline(cin, isbn);
                library.addBook(title, author, isbn);
                break;
            case 2:
                cout << "Enter ISBN to remove: "; getline(cin, isbn);
                library.removeBook(isbn);
                break;
            case 3:
                cout << "Enter User ID: "; getline(cin, id);
                cout << "Enter Name: "; getline(cin, name);
                library.registerUser(id, name);
                break;
            case 4:
                cout << "Enter User ID to remove: "; getline(cin, id);
                library.removeUser(id);
                break;
            case 5:
                library.displayAllBooks();
                break;
            case 6:
                library.displayAllUsers();
                break;
            case 7:
                cout << "Enter ISBN to borrow: "; getline(cin, isbn);
                cout << "Enter User ID: "; getline(cin, id);
                library.borrowBook(isbn, id);
                break;
            case 8:
                cout << "Enter ISBN to return: "; getline(cin, isbn);
                cout << "Enter User ID: "; getline(cin, id);
                library.returnBook(isbn, id);
                break;
            case 9:
                cout << "Enter User ID: "; getline(cin, id);
                library.displayBorrowedBooks(id);
                break;
            case 0:
                cout << "Exiting... Goodbye!\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 0);
    return 0;
}
