#include "common.hpp"
using namespace std;

typedef string ISBN;

constexpr size_t HASHSIZE = 10007;

const size_t isbn_hash(const ISBN &isbn) {
    int state = 0;
    size_t s = 0;
    size_t t = 0;
    for (auto c : isbn) {
        if (isdigit(c)) {
            state += 1;
            if (state == 5) {
                t = 0;
                s += t;
                state = 1;
            }
            t = t * 10 + (c - '0');
        }
    }
    s += t;
    return s % HASHSIZE;
}

struct Book {
    string isbn;
    string name;
    string author;
    size_t total;
    size_t available;
};

class HashTable {
  private:
    struct HashNode {
        ISBN key;
        Book *val;

        ~HashNode() { delete val; }
    };

    HashNode **buf;
    std::size_t len;
    std::size_t cap;

  public:
    HashTable(void) {
        len = 0;
        cap = HASHSIZE;
        buf = new HashNode *[cap];
        memset(buf, 0, cap * sizeof(HashNode **));
    }
    ~HashTable() {
        len = 0;
        for (size_t i = 0; i < cap; ++i) {
            delete buf[i];
        }
        delete buf;
    }

    size_t _search_empty(const ISBN &isbn, size_t &rv_coll) const {
        const size_t h0 = isbn_hash(isbn) % cap;
        if (!buf[h0]) {
            return h0;
        }
        rv_coll = 1;
        for (size_t d = 1; d < cap; ++d) {
            const size_t h = (h0 + d) % cap;
            if (!buf[h]) {
                return h;
            }
            rv_coll += 1;
        }
        return cap;
    }
    size_t _search(const ISBN &isbn) const {
        const size_t h0 = isbn_hash(isbn) % cap;
        if (!buf[h0]) {
            return cap;
        }
        if (buf[h0]->key == isbn) {
            return h0;
        }
        for (size_t d = 1; d < cap; ++d) {
            const size_t h = (h0 + d) % cap;
            if (!buf[h]) {
                return cap;
            }
            if (buf[h]->key == isbn) {
                return h;
            }
        }
        return cap;
    }

    void insert(const ISBN isbn, Book *book) {
        size_t coll;
        const size_t idx = _search_empty(isbn, coll);
        assert(idx < cap);
        HashNode *node = new HashNode{.key = std::move(isbn), .val = book};
        buf[idx] = node;
    }

    Book *search(const ISBN &isbn) const {
        const size_t idx = _search(isbn);
        if (idx == cap) {
            return nullptr;
        }
        return buf[idx]->val;
    }
};

struct BorrowRecord {
    string token;
    string date;
    string isbn;
};

struct Library {
    HashTable table;
    vector<BorrowRecord *> records;
};

void UI_add_book(Library &lib) {
    cout << "Add book" << endl;

    string isbn = input_string("isbn: ");
    Book *book = lib.table.search(isbn);
    if (book) {
        cout << "the book already exists" << endl;
        const size_t amt = input_number<size_t>("the amount of books: ");
        book->total += amt;
        return;
    }

    book = new Book;
    book->isbn = std::move(isbn);
    book->name = input_string("name: ");
    book->author = input_string("author: ");
    book->total = input_number<size_t>("total: ");
    book->available = book->total;
    lib.table.insert(book->isbn, book);
}

void UI_borrow(Library &lib) {
    cout << "Borrow book" << endl;
    string isbn = input_string("isbn: ");

    Book *book = lib.table.search(isbn);
    if (!book) {
        cout << "No such book" << endl;
        return;
    }
    if (book->available == 0) {
        cout << "There are no available books: " << isbn << endl;
        return;
    }

    BorrowRecord *record = new BorrowRecord;
    record->isbn = std::move(isbn);
    record->token = input_string("token: ");
    record->date = input_string("date: ");

    book->available -= 1;
    lib.records.push_back(record);

    cout << "Borrowed" << endl;
}

void UI_return(Library &lib) {
    cout << "Return book" << endl;
    string isbn = input_string("isbn: ");

    Book *book = lib.table.search(isbn);
    if (!book) {
        cout << "No such book" << endl;
        return;
    }

    string token = input_string("token: ");

    for (auto i = lib.records.begin(); i != lib.records.end(); ++i) {
        const BorrowRecord &record = **i;
        if (record.isbn == isbn && record.token == token) {
            delete *i;
            lib.records.erase(i);
            book->available += 1;
            cout << "Returned" << endl;
            return;
        }
    }
    cout << "No such record" << endl;
}

void UI_search(const Library &lib) {
    cout << "Search book" << endl;
    string isbn = input_string("isbn: ");

    const Book *book = lib.table.search(isbn);
    if (!book) {
        cout << "No such book" << endl;
    }

    cout << "The book: " << endl;
    cout << left << setw(10) << "isbn" << book->isbn << endl;
    cout << left << setw(10) << "name" << book->name << endl;
    cout << left << setw(10) << "author" << book->author << endl;
    cout << left << setw(10) << "available" << book->available << endl;
    cout << left << setw(10) << "total" << book->total << endl;
}

int main() {
    cout << "Library system" << endl;

    Library lib;

    string s;
    for (;;) {
        cout << "1. Add book" << endl;
        cout << "2. Borrow book" << endl;
        cout << "3. Return book" << endl;
        cout << "4. Search book" << endl;
        cout << "5. Exit" << endl;

        const int no = input_number<int>("System call number: ", 1, 6);

        cout << endl;
        if (no == 1) {
            UI_add_book(lib);
        } else if (no == 2) {
            UI_borrow(lib);
        } else if (no == 3) {
            UI_return(lib);
        } else if (no == 4) {
            UI_search(lib);
        } else if (no == 5) {
            break;
        }

        getline(cin, s);
        cout << endl;
    }
    cout << "Good bye" << endl;
}