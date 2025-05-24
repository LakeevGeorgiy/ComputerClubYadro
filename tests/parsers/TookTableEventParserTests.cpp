
#include <gtest/gtest.h>
#include <type_traits>

#include "../src/data/services/parsers/ClientTookTableParser.h"
#include "../src/data/services/parsers/ClientArrivalParser.h"
#include "../src/data/services/parsers/ClientLeftParser.h"
#include "../src/data/services/parsers/ClientWaitParser.h"

class TookTableEventParserTests : public testing::Test {
public:
    std::shared_ptr<EventParserInterface> parser_;

    TookTableEventParserTests() {
        parser_ = std::make_shared<ClientTookTableParser>();
    }
};

TEST_F(TookTableEventParserTests, Success) {
    const uint8_t id = 2;
    const Time time("12:45");
    const std::string client("client3");
    const uint32_t table_id = 5;

    std::string data = time.GetString() + " " + std::to_string(id) + " " + client + " " + std::to_string(table_id);
    auto event = parser_->ParseEvent(data);

    EXPECT_EQ(event->id_, id);
    EXPECT_EQ(event->time_, time);
    auto took_table_event = dynamic_cast<ClientTookTableEvent*>(event.get());
    EXPECT_NE(took_table_event, nullptr);
    EXPECT_EQ(took_table_event->table_id_, table_id);
}

TEST_F(TookTableEventParserTests, NoSpaceError) {
    std::string_view data = "12:452 client3 5";
    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}

TEST_F(TookTableEventParserTests, TwoSpacesError) {
    std::string_view data = "12:45  2  client3  5";
    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}

TEST_F(TookTableEventParserTests, AddTextAfterError) {
    std::string_view data = "12:45 2 client3 5 smth";
    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}

TEST_F(TookTableEventParserTests, AddTextBeforeError) {
    std::string_view data = "smth 12:45 2 client3 5";
    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}

TEST_F(TookTableEventParserTests, MissingTableIdError) {
    std::string_view data = "12:45 2 client3";
    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}

TEST_F(TookTableEventParserTests, InvalidTableIdFormat) {
    std::string_view data = "12:45 2 client3 table5";
    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}

TEST_F(TookTableEventParserTests, ZeroTableIdError) {
    std::string_view data = "12:45 2 client3 0";
    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}

TEST_F(TookTableEventParserTests, UpperLetterInClient) {
    std::string_view data = "12:45 2 Client3 5";
    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}

TEST_F(TookTableEventParserTests, InvalidSymbolInClient) {
    std::string_view data = "12:45 2 client3! 5";
    EXPECT_THROW(parser_->ParseEvent(data), std::invalid_argument);
}
