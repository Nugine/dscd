#include "varsort.hpp"
using namespace std;
using std::chrono::duration;
using std::chrono::steady_clock;

void rand_fill(int arr[], const size_t n) {
    for (size_t i = 0; i < n; ++i) {
        arr[i] = rand();
    }
}

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

    srand(time(nullptr));

    for (size_t n = 500; n <= MAXN; n += 500) {
        rand_fill(arr, n);
        memcpy(ans, arr, sizeof(arr));
        sort(ans, ans + n);

        vector<Record> records;

        for (auto [name, fn] : sort_methods) {
            memcpy(buf, arr, sizeof(arr));
            cmp_count = 0;

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

        constexpr size_t W = 20;

        cout << "Round #" << (n / 500) << " ";
        cout << "n = " << n << endl;
        cout << left << setw(W) << "name";
        cout << left << setw(W) << "time";
        cout << left << setw(W) << "cmp count";
        cout << endl;
        for (const auto &rc : records) {
            cout << left << setw(W) << rc.name;
            cout << left << setw(W)
                 << (to_string(rc.nanoseconds / 1.0e6) + "ms");
            cout << left << setw(W) << rc.cmp_count;
            cout << endl;
        }
        cout << endl;
    }
}
