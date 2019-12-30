#if !defined(_VARSORT_HPP)
#define _VARSORT_HPP

#include "common.hpp"

typedef bool (*cmp_fn)(const int lhs, const int rhs);
typedef void (*sort_fn)(int arr[], const size_t len, cmp_fn);

void std_sort(int arr[], const size_t len, cmp_fn cmp);

static const std::pair<const char *, sort_fn> sort_methods[] = {
    {"std_sort", std_sort}};

void std_sort(int arr[], const size_t len, cmp_fn cmp) {
    std::sort(arr, arr + len, cmp);
}

#endif // _VARSORT_HPP
