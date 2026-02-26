#pragma once
#include <iostream>
#include <string>

using namespace std;

namespace Colors {
const string RESET = "\033[0m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN = "\033[36m";
const string WHITE = "\033[37m";
const string BOLD = "\033[1m";
}  // namespace Colors

inline void logger(string message, int type) {
    if (type == 0) {
        cout << Colors::GREEN << "[INFO] " << message << Colors::RESET << "\n";
    } else if (type == 1) {
        cout << Colors::YELLOW << "[DEBUG] " << message << Colors::RESET << "\n";
    } else {
        cout << Colors::RED << "[ERROR] " << message << Colors::RESET << "\n";
    }
}