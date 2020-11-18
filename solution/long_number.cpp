#include "long_number.hpp"
#include <cmath>
#include <iomanip>
#include <cstdlib>
#include <algorithm>
#include <exception>

longNumber::longNumber(const std::string &str) {
    int power = static_cast<int>(log10(static_cast<double>(base)));
    for (int i = static_cast<int>((str.size() - 1)); i >= 0; i -= power) {
        if (i < power) {
            lnum.push_back(std::atoll(str.substr(0, i + 1).c_str()));
        } else {
            lnum.push_back(std::atoll(str.substr(i - power + 1, power).c_str()));
        }
    }

    deleteLeadingZeros();
}

longNumber::longNumber(const longNumber &ln) {
    lnum = ln.lnum;
}

longNumber &longNumber::operator=(const longNumber &second) {
    this->lnum = second.lnum;
    return *this;
}

bool longNumber::operator<(const longNumber &second) {
    return (compare(*this, second) == 1 ? true : false);
}

bool longNumber::operator>(const longNumber &second) {
    return (compare(*this, second) == -1 ? true : false);
}

bool longNumber::operator==(const longNumber &second) {
    return (compare(*this, second) == 0 ? true : false);
}

bool longNumber::operator<=(const longNumber &second) {
    int res = compare(*this, second);
    if (res == 1 || res == 0) 
        return true;
    return false;
}

bool longNumber::operator>=(const longNumber &second) {
    int res = compare(*this, second);
    if (res == -1 || res == 0) 
        return true;
    return false;
}

longNumber pow(longNumber &n, const longNumber &exp) {
    if (longNumber("0") == exp) {
        if (n == longNumber("0")) {
            throw std::logic_error("Error");
        } else {
            return longNumber("1");
        }
    }
    if (n == longNumber("1") || longNumber("1") == exp) {
        return n;
    }
    return help_pow(n, exp);
}

longNumber operator+(const longNumber &first, const longNumber &second) {
    unsigned long carryover = 0;
    unsigned long maxSize = std::max(first.lnum.size(), second.lnum.size());
    unsigned long sumInDigit = 0;
    longNumber res;
    for (size_t i = 0; i < maxSize; i++) {
        if (first.lnum.size() <= i) {
            sumInDigit = second.lnum[i] + carryover;
        } else if (second.lnum.size() <= i) {
            sumInDigit = first.lnum[i] + carryover;
        } else {
            sumInDigit = first.lnum[i] + second.lnum[i] + carryover;
        }
        res.lnum.push_back(sumInDigit % first.base);
        carryover = sumInDigit / first.base;
    }
    if (carryover != 0) {
        res.lnum.push_back(carryover);
    }
    return res;
}

longNumber operator-(const longNumber &first, const longNumber &second) {
    if (first.lnum.size() < second.lnum.size()) {
        throw std::logic_error("Error");
    }
    unsigned long carryover = 0;
    unsigned long size = first.lnum.size();
    long long differenceInDigit = 0;
    longNumber res;
    for (size_t i = 0; i < size; i++) {
        if (i >= second.lnum.size()) {
            differenceInDigit = first.lnum[i] - carryover;
        } else {
            differenceInDigit = first.lnum[i] - second.lnum[i] - carryover;
        }
        carryover = 0;
        if (differenceInDigit < 0) {
            differenceInDigit += first.base;
            carryover = 1;
        }
        res.lnum.push_back(differenceInDigit);
    }
    if (carryover == 1) {
        throw std::logic_error("Error");
    }
    res.deleteLeadingZeros();
    return res;
}

longNumber operator*(const longNumber &first, const longNumber &second) {
    if (second.lnum.size() == 1) {
        return mult_small(first, second);
    } else if (first.lnum.size() == 1) {
        return mult_small(second, first);
    }
    /* karatsuba
    longNumber a{first}, b{second};
    if (first.lnum.size() > second.lnum.size()) {
        size_t diff = first.lnum.size() - second.lnum.size();
        a.insertLeadingZeros(diff);
    } else if (first.lnum.size() < second.lnum.size()) {
        size_t diff = second.lnum.size() - first.lnum.size();
        b.insertLeadingZeros(diff);
    }
    longNumber res = mult_karatsuba(a, b);
    res.deleteLeadingZeros();
    return res;
    */
    longNumber res = mult_column(first, second);
    res.deleteLeadingZeros();
    return res;
}

longNumber operator/(const longNumber &first, const longNumber &second) {
    return div(first, second);
}

std::ostream &operator<<(std::ostream &out, const longNumber &num) {
    out << (num.lnum.empty() ? 0 : num.lnum.back());
    int power = static_cast<int>(log10(static_cast<double>(num.base)));
    for (int i = static_cast<int>((num.lnum.size() - 2)); i >= 0; i--) {
        out << std::setw(power) << std::setfill('0') << num.lnum[i];
    }

    return out;
}

int compare(const longNumber &first, const longNumber &second) {
    if (first.lnum.size() < second.lnum.size()) {
        return 1;
    } else if (first.lnum.size() > second.lnum.size()) {
        return -1;
    }
    for (size_t i = first.lnum.size(); i > 0; i--) {
        if (first.lnum[i - 1] < second.lnum[i - 1]) {
            return 1;
        } else if (first.lnum[i - 1] > second.lnum[i - 1]) {
            return -1;
        }
    }
    return 0;
}

longNumber help_pow(longNumber &n, const longNumber &exp) {
    if (longNumber{"1"} == exp) {
        return n;
    }
    if (exp.lnum[0] % 2 == 0) {
        longNumber x = help_pow(n, exp / longNumber{"2"});
        return x * x;
    } else {
        return help_pow(n, exp - longNumber{"1"}) * n;
    }
}

void longNumber::deleteLeadingZeros() {
    while (lnum.size() > 1 && lnum.back() == 0) {
        lnum.pop_back();
    }
}

longNumber mult_column(const longNumber &first, const longNumber &second) {
    longNumber res;
    res.lnum.resize(first.lnum.size() + second.lnum.size());

    for (size_t i = 0; i < second.lnum.size(); i++) {
        long carryover = 0;
        for (size_t j = 0; j < first.lnum.size() || carryover != 0; j++) {
            long long cur = res.lnum[i + j] + second.lnum[i] * 1ll * (j < first.lnum.size() ? first.lnum[j] : 0) + carryover;
            res.lnum[i + j] = cur % first.base;
            carryover = cur / first.base;
        }
    }
    res.deleteLeadingZeros();
    return res;
}

longNumber mult_small(const longNumber &first, const longNumber &second) {
    unsigned long long carryover = 0;
    unsigned long long multInDigit = 0;
    size_t size = first.lnum.size();
    longNumber res;
    for (size_t i = 0; i < size; i++) {
        multInDigit = first.lnum[i] * second.lnum[0] + carryover;
        res.lnum.push_back(multInDigit % first.base);
        carryover = multInDigit / first.base;
    }
    if (carryover != 0) {
        res.lnum.push_back(carryover);
    }
    res.deleteLeadingZeros();
    return res;
}

/* karatsuba
void longNumber::mult_pow_base(size_t e) {
    std::vector<unsigned long long> toIns;
    for (size_t i = 0; i < e; i++) {
        toIns.push_back(0);
    }
    this->lnum.insert(this->lnum.begin(), toIns.begin(), toIns.end());
    this->deleteLeadingZeros();
}

longNumber mult_karatsuba(longNumber &first, longNumber &second) {
    longNumber zero{"0"};
    if (first == zero || second == zero) {
        return zero;
    }
    if (second.lnum.size() == 1) {
        return mult_small(first, second);
    } else if (first.lnum.size() == 1) {
        return mult_small(second, first);
    }

    if (first.lnum.size() > second.lnum.size()) {
        size_t diff = first.lnum.size() - second.lnum.size();
        second.insertLeadingZeros(diff);
    } else if (first.lnum.size() < second.lnum.size()) {
        size_t diff = second.lnum.size() - first.lnum.size();
        first.insertLeadingZeros(diff);
    }

    longNumber aZero, aOne, bZero, bOne;
    aZero.lnum.resize(first.lnum.size() / 2);
    bZero.lnum.resize(first.lnum.size() / 2);
    aOne.lnum.resize(first.lnum.size() - first.lnum.size() / 2);
    bOne.lnum.resize(first.lnum.size() - first.lnum.size() / 2);

    for (size_t i = 0; i <= first.lnum.size() / 2 - 1; i++) {
        aZero.lnum[i] = first.lnum[i];
        bZero.lnum[i] = second.lnum[i];
    }
    for (size_t i = first.lnum.size() / 2; i < first.lnum.size(); i++) {
        aOne.lnum[i - first.lnum.size() / 2] = first.lnum[i];
        bOne.lnum[i - first.lnum.size() / 2] = second.lnum[i];
    }

    longNumber firstRes = mult_karatsuba(aZero, bZero);
    longNumber secondRes = mult_karatsuba(aOne, bOne);
    longNumber sumA = aZero + aOne;
    longNumber sumB = bZero + bOne;
    longNumber thirdRes = mult_karatsuba(sumA, sumB) - firstRes - secondRes;
    thirdRes.mult_pow_base(first.lnum.size() / 2);
    secondRes.mult_pow_base(first.lnum.size() / 2 * 2);
    
    first.deleteLeadingZeros();
    second.deleteLeadingZeros();

    return (firstRes + secondRes + thirdRes);
}

void longNumber::insertLeadingZeros(size_t cnt) {
    for (size_t i = cnt; i > 0; i--) {
        this->lnum.push_back(0);
    }
}
*/

void longNumber::shift_right() {
    if (this->lnum.size() == 0) {
        this->lnum.push_back(0);
        return;
    }
    this->lnum.push_back(this->lnum[this->lnum.size() - 1]);
    for (size_t i = this->lnum.size() - 2; i > 0; i--) {
        this->lnum[i] = this->lnum[i - 1];
    }
    this->lnum[0] = 0;
}

longNumber div(const longNumber &first, const longNumber &second) {
    if (longNumber("0") == second) {
        throw std::logic_error("Error");
    }
    longNumber b = second;
    longNumber result, current;
    result.lnum.resize(first.lnum.size());
    for (long i = static_cast<long>(first.lnum.size()) - 1; i >= 0; i--) {
        current.shift_right();
        current.lnum[0] = first.lnum[i];
        current.deleteLeadingZeros();
        long long x = 0, l = 0, r = static_cast<long long>(first.base);
        while (l <= r) {
            long long m = (l + r) / 2;
            longNumber t = mult_small(b, longNumber(std::to_string(m)));
            if (t <= current) {
                x = m;
                l = m + 1;
            } else {
                r = m - 1;
            }
        }
        result.lnum[i] = x;
        current = current - b * longNumber(std::to_string(x));
    }
    result.deleteLeadingZeros();
    return result;
}
