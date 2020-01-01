#include <iostream>
#include <random>
using namespace std;
int main() {
    random_device rnd;
    for (int n = 1000; n <= 30000; n += 1000) {
        for (int i = 0; i < n; ++i) {
            cout << static_cast<int>(rnd()) << ' ';
        }
        cout << endl;
    }
    return 0;
}
