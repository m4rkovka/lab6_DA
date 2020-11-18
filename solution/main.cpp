#include <iostream>
#include <exception>
#include <string>
#include "long_number.hpp"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    
    std::string firstOperand, secondOperand;
    char operation;

    while (std::cin >> firstOperand >> secondOperand >> operation)  {
        longNumber left(firstOperand), right(secondOperand);
        if (operation == '+') {
            std::cout << left + right << "\n";
        } else if (operation == '-') {
            try {
                std::cout << left - right << "\n";
            } catch (std::logic_error &e) {
                std::cout << e.what() << "\n";
            }
        } else if (operation == '*') {
            std::cout << left * right << "\n";
        } else if (operation == '^') {
            try {
                std::cout << pow(left, right) << "\n";
            } catch (std::logic_error &e) {
                std::cout << e.what() << "\n";
            }
        } else if (operation == '/') {
            try {
                std::cout << left / right << "\n";
            } catch (std::logic_error &e) {
                std::cout << e.what() << "\n";
            }
        } else if (operation == '<') {
            bool res = left < right;
            if (res) {
                std::cout << "true\n";
            } else {
                std::cout << "false\n";
            }
        } else if (operation == '=') {
            bool res = left == right;
            if (res) {
                std::cout << "true\n";
            } else {
                std::cout << "false\n";
            }
        } else if (operation == '>') {
            bool res = left > right;
            if (res) {
                std::cout << "true\n";
            } else {
                std::cout << "false\n";
            }
        }
    }

    return 0;
}