#if !defined(_VARSORT_HPP)
#define _VARSORT_HPP

#include "common.hpp"

typedef bool (*cmp_fn)(const int lhs, const int rhs);
typedef void (*sort_fn)(int arr[], const size_t len, cmp_fn);

void std_sort(int arr[], const size_t len, cmp_fn lt) {
    std::sort(arr, arr + len, lt);
}

void direct_insert_sort(int arr[], const size_t len, cmp_fn lt) {
    for (size_t i = 1; i < len; ++i) {
        if (lt(arr[i], arr[i - 1])) {
            const int ai = arr[i];
            ssize_t j = i - 1;
            for (; j >= 0; --j) {
                if (lt(ai, arr[j])) {
                    arr[j + 1] = arr[j];
                } else {
                    break;
                }
            }
            arr[j + 1] = ai;
        }
    }
}

void binary_insert_sort(int arr[], const size_t len, cmp_fn lt) {
    for (size_t i = 1; i < len; ++i) {
        if (lt(arr[i], arr[i - 1])) {
            const int ai = arr[i];
            size_t l = 0, r = i - 1;
            while (l < r) {
                const size_t mid = (l + r) / 2;
                if (!lt(ai, arr[mid])) { // arr[mid] <= ai
                    l = mid + 1;
                } else {
                    r = mid; // arr[mid] > ai
                }
            }
            memmove(&arr[l + 1], &arr[l], sizeof(int) * (i - l));
            arr[l] = ai;
        }
    }
}

void bubble_sort(int arr[], const size_t len, cmp_fn lt) {
    bool flag = true;
    for (size_t i = 0; i + 1 < len; ++i) {
        for (size_t j = len - 1; j > i; --j) {
            if (lt(arr[j], arr[j - 1])) {
                std::swap(arr[j], arr[j - 1]);
                flag = false;
            }
        }
        if (flag) {
            return;
        }
    }
}

void selection_sort(int arr[], const size_t len, cmp_fn lt) {
    for (size_t i = 0; i + 1 < len; ++i) {
        int mi = arr[i];
        size_t pos = i;
        for (size_t j = i + 1; j < len; ++j) {
            if (lt(arr[j], mi)) {
                mi = arr[j];
                pos = j;
            }
        }
        if (pos != i) {
            std::swap(arr[i], arr[pos]);
        }
    }
}

void shell_sort(int arr[], const size_t len, cmp_fn lt) {
    const size_t t = floor(log(double(len) + 1) / log(2));
    for (size_t k = 1; k <= t; ++k) {
        const size_t dk = pow(2, t - k + 1) - 1;
        for (size_t i = dk; i < len; ++i) {
            if (lt(arr[i], arr[i - dk])) {
                const auto x = arr[i];
                ssize_t j;
                for (j = i - dk; j >= 0; j -= dk) {
                    if (lt(x, arr[j])) {
                        arr[j + dk] = arr[j];
                    } else {
                        break;
                    }
                }
                arr[j + dk] = x;
            }
        }
    }
}

void _quick_sort_impl(int arr[], const size_t l, const size_t r, cmp_fn lt) {
    if (l == r || l + 1 == r) {
        return;
    }
    if (l + 2 == r) {
        if (lt(arr[r - 1], arr[l])) {
            std::swap(arr[l], arr[r - 1]);
        }
        return;
    }

    const size_t mid = (l + r) / 2;

    size_t _pos[3] = {l, mid, r - 1};

#define _bubb(bf, af)                                                          \
    do {                                                                       \
        if (lt(arr[_pos[af]], arr[_pos[bf]])) {                                \
            std::swap(_pos[af], _pos[bf]);                                     \
        }                                                                      \
    } while (0)

    _bubb(1, 2);
    _bubb(0, 1);
    _bubb(1, 2);

#undef _bubb

    std::swap(arr[l], arr[_pos[1]]);
    const auto pk = arr[l];

    size_t low = l, high = r - 1;
    while (low < high) {
        while (low < high && !lt(arr[high], pk)) {
            --high;
        }
        arr[low] = arr[high];
        while (low < high && !lt(pk, arr[low])) {
            ++low;
        }
        arr[high] = arr[low];
    }
    arr[low] = pk;
    const size_t pk_pos = low;

    _quick_sort_impl(arr, l, pk_pos, lt);
    _quick_sort_impl(arr, pk_pos + 1, r, lt);
}

void quick_sort(int arr[], const size_t len, cmp_fn lt) {
    _quick_sort_impl(arr, 0, len, lt);
}

void _heap_adjust(int h[], size_t s, const size_t e, cmp_fn lt) {
    const auto x = h[s];
    for (size_t j = s * 2 + 1; j <= e; j = j * 2 + 1) {
        if (j < e && lt(h[j], h[j + 1])) {
            ++j;
        }
        if (!lt(x, h[j])) {
            break;
        }
        h[s] = h[j];
        s = j;
    }
    h[s] = x;
}

void heap_sort(int arr[], const size_t len, cmp_fn lt) {
    for (ssize_t i = len / 2; i >= 0; --i) {
        _heap_adjust(arr, i, len - 1, lt);
    }
    for (size_t i = len; i > 1; --i) {
        std::swap(arr[0], arr[i - 1]);
        _heap_adjust(arr, 0, i - 2, lt);
    }
}

int _count_digits(int x) {
    if (x == 0) {
        return 1;
    }
    int c = 0;
    while (x) {
        ++c;
        x /= 10;
    }
    return c;
}

int _nth_digit(int x, int n) {
    while (n--) {
        x /= 10;
    }
    return x % 10;
}

void radix_sort(int arr[], const size_t len, cmp_fn lt) {
    if (len == 0 || len == 1) {
        return;
    }
    constexpr size_t INF = size_t(-1);

    size_t *next = new size_t[len];
    int max_digits = 0;
    for (size_t i = 0; i < len; ++i) {
        next[i] = i + 1;
        max_digits = std::max(max_digits, _count_digits(arr[i]));
    }
    next[len - 1] = INF;

    size_t f[10];
    size_t e[10];
    size_t h = 0;

    for (int round = 0; round < max_digits; ++round) {
        for (auto &p : f) {
            p = INF;
        }
        for (auto &p : e) {
            p = INF;
        }
        for (size_t p = h; p != INF; p = next[p]) {
            int j = _nth_digit(arr[p], round);

            if (f[j] == INF) {
                f[j] = p;
            } else {
                next[e[j]] = p;
            }
            e[j] = p;
        }
        for (int i = 0; i < 10; ++i) {
            if (e[i] != INF) {
                next[e[i]] = INF;
            }
        }

        h = INF;
        size_t t = INF;
        int i = 0;
        for (; i < 10; ++i) {
            if (f[i] != INF) {
                h = f[i];
                t = e[i];
                break;
            }
        }
        for (; i < 10; ++i) {
            if (f[i] != INF) {
                next[t] = f[i];
                t = e[i];
            }
        }
        next[t] = INF;
    }

    size_t *adr = next;
    size_t i = 0;
    size_t pr = INF;
    for (size_t p = h; p != INF; p = next[p]) {
        if (pr != INF) {
            adr[pr] = i;
            ++i;
        }
        pr = p;
    }
    adr[pr] = i;

    for (i = 0; i < len; ++i) {
        while (adr[i] != i) {
            size_t j = adr[i];
            std::swap(arr[i], arr[j]);
            std::swap(adr[i], adr[j]);
        }
    }

    delete next;
}

#define _SORT_METHOD(x)                                                        \
    { #x, x }

static const std::pair<const char *, sort_fn> sort_methods[] = {
    _SORT_METHOD(std_sort),           _SORT_METHOD(direct_insert_sort),
    _SORT_METHOD(binary_insert_sort), _SORT_METHOD(bubble_sort),
    _SORT_METHOD(selection_sort),     _SORT_METHOD(shell_sort),
    _SORT_METHOD(quick_sort),         _SORT_METHOD(heap_sort),
    _SORT_METHOD(radix_sort),
};

#undef _SORT_METHOD

#endif // _VARSORT_HPP
