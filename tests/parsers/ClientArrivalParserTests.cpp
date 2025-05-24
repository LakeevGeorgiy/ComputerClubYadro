#include <gtest/gtest.h>
#include <type_traits>

#include "../src/data/services/parsers/ClientArrivalParser.h"
#include "../src/data/services/parsers/ClientLeftParser.h"
#include "../src/data/services/parsers/ClientTookTableParser.h"
#include "../src/data/services/parsers/ClientWaitParser.h"

class ClientArrivalParserTests : public testing::Test {
public:

    std::shared_ptr<EventParserInterface> parser_;

public:

    ClientArrivalParserTests() {
        parser_ = std::make_shared<ClientArrivalParser>();
    }
};

TEST_F(ClientArrivalParserTests, Success) {

    const uint8_t id = 1;
    const Time time("09:00");
    const std::string client("client1");

    std::string_view data = time.GetString() + " " + std::to_string(id) + " " + client;
    auto event = parser_->ParseEvent(data);

    EXPECT_EQ(event->id_, id);
    EXPECT_EQ(event->time_, time);

    EXPECT_NE(dynamic_cast<ClientArrivalEvent*>(event.get()), nullptr);
}

TEST_F(ClientArrivalParserTests, FirstZeroHourError) {

    std::string_view data = "9:00 1 client1";

    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}

TEST_F(ClientArrivalParserTests, DelimeterError) {

    std::string_view data = "09.00 1 client1";

    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}

TEST_F(ClientArrivalParserTests, ZeroMinuteError) {

    std::string_view data = "09:1 1 client1";

    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}

TEST_F(ClientArrivalParserTests, NoSpaceError) {

    std::string_view data = "09:001 client1";

    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}

TEST_F(ClientArrivalParserTests, TwoSpacesError) {

    std::string_view data = "09:00  1 client1";

    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}

TEST_F(ClientArrivalParserTests, AddTextAfterError) {

    std::string_view data = "09:00 1 client1 smth";

    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}

TEST_F(ClientArrivalParserTests, AddTextBeforeError) {

    std::string_view data = "smth 09:00 1 client1";

    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}