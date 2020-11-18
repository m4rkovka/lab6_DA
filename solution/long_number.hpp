#ifndef LONG_NUMBER_HPP
#define LONG_NUMBER_HPP 1

#include <vector>
#include <iostream>
#include <string>

class longNumber {
    std::vector<unsigned long long> lnum;
    const unsigned long base = 1000000000;

public:
    longNumber() = default;
    longNumber(const std::string &str);
    longNumber(const longNumber &ln);
    longNumber &operator=(const longNumber &second);
    bool operator<(const longNumber &second);
    bool operator>(const longNumber &second);
    bool operator==(const longNumber &second);
    bool operator<=(const longNumber &second);
    bool operator>=(const longNumber &second);

    friend longNumber pow(longNumber &n, const longNumber &exp);
    friend longNumber operator+(const longNumber &first, const longNumber &second);
    friend longNumber operator-(const longNumber &first, const longNumber &second);
    friend longNumber operator*(const longNumber &first, const longNumber &second);
    friend longNumber operator/(const longNumber &first, const longNumber &second);
    friend std::ostream &operator<<(std::ostream &out, const longNumber &num);

private:
    friend int compare(const longNumber &first, const longNumber &second);
    friend longNumber help_pow(longNumber &n, const longNumber &exp);
    void deleteLeadingZeros();
    friend longNumber mult_column(const longNumber &first, const longNumber &second);
    friend longNumber mult_small(const longNumber &first, const longNumber &second);
    /* karatsuba
    void mult_pow_base(size_t e);
    friend longNumber mult_karatsuba(longNumber &first, longNumber &second);
    void insertLeadingZeros(size_t cnt);
    */
    void shift_right();
    friend longNumber div(const longNumber &first, const longNumber &second);
};

#endif 