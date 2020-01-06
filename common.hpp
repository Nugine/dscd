#if !defined(_COMMON_HPP)
#define _COMMON_HPP

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <random>
#include <sstream>
#include <string>
#include <vector>

template <typename T>
T input_number(const char *prompt, const T lowbd, const T upbd) {
    T n;
    std::string s;
    for (;;) {
        std::cout << prompt << std::flush;
        if (getline(std::cin, s)) {
            std::stringstream ss(s);
            if (ss >> n && lowbd <= n && n < upbd) {
                if (!(ss >> s)) {
                    return n;
                }
            }
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
