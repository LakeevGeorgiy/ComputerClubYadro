#include <gtest/gtest.h>

#include "../src/domain/models/Time.h"

TEST(TimeTest, SuccessTimeStringConstuctor) {
    const std::string time_str = "09:00";
    EXPECT_NO_THROW(Time time(time_str));
}

TEST(TimeTest, FailedStringConstructorFirstZeroHour) {
    const std::string time_str = "9:00";
    EXPECT_THROW(Time time(time_str), std::runtime_error);
}

TEST(TimeTest, FailedStringConstructorFirstZeroMinute) {
    const std::string time_str = "09:1";
    EXPECT_THROW(Time time(time_str), std::runtime_error);
}

TEST(TimeTest, FailedStringConstructorDelimeter) {
    const std::string time_str = "09.00";
    EXPECT_THROW(Time time(time_str), std::runtime_error);
}

TEST(TimeTest, FailedStringConstructorAddSymbolsBefore) {
    const std::string time_str = "s 09:00";
    EXPECT_THROW(Time time(time_str), std::runtime_error);
}

TEST(TimeTest, FailedStringConstructorAddSymbolsAfter) {
    const std::string time_str = "09:00 s";
    EXPECT_THROW(Time time(time_str), std::runtime_error);
}

TEST(TimeTest, FailedNotCorrectHours) {
    const std::string time_str = "29:00";
    EXPECT_THROW(Time time(time_str), std::runtime_error);
}

TEST(TimeTest, SuccessTimeValueConstuctor) {
    const uint8_t hours = 20;
    const uint8_t minutes = 20;
    EXPECT_NO_THROW(Time time(hours, minutes));
}

TEST(TimeTest, GetStringSimple) {
    const uint8_t hours = 20;
    const uint8_t minutes = 20;
    Time time(hours, minutes);

    const std::string expected = "20:20";
    EXPECT_EQ(time.GetString(), expected);
}

TEST(TimeTest, GetStringWithLeadingZeroHour) {
    const uint8_t hours = 2;
    const uint8_t minutes = 20;
    Time time(hours, minutes);

    const std::string expected = "02:20";
    EXPECT_EQ(time.GetString(), expected);
}

TEST(TimeTest, GetStringWithLeadingZeroMinute) {
    const uint8_t hours = 2;
    const uint8_t minutes = 2;
    Time time(hours, minutes);

    const std::string expected = "02:02";
    EXPECT_EQ(time.GetString(), expected);
}

TEST(TimeTest, TimeInMinute ){
    const uint8_t hours = 2;
    const uint8_t minutes = 20;

    Time time(hours, minutes);
    EXPECT_EQ(time.TimeInMinute(), 140);
}


TEST(TimeTest, TimeInMinuteMidnight){
    const uint8_t hours = 0;
    const uint8_t minutes = 0;

    Time time(hours, minutes);
    EXPECT_EQ(time.TimeInMinute(), 0);
}

TEST(TimeTest, SuccessOperatorPlus) {
    Time first_time(10, 20);
    Time second_time(15, 43);
    first_time += second_time;

    const std::string expected = "26:03";
    EXPECT_EQ(first_time.GetString(), expected);   
}

TEST(TimeTest, SuccessOperatorMinus) {
    Time first_time(10, 20);
    Time second_time(7, 43);
    Time result_time = first_time - second_time;

    const std::string expected = "02:37";
    EXPECT_EQ(result_time.GetString(), expected);   
}

TEST(TimeTest, SuccessOperatorEq) {
    Time first_time(15, 43);
    Time second_time(15, 43);

    EXPECT_EQ(first_time, second_time);   
}

TEST(TimeTest, FailedOperatorEq) {
    Time first_time(19, 43);
    Time second_time(15, 43);

    EXPECT_FALSE(first_time == second_time);   
}

TEST(TimeTest, SuccessOperatorLess) {
    Time first_time(15, 43);
    Time second_time(15, 44);

    EXPECT_TRUE(first_time < second_time);   
}

TEST(TimeTest, FailedOperatorLess) {
    Time first_time(19, 43);
    Time second_time(15, 43);

    EXPECT_FALSE(first_time < second_time);   
}