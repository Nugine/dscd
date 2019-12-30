#if !defined(_COMMON_HPP)
#define _COMMON_HPP

#include <iostream>
#include <string>

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
