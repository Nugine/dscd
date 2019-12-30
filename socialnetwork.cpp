#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
using namespace std;

#include "common.hpp"

struct Position {
    double x, y;
};

const double dist(const Position lhs, const Position rhs) {
    const double dx = rhs.x - lhs.x;
    const double dy = rhs.y - lhs.y;
    return sqrt(dx * dx + dy * dy);
}

struct PersonNode {
    size_t idx;
    string name;
    Position pos;

    vector<size_t> relations;
};

struct Network {
    vector<PersonNode *> people;
    vector<pair<size_t, size_t>> relations;
};

const bool Nw_load(Network &nw, const string &path) {
    fstream file(path, fstream::in);
    if (!file) {
        return false;
    }
    nw.people.clear();
    nw.relations.clear();

    size_t n = 0;
    file >> n;

    string s;
    getline(file, s);

    double x, y;
    for (size_t i = 0; i < n; ++i) {
        getline(file, s);
        file >> x >> y;
        PersonNode *node = new PersonNode;
        node->idx = i;
        node->name = s;
        node->pos = {x, y};
        nw.people.push_back(node);
        getline(file, s);
    }

    size_t m = 0;
    file >> m;
    for (size_t i = 0; i < m; ++i) {
        size_t u, v;
        file >> u >> v;
        nw.relations.push_back({u, v});
        nw.people[u]->relations.push_back(v);
        nw.people[v]->relations.push_back(u);
    }

    if (!file) {
        return false;
    }

    return true;
}

vector<size_t> Nw_find_path(const Network &nw, const size_t u, const size_t v) {
    const size_t n = nw.people.size();
    vector<bool> vis(n);
    vector<size_t> prev(n, -1);
    vector<size_t> path;

    queue<size_t> q;
    q.push(u);
    while (!q.empty()) {
        size_t cur = q.front();
        q.pop();
        vis[cur] = true;
        if (cur == v) {
            for (; cur != -1; cur = prev[cur]) {
                path.push_back(cur);
            }
            break;
        }
        for (auto p : nw.people[cur]->relations) {
            if (!vis[p]) {
                prev[p] = cur;
                q.push(p);
            }
        }
    }
    reverse(path.begin(), path.end());
    return path;
}

vector<size_t> Nw_find_r2(const Network &nw, const size_t u) {
    queue<size_t> q;
    for (auto p : nw.people[u]->relations) {
        q.push(p);
    }
    vector<size_t> r2;
    while (!q.empty()) {
        const size_t cur = q.front();
        q.pop();
        for (auto p : nw.people[cur]->relations) {
            if (p != u) {
                r2.push_back(p);
            }
        }
    }
    return r2;
}

vector<size_t> Nw_find_around(const Network &nw, const size_t u,
                              const double rad) {
    const Position center = nw.people[u]->pos;
    vector<size_t> v;
    for (auto p : nw.people) {
        if (p->idx == u) {
            continue;
        }
        if (dist(center, p->pos) <= rad) {
            v.push_back(p->idx);
        }
    }
    return v;
}

void UI_load(Network &nw) {
    const string path = input_string("social network file path: ");
    const bool ret = Nw_load(nw, path);
    if (ret) {
        cout << "Loaded" << endl;
        cout << "The network has " << nw.people.size() << " people" << endl;
    } else {
        cout << "Failed" << endl;
    }
}

void _show_list(const Network &nw, const vector<size_t> &list) {

    const size_t len = list.size();
    for (size_t i = 0; i < len; ++i) {
        cout << "#" << left << setw(2) << (i + 1) << ": ";
        cout << "[" << left << setw(2) << list[i] << "] ";
        cout << nw.people[list[i]]->name << endl;
    }
}

void UI_show_path(const Network &nw) {
    const size_t n = nw.people.size();
    const size_t start = input_number<size_t>("start person index: ", 0, n);
    const size_t end = input_number<size_t>("end person index: ", 0, n);
    const auto path = Nw_find_path(nw, start, end);
    if (path.empty()) {
        cout << "No such path" << endl;
    } else {
        _show_list(nw, path);
    }
}

void UI_show_r2(const Network &nw) {
    const size_t n = nw.people.size();
    const size_t u = input_number<size_t>("the person index: ", 0, n);
    const auto relations = Nw_find_r2(nw, u);
    if (relations.empty()) {
        cout << "No indirect relations" << endl;
    } else {
        _show_list(nw, relations);
    }
}

void UI_show_around(const Network &nw) {
    const size_t n = nw.people.size();
    const size_t u = input_number<size_t>("the person index: ", 0, n);
    const double radius = input_number<double>("radius: ", 0, 0xffffffff);
    const auto around = Nw_find_around(nw, u, radius);
    if (around.empty()) {
        cout << "No people around" << endl;
    } else {
        _show_list(nw, around);
    }
}

int main() {
    Network nw;

    cout << "Social network system" << endl;

    string s;
    for (;;) {
        cout << "1. Load network from file" << endl;
        cout << "2. Show path between two people" << endl;
        cout << "3. Show indirect relations of one person" << endl;
        cout << "4. Show people around one person" << endl;
        cout << "5. Exit" << endl;

        const int no = input_number<int>("System call number: ", 1, 6);

        cout << endl;
        if (no == 1) {
            UI_load(nw);
        } else if (no == 2) {
            UI_show_path(nw);
        } else if (no == 3) {
            UI_show_r2(nw);
        } else if (no == 4) {
            UI_show_around(nw);
        } else if (no == 5) {
            break;
        }

        getline(cin, s);
        cout << endl;
    }
    cout << "Good bye" << endl;
}