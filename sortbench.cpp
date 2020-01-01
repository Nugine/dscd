#include "sortbench.hpp"
using namespace std;
using std::chrono::duration;
using std::chrono::steady_clock;

void rand_fill(int arr[], const size_t n) {}

static size_t cmp_count = 0;
bool cmp(const int lhs, const int rhs) {
    cmp_count += 1;
    return lhs < rhs;
}

struct Record {
    const char *name;
    unsigned long long nanoseconds;
    size_t cmp_count;
};

int main() {
    constexpr size_t MAXN = 30000;
    static int arr[MAXN + 7];
    static int buf[MAXN + 7];
    static int ans[MAXN + 7];

    cout << "sort algorithm benchmark" << endl;
    cout << "data source:" << endl;
    cout << "1. random" << endl;
    cout << "2. file" << endl;

    const int no = input_number<int>("source number: ", 1, 3);

    fstream file;
    random_device rnd;

    if (no == 1) {
        srand(time(nullptr));
    } else if (no == 2) {
        const string path = input_string("file path: ");
        file.open(path);
        if (!file) {
            cerr << "fatal error: can not open file \"" << path << "\"" << endl;
            exit(1);
        }
    }

    for (size_t n = 1000; n <= MAXN; n += 1000) {
        cout << "Round #" << (n / 500) << " ";
        cout << "n = " << n << endl;

        if (no == 1) {
            for (size_t i = 0; i < n; ++i) {
                arr[i] = static_cast<int>(rnd());
            }
        } else {
#ifdef DEBUG
            cout << "reading..." << endl;
#endif

            for (size_t i = 0; i < n; ++i) {
                file >> arr[i];
            }
            if (!file) {
                cerr << "fatal error: file format error" << endl;
                exit(1);
            }

#ifdef DEBUG
            cout << "loaded" << endl;
#endif
        }

        memcpy(ans, arr, sizeof(arr));
        sort(ans, ans + n);

        vector<Record> records;

        for (auto [name, fn] : sort_methods) {
            memcpy(buf, arr, sizeof(arr));
            cmp_count = 0;

#ifdef DEBUG
            cout << "run " << name << endl;
#endif
            const auto start = steady_clock::now();
            fn(buf, n, cmp);
            const auto end = steady_clock::now();

            auto d = end - start;
            unsigned long long t = d.count();

            records.push_back({name, t, cmp_count});

            if (memcmp(buf, ans, sizeof(int) * n) != 0) {
                cerr << "incorrect algorithm: " << name << endl;
                exit(1);
            }
        }

#ifdef DEBUG
        cout << "completed" << endl;
#endif

        std::sort(records.begin(), records.end(),
                  [](const Record &lhs, const Record &rhs) {
                      return lhs.nanoseconds < rhs.nanoseconds;
                  });

        cout << left << setw(20) << "name";
        cout << left << setw(14) << "     time";
        cout << right << setw(14) << "cmp count";
        cout << endl;
        for (const auto &rc : records) {
            cout << left << setw(20) << rc.name;
            cout << right << setw(11) << to_string(rc.nanoseconds / 1.0e6)
                 << " ms";
            cout << right << setw(14) << rc.cmp_count;
            cout << endl;
        }
        cout << endl;
    }
}
