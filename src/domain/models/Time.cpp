#include "Time.h"

#include <regex>
#include <exception>

uint8_t Time::GetDigit(char symbol) {
    return symbol - '0';
}

uint32_t Time::TimeInMinute() const {
    return hours_ * 60 + minutes_;
}

Time::Time(std::string_view time_str) {

    const std::string copy_str(time_str);
    const std::string kTimeRegex = "^[\\d]{2}:[\\d]{2}";
    std::regex regex(kTimeRegex);
    
    if (!std::regex_match(copy_str, regex)) {
        throw std::runtime_error("Can't construct time from string");
    }
    
    hours_ = GetDigit(copy_str[0]) * 10 + GetDigit(copy_str[1]);
    minutes_ = GetDigit(copy_str[3]) * 10 + GetDigit(copy_str[4]);

}
Time::Time(uint8_t hours, uint8_t minutes)
    : hours_(hours)
    , minutes_(minutes)
{ }

Time::Time(const Time &other)
    : hours_(other.hours_)
    , minutes_(other.minutes_)
{ }

std::string Time::GetString() const {
    return std::to_string(hours_ / 10) + std::to_string(hours_ % 10) + ":" + std::to_string(minutes_ / 10) + std::to_string(minutes_ % 10);
}

Time& Time::operator+=(const Time &rhs) {
    minutes_ += rhs.minutes_;
    hours_ += (rhs.hours_ + minutes_ / 60);
    minutes_ %= 60;
    return *this;
}

Time operator-(const Time &lhs, const Time &rhs) {
    auto lhs_minutes = lhs.TimeInMinute();
    auto rhs_minutes = rhs.TimeInMinute();
    auto diff = lhs_minutes - rhs_minutes;
    
    uint8_t minute = diff % 60;
    uint8_t hour = diff / 60;

    return Time(hour, minute);
}

bool operator==(const Time &lhs, const Time &rhs) {
    return lhs.hours_ == rhs.hours_ && lhs.minutes_ == rhs.minutes_;
}

bool operator<(const Time &lhs, const Time &rhs){
    return lhs.hours_ < rhs.hours_ || (lhs.hours_ == rhs.hours_ && lhs.hours_ < rhs.hours_);
}
