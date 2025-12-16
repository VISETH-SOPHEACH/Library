#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

string password = "M3";

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

class Book
{
public:
  int id;
  string title,  author;
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

class Member
{
public:
  string id, name;
  Member() : id(0), name("") {}
  Member(string i, string n) : id(i), name(n) {}
  void display() const
  {
    cout << setw(13) << id
         << setw(13) << name << endl;
  }
};

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
      Member m(line.substr(0, pos), line.substr(pos + 1));
      members.add(m);
    }
  }

  void deleteBook(int id)
  {
    for (auto it = books.items.begin(); it != books.items.end(); ++it)
    {
      if (it->id == id)
      {
        books.items.erase(it);
        saveBooks();
        cout << "Book deleted successfully!\n";
        return;
      }
    }
    cout << "Book not found!\n";
  }

  void deleteMember(const string &id)
  {
    for (auto it = members.items.begin(); it != members.items.end(); ++it)
    {
      if (it->id == id)
      {
        members.items.erase(it);
        saveMembers();
        cout << "Member deleted successfully!\n";
        return;
      }
    }
    cout << "Member not found!\n";
  }
  bool isMemberExists(const string &id)
  {
    for (const auto &m : members.items)
    {
      if (m.id == id)
        return true;
    }
    return false;
  }
};

int main()
{
  Library lib;
  lib.loadBooks();
  lib.loadMembers();

  int choice;
  do
  {
    cout << "\n===== LIBRARY MANAGEMENT =====\n";
    cout << "1. Add Book\n2. Show All Books\n3. Add Member\n4. Show Members\n5. Show book and member\n6. Delete Book\n7. Delete Member\n0. Exit\nChoose: ";
    cin >> choice;
    cin.ignore();
    if (choice == 1)
    {
      string member_id;
      int id;
      string title, author;

      cout << "Enter Member ID: ";
      cin >> member_id;

      if (!lib.isMemberExists(member_id))
      {
        cout << "Access denied! Member not found.\n";
        continue;
      }

      cout << "Book ID: ";
      cin >> id;
      cin.ignore();

      cout << "Title: ";
      getline(cin, title);

      cout << "Author: ";
      getline(cin, author);

      lib.books.add(Book(id, title, author));
      lib.saveBooks();

      cout << "Book added successfully by member " << member_id << "!\n";
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
      string id, name;
      cout << "Enter Password : ";
      cin >> password;
      if (password != "M3")
      {
        break;
      }
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
    else if (choice == 6)
    {
      int id;
      cout << "Enter Book ID to delete: ";
      cin >> id;
      lib.deleteBook(id);
    }
    else if (choice == 7)
    {
      string id;
      cin.ignore();
      cout << "Enter Member ID to delete: ";
      getline(cin, id);
      lib.deleteMember(id);
    }

  } while (choice != 0);

  cout << "Program exited.\n";
  return 0;
}