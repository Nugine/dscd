#if !defined(_VARSORT_HPP)
#define _VARSORT_HPP

#include "common.hpp"

typedef bool (*cmp_fn)(const int lhs, const int rhs);
typedef void (*sort_fn)(int arr[], const size_t len, cmp_fn);

void std_sort(int arr[], const size_t len, cmp_fn cmp);
void direct_insert_sort(int arr[], const size_t len, cmp_fn cmp);
void binary_insert_sort(int arr[], const size_t len, cmp_fn cmp);

static const std::pair<const char *, sort_fn> sort_methods[] = {
    {"std_sort", std_sort},
    {"direct_insert_sort", direct_insert_sort},
    {"binary_insert_sort", binary_insert_sort},
};

void std_sort(int arr[], const size_t len, cmp_fn cmp) {
    std::sort(arr, arr + len, cmp);
}

void direct_insert_sort(int arr[], const size_t len, cmp_fn cmp) {
    for (size_t i = 1; i < len; ++i) {
        if (cmp(arr[i], arr[i - 1])) {
            const int ai = arr[i];
            ssize_t j = i - 1;
            for (; j >= 0; --j) {
                if (cmp(ai, arr[j])) {
                    arr[j + 1] = arr[j];
                } else {
                    break;
                }
            }
            arr[j + 1] = ai;
        }
    }
}

void binary_insert_sort(int arr[], const size_t len, cmp_fn cmp) {
    for (size_t i = 1; i < len; ++i) {
        if (cmp(arr[i], arr[i - 1])) {
            const int ai = arr[i];
            size_t l = 0, r = i - 1;
            while (l < r) {
                const size_t mid = (l + r) / 2;
                if (!cmp(ai, arr[mid])) { // arr[mid] <= ai
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

#endif // _VARSORT_HPP
