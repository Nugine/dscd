#if !defined(_COMMON_HPP)
#define _COMMON_HPP

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <random>

template <typename T> T input_number(const char *prompt) {
    T n;
    std::string s;
    for (;;) {
        std::cout << prompt << std::flush;
        std::cin >> n;
        if (std::cin) {
            std::getline(std::cin, s);
            if (s.empty()) {
                return n;
            } else {
                continue;
            }
        }
        if (std::cin.eof()) {
            std::cerr << "fatal error: stdin eof" << std::endl;
            exit(1);
        }
        std::cin.clear();
        std::getline(std::cin, s);
    }
}

template <typename T>
T input_number(const char *prompt, const T lowbd, const T upbd) {
    T n;
    std::string s;
    for (;;) {
        std::cout << prompt << std::flush;
        std::cin >> n;
        std::getline(std::cin, s);
        if (std::cin && s.empty() && lowbd <= n && n < upbd) {
            return n;
        }
        if (std::cin.eof()) {
            std::cerr << "fatal error: stdin eof" << std::endl;
            exit(1);
        }
        std::cin.clear();
    }
}

std::string input_string(const char *prompt) {
    std::string s;
    for (;;) {
        std::cout << prompt << std::flush;
        std::getline(std::cin, s);
        if (std::cin && !s.empty()) {
            return s;
        }
        if (std::cin.eof()) {
            std::cerr << "fatal error: stdin eof" << std::endl;
            exit(1);
        }
        std::cin.clear();
    }
}

#endif // _COMMON_HPP
