#include <gtest/gtest.h>
#include <type_traits>

#include "../src/data/services/parsers/ClientArrivalParser.h"
#include "../src/data/services/parsers/ClientLeftParser.h"
#include "../src/data/services/parsers/ClientTookTableParser.h"
#include "../src/data/services/parsers/ClientWaitParser.h"

class ArrivalEventParserTests : public testing::Test {
public:

    std::shared_ptr<EventParserInterface> parser_;

public:

    ArrivalEventParserTests() {
        parser_ = std::make_shared<ClientArrivalParser>();
    }
};

TEST_F(ArrivalEventParserTests, Success) {

    const uint8_t id = 1;
    const Time time("09:00");
    const std::string client("client1");

    std::string data = time.GetString() + " " + std::to_string(id) + " " + client;
    auto event = parser_->ParseEvent(data);

    EXPECT_EQ(event->id_, id);
    EXPECT_EQ(event->time_, time);

    EXPECT_NE(dynamic_cast<ClientArrivalEvent*>(event.get()), nullptr);
}



TEST_F(ArrivalEventParserTests, NoSpaceError) {

    std::string_view data = "09:001 client1";

    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}

TEST_F(ArrivalEventParserTests, TwoSpacesError) {

    std::string_view data = "09:00  1 client1";

    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}

TEST_F(ArrivalEventParserTests, AddTextAfterError) {

    std::string_view data = "09:00 1 client1 smth";

    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}

TEST_F(ArrivalEventParserTests, AddTextBeforeError) {

    std::string_view data = "smth 09:00 1 client1";

    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}

TEST_F(ArrivalEventParserTests, UpperLetterInClient) {

    std::string_view data = "09:00 1 Client1";

    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}

TEST_F(ArrivalEventParserTests, InvalidSymbolInClient) {

    std::string_view data = "09:00 1 client1]";

    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}