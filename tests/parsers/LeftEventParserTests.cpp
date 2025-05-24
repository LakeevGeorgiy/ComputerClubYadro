
#include <gtest/gtest.h>
#include <type_traits>

#include "../src/data/services/parsers/ClientLeftParser.h"
#include "../src/data/services/parsers/ClientArrivalParser.h"
#include "../src/data/services/parsers/ClientTookTableParser.h"
#include "../src/data/services/parsers/ClientWaitParser.h"

class LeftEventParserTests : public testing::Test {
public:

    std::shared_ptr<EventParserInterface> parser_;

public:

    LeftEventParserTests() {
        parser_ = std::make_shared<ClientLeftParser>();
    }
};

TEST_F(LeftEventParserTests, Success) {
    const uint8_t id = 4;
    const Time time("10:15");
    const std::string client("client2");

    std::string data = time.GetString() + " " + std::to_string(id) + " " + client;
    auto event = parser_->ParseEvent(data);

    EXPECT_EQ(event->id_, id);
    EXPECT_EQ(event->time_, time);
    EXPECT_NE(dynamic_cast<ClientLeftEvent*>(event.get()), nullptr);
}

TEST_F(LeftEventParserTests, NoSpaceError) {
    std::string_view data = "10:154 client2";
    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}

TEST_F(LeftEventParserTests, TwoSpacesError) {
    std::string_view data = "10:15  4 client2";
    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}

TEST_F(LeftEventParserTests, AddTextAfterError) {
    std::string_view data = "10:15 4 client2 smth";
    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}

TEST_F(LeftEventParserTests, AddTextBeforeError) {
    std::string_view data = "smth 10:15 4 client2";
    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}

TEST_F(LeftEventParserTests, UpperLetterInClient) {
    std::string_view data = "10:15 4 Client2";
    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}

TEST_F(LeftEventParserTests, InvalidSymbolInClient) {
    std::string_view data = "10:15 4 client2]";
    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}