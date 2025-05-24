
#include <gtest/gtest.h>
#include <type_traits>

#include "../src/data/services/parsers/ClientWaitParser.h"
#include "../src/data/services/parsers/ClientArrivalParser.h"
#include "../src/data/services/parsers/ClientLeftParser.h"
#include "../src/data/services/parsers/ClientTookTableParser.h"

class ClientWaitParserTests : public testing::Test {
public:

    std::shared_ptr<EventParserInterface> parser_;

public:

    ClientWaitParserTests() {
        parser_ = std::make_shared<ClientWaitParser>();
    }
};

TEST_F(ClientWaitParserTests, Success) {
    const uint8_t id = 3;
    const Time time("11:30");
    const std::string client("client4");

    std::string data = time.GetString() + " " + std::to_string(id) + " " + client;
    auto event = parser_->ParseEvent(data);

    EXPECT_EQ(event->id_, id);
    EXPECT_EQ(event->time_, time);
    EXPECT_NE(dynamic_cast<ClientWaitEvent*>(event.get()), nullptr);
}

TEST_F(ClientWaitParserTests, NoSpaceError) {
    std::string_view data = "11:303 client4";
    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}

TEST_F(ClientWaitParserTests, TwoSpacesError) {
    std::string_view data = "11:30  3 client4";
    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}

TEST_F(ClientWaitParserTests, AddTextAfterError) {
    std::string_view data = "11:30 3 client4 smth";
    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}

TEST_F(ClientWaitParserTests, AddTextBeforeError) {
    std::string_view data = "smth 11:30 3 client4";
    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}

TEST_F(ClientWaitParserTests, UpperLetterInClient) {
    std::string_view data = "11:30 3 Client4";
    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}

TEST_F(ClientWaitParserTests, InvalidSymbolInClient) {
    std::string_view data = "11:30 3 client4]";
    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}
