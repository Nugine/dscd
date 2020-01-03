#include "common.hpp"
using namespace std;

struct AddrItem {
    string name;   // 姓名
    string street; // 街道
    string city;   // 城市
    string eip;    // 邮编
    string state;  // 国家
};

struct AddrNode {
    AddrNode *prev;
    AddrNode *next;
    AddrItem *item;
};

struct AddrList {
    AddrNode *head;
    size_t len;
};

AddrList AL_new(void) {
    AddrList list;
    list.head = nullptr;
    list.len = 0;
    return list;
}

void AL_release(AddrList &list) {
    AddrNode *iter = list.head;
    AddrNode *node = nullptr;
    for (size_t i = 0; i < list.len; ++i) {
        if (node) {
            delete node->item;
            delete node;
        }
        node = iter;
        iter = iter->next;
    }
    if (node) {
        delete node->item;
        delete node;
    }
    list.len = 0;
    list.head = nullptr;
}

// return true if succeeded
// return false if item->name is not unique
const bool AL_sorted_insert(AddrList &list, AddrItem *const item) {
    AddrNode *node = new AddrNode{
        .prev = nullptr,
        .next = nullptr,
        .item = item,
    };

    if (list.len == 0) {
        node->prev = node;
        node->next = node;
        list.head = node;
        list.len = 1;
        return true;
    }

    AddrNode *iter = list.head;
    for (size_t i = 0; i < list.len; ++i) {
        if (iter->item->name == item->name) {
            delete node;
            return false;
        }
        if (iter->item->name > item->name) {
            break;
        }
        iter = iter->next;
    }

    node->prev = iter->prev;
    node->next = iter;
    node->prev->next = node;
    node->next->prev = node;
    list.len += 1;

    return true;
}

// assert: list is unique by item.name
// return nullptr if there is no such item
AddrItem *AL_search(const AddrList &list, const string &name) {
    AddrNode *iter = list.head;
    for (size_t i = 0; i < list.len; ++i) {
        if (iter->item->name == name) {
            return iter->item;
        }
        iter = iter->next;
    }
    return nullptr;
}

const bool AL_delete(AddrList &list, const string &name) {
    AddrNode *iter = list.head;
    for (size_t i = 0; i < list.len; ++i) {
        if (iter->item->name == name) {
            goto del;
        }
        iter = iter->next;
    }
    return false;
del:
    if (list.len == 1) {
        delete iter->item;
        delete iter;
        list.head = nullptr;
        list.len = 0;
    } else {
        iter->prev->next = iter->next;
        iter->next->prev = iter->prev;
        delete iter->item;
        delete iter;
        list.len -= 1;
    }
    return true;
}

const bool AL_save(const AddrList &list, const string &path) {
    fstream file(path, fstream::out | fstream::trunc);
    if (!file) {
        return false;
    }
    file << list.len << endl;

    AddrNode *iter = list.head;
    for (size_t i = 0; i < list.len; ++i) {
        const AddrItem &item = *iter->item;
        file << item.name << endl;
        file << item.street << endl;
        file << item.city << endl;
        file << item.eip << endl;
        file << item.state << endl;
        file << endl;
        if (!file) {
            return false;
        }
        iter = iter->next;
    }
    return true;
}

void AL_push(AddrList &list, AddrItem *const item) {
    AddrNode *node = new AddrNode;
    node->item = item;
    if (list.len == 0) {
        node->prev = node;
        node->next = node;
        list.head = node;
        list.len = 1;
    } else {
        node->next = list.head;
        node->prev = list.head->prev;
        node->next->prev = node;
        node->prev->next = node;
        list.len += 1;
    }
}

AddrItem *_load_item(istream &in) {
    AddrItem *item = new AddrItem;
    string s;

    getline(in, s);
    item->name = s;
    getline(in, s);
    item->street = s;
    getline(in, s);
    item->city = s;
    getline(in, s);
    item->eip = s;
    getline(in, s);
    item->state = s;
    getline(in, s);

    if (in) {
        return item;
    } else {
        delete item;
        return nullptr;
    }
}

const bool AL_load(const string &path, AddrList &rv_list) {
    fstream file(path, fstream::in);
    if (!file) {
        return false;
    }

    AL_release(rv_list);

    string s;
    size_t len;
    file >> len;
    getline(file, s);

    if (!len) {
        return true;
    }

    AddrItem *item = _load_item(file);
    if (!item) {
        return false;
    }
    AL_push(rv_list, item);
    for (size_t i = 1; i < len; ++i) {
        item = _load_item(file);
        if (!item) {
            return false;
        }
        AL_push(rv_list, item);
    }
    return true;
}

void UI_display_single_item(const AddrItem *const item) {
    cout << "The address:" << endl;
    cout << left << setw(8) << "name" << item->name << endl;
    cout << left << setw(8) << "street" << item->street << endl;
    cout << left << setw(8) << "city" << item->city << endl;
    cout << left << setw(8) << "eip" << item->eip << endl;
    cout << left << setw(8) << "state" << item->state << endl;
}

void UI_add(AddrList &list) {
    cout << "New address:" << endl;
    auto name = input_string("name: ");

    if (AL_search(list, name)) {
        cout << "the address already exists" << endl;
        return;
    }

    AddrItem *item = new AddrItem;
    item->name = std::move(name);
    item->street = input_string("street: ");
    item->city = input_string("city: ");
    item->eip = input_string("eip: ");
    item->state = input_string("state: ");

    UI_display_single_item(item);
    if (!AL_sorted_insert(list, item)) {
        delete item;
    }
}

void UI_display(const AddrList &list) {
    cout << left << setw(10) << "name";
    cout << left << setw(10) << "street";
    cout << left << setw(10) << "city";
    cout << left << setw(10) << "eip";
    cout << left << setw(10) << "state";
    cout << endl;

    AddrNode *iter = list.head;
    for (size_t i = 0; i < list.len; ++i) {
        const AddrItem &item = *iter->item;

        cout << left << setw(10) << item.name;
        cout << left << setw(10) << item.street;
        cout << left << setw(10) << item.city;
        cout << left << setw(10) << item.eip;
        cout << left << setw(10) << item.state;
        cout << endl;

        iter = iter->next;
    }
}

void UI_search(const AddrList &list) {
    cout << "Search address by name:" << endl;
    const string name = input_string("name: ");
    AddrItem *item = AL_search(list, name);
    if (item == nullptr) {
        cout << "No such address" << endl;
    } else {
        cout << endl;
        UI_display_single_item(item);
    }
}

void UI_delete(AddrList &list) {
    cout << "Delete address by name:" << endl;
    const string name = input_string("name: ");
    const bool ret = AL_delete(list, name);
    if (ret) {
        cout << "Deleted" << endl;
    } else {
        cout << "No such address" << endl;
    }
}

void UI_save(const AddrList &list) {
    cout << "Save address book to file:" << endl;
    const string path = input_string("file path: ");
    const bool ret = AL_save(list, path);
    if (ret) {
        cout << "Saved" << endl;
    } else {
        cout << "Failed" << endl;
    }
}

void UI_load(AddrList &rv_list) {
    cout << "Load address book from file:" << endl;
    const string path = input_string("file path: ");
    const bool ret = AL_load(path, rv_list);
    if (ret) {
        cout << "Loaded" << endl;
    } else {
        AL_release(rv_list);
        cout << "Failed" << endl;
    }
}

int main() {
    AddrList list = AL_new();

    string s;
    for (;;) {
        cout << "Address book system" << endl;
        cout << "1. Add a new address" << endl;
        cout << "2. Delete an address by name" << endl;
        cout << "3. Search an address by name" << endl;
        cout << "4. Display all addresses" << endl;
        cout << "5. Load address book from file" << endl;
        cout << "6. Save address book to file" << endl;
        cout << "7. Exit" << endl;

        const int no = input_number<int>("System call number: ", 1, 8);

        cout << endl;
        if (no == 1) {
            UI_add(list);
        } else if (no == 2) {
            UI_delete(list);
        } else if (no == 3) {
            UI_search(list);
        } else if (no == 4) {
            UI_display(list);
        } else if (no == 5) {
            UI_load(list);
        } else if (no == 6) {
            UI_save(list);
        } else if (no == 7) {
            break;
        }

        getline(cin, s);
        cout << endl;
    }
    cout << "Good bye" << endl;
}