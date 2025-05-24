#pragma once

#include <string_view>
#include <cstdint>

class Time {
private:

    uint8_t hours_;
    uint8_t minutes_;

    uint8_t GetDigit(char symbol);
    uint32_t TimeInMinute() const;

public:

    Time() = default;
    Time(std::string_view time_str);
    Time(uint8_t hours, uint8_t minutes);
    Time(const Time& other);

    std::string GetString() const;

    Time& operator+=(const Time& rhs);
    friend Time operator-(const Time& lhs, const Time& rhs);
    friend bool operator==(const Time& lhs, const Time& rhs);
    friend bool operator<(const Time& lhs, const Time& rhs);
};