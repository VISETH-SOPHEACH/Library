#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

template <typename T>
class Manager
{
public:
  vector<T> items;
  void add(const T &item) { items.push_back(item); }
  void showAll() const
  {
    for (const auto &item : items)
      item.display();
  }
};

/* ================= BOOK CLASS ================= */
/* ================= BOOK CLASS ================= */
class Book
{
public:
  int id;
  string title;
  string author;
  bool isBorrowed;
  Book() : id(0), title(""), author(""), isBorrowed(false) {}
  Book(int i, string t, string a) : id(i), title(t), author(a), isBorrowed(false) {}
  void display() const
  {
    cout << setw(13) << id
         << setw(13) << title
         << setw(13) << author
         << setw(13) << (isBorrowed ? "Borrowed" : "Available") << endl;
  }
};

/* ================= MEMBER CLASS ================= */
class Member
{
public:
  int id;
  string name;
  Member() : id(0), name("") {}
  Member(int i, string n) : id(i), name(n) {}
  void display() const
  {
    cout << setw(13) << id
         << setw(13) << name << endl;
  }
};

/* ================= LIBRARY CLASS ================= */
class Library
{
public:
  Manager<Book> books;
  Manager<Member> members;

  void saveBooks()
  {
    ofstream file("books.txt");
    for (auto &b : books.items)
      file << b.id << "," << b.title << "," << b.author << "," << b.isBorrowed << endl;
  }

  void loadBooks()
  {
    ifstream file("books.txt");
    if (!file)
      return;
    string line;
    while (getline(file, line))
    {
      if (line.empty())
        continue;
      Book b;
      size_t pos = 0, prev = 0;
      int part = 0;
      while ((pos = line.find(',', prev)) != string::npos)
      {
        string token = line.substr(prev, pos - prev);
        if (part == 0)
          b.id = stoi(token);
        else if (part == 1)
          b.title = token;
        else if (part == 2)
          b.author = token;
        prev = pos + 1;
        part++;
      }
      b.isBorrowed = stoi(line.substr(prev));
      books.add(b);
    }
  }

  void saveMembers()
  {
    ofstream file("members.txt");
    for (auto &m : members.items)
      file << m.id << "," << m.name << endl;
  }

  void loadMembers()
  {
    ifstream file("members.txt");
    if (!file)
      return;
    string line;
    while (getline(file, line))
    {
      if (line.empty())
        continue;
      size_t pos = line.find(',');
      Member m(stoi(line.substr(0, pos)), line.substr(pos + 1));
      members.add(m);
    }
  }
};

/* ================= MAIN ================= */
int main()
{
  Library lib;
  lib.loadBooks();
  lib.loadMembers();

  int choice;
  do
  {
    cout << "\n===== LIBRARY MANAGEMENT =====\n";
    cout << "1. Add Book\n2. Show All Books\n3. Add Member\n4. Show Members\n5. Show book and member\n0. Exit\nChoose: ";
    cin >> choice;
    cin.ignore(); // clear newline before getline
    if (choice == 1)
    {
      int id;
      string title, author;
      cout << "Book ID: ";
      cin >> id;
      cin.ignore();
      cout << "Title: ";
      getline(cin, title);
      cout << "Author: ";
      getline(cin, author);
      lib.books.add(Book(id, title, author));
      lib.saveBooks();
      cout << "Book added successfully!\n";
    }
    else if (choice == 2)
    {
      cout << endl
           << left << setw(13) << left << "Book ID"
           << setw(13) << "Book Title"
           << setw(13) << "Book Author"
           << setw(13) << "Book Status" << endl;
      lib.books.showAll();
    }
    else if (choice == 3)
    {
      int id;
      string name;
      cout << "Member ID: ";
      cin >> id;
      cin.ignore();
      cout << "Name: ";
      getline(cin, name);
      lib.members.add(Member(id, name));
      lib.saveMembers();
      cout << "Member added successfully!\n";
    }
    else if (choice == 4)
    {
      cout << endl
           << left << setw(13) << "Member ID"
           << setw(13) << "Member Name" << endl;
      lib.members.showAll();
    }
    else if (choice == 5)
    {
      cout << endl
           << left << setw(13) << "Member ID"
           << setw(13) << "Member Name" << endl;
      lib.members.showAll();
      cout << endl
           << left << setw(13) << left << "Book ID"
           << setw(13) << "Book Title"
           << setw(13) << "Book Author"
           << setw(13) << "Book Status" << endl;
      lib.books.showAll();
    }
  } while (choice != 0);

  cout << "Program exited.\n";
  return 0;
}
