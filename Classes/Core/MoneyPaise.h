#ifndef __MONEY_PAISE_H__
#define __MONEY_PAISE_H__

#include <cstdint>
#include <string>
#include <sstream>
#include <iomanip>

class MoneyPaise {
public:
    int64_t value; // In paise (100 paise = 1 INR)

    constexpr MoneyPaise() : value(0) {}
    constexpr explicit MoneyPaise(int64_t paise) : value(paise) {}

    static MoneyPaise fromRupees(double rupees) {
        if (rupees < 0) return MoneyPaise(0);
        return MoneyPaise(static_cast<int64_t>(rupees * 100.0 + 0.5));
    }

    double toRupees() const {
        return static_cast<double>(value) / 100.0;
    }

    std::string formatted() const {
        char buf[64];
        snprintf(buf, sizeof(buf), "₹%.2f", toRupees());
        return std::string(buf);
    }

    std::string formattedIntegerRupees() const {
        char buf[64];
        snprintf(buf, sizeof(buf), "₹%lld", static_cast<long long>(value / 100));
        return std::string(buf);
    }

    std::string decimalString() const {
        char buf[64];
        snprintf(buf, sizeof(buf), "%.2f", toRupees());
        return std::string(buf);
    }

    MoneyPaise operator+(const MoneyPaise& other) const { return MoneyPaise(value + other.value); }
    MoneyPaise operator-(const MoneyPaise& other) const { return MoneyPaise(value - other.value); }
    MoneyPaise& operator+=(const MoneyPaise& other) { value += other.value; return *this; }
    MoneyPaise& operator-=(const MoneyPaise& other) { value -= other.value; return *this; }

    bool operator==(const MoneyPaise& other) const { return value == other.value; }
    bool operator!=(const MoneyPaise& other) const { return value != other.value; }
    bool operator<(const MoneyPaise& other) const { return value < other.value; }
    bool operator<=(const MoneyPaise& other) const { return value <= other.value; }
    bool operator>(const MoneyPaise& other) const { return value > other.value; }
    bool operator>=(const MoneyPaise& other) const { return value >= other.value; }
};

struct WalletBalance {
    MoneyPaise total;
    MoneyPaise deposit;
    MoneyPaise winning;
    MoneyPaise bonus;

    WalletBalance() : total(0), deposit(0), winning(0), bonus(0) {}
    WalletBalance(MoneyPaise t, MoneyPaise d, MoneyPaise w, MoneyPaise b)
        : total(t), deposit(d), winning(w), bonus(b) {}
};

#endif // __MONEY_PAISE_H__
