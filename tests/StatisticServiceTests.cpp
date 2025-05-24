#include <gtest/gtest.h>
#include <memory>

#include "../src/data/services/StatisticService.h"
#include "../src/data/repositories/OutputRepository.h"

class MockOutputRepository : public OutputRepositoryInterface {
public:

    std::vector<std::string> output_lines;

public:

    void WriteLine(std::string_view line) override {
        output_lines.emplace_back(line);
    }

    void Clear() { 
        output_lines.clear(); 
    }
};

class StatisticServiceTests : public testing::Test {
public:

    InputHeaderData header_;
    std::shared_ptr<MockOutputRepository> repository_;
    std::shared_ptr<StatisticService> service_;

public:

    StatisticServiceTests()
        : header_(3, Time("09:00"), Time("21:00"), 100)
        , repository_(std::make_shared<MockOutputRepository>())
        , service_(std::make_shared<StatisticService>(header_, repository_))
    { }
};

TEST_F(StatisticServiceTests, ClientFullCycle) {
    
    auto arrival_event = ClientArrivalEvent(1, Time("10:00"), "client1");
    service_->HandleEvent(arrival_event);

    auto took_table_event = ClientTookTableEvent(2, Time("10:15"), "client1", 2);
    service_->HandleEvent(took_table_event);

    auto left_event = ClientLeftEvent(4, Time("12:00"), "client1");
    service_->HandleEvent(left_event);

    service_->WriteTablesReport();

    auto mock_repository = static_cast<MockOutputRepository*>(repository_.get());
    ASSERT_EQ(repository_->output_lines.size(), 8);
    EXPECT_EQ(repository_->output_lines[6], "2 200 01:45");
}

TEST_F(StatisticServiceTests, ErrorOnTableTakeWhenOccupied) {
    auto arrival_event_1 = ClientArrivalEvent(1, Time("10:00"), "client1");
    service_->HandleEvent(arrival_event_1);

    auto took_table_event_1 = ClientTookTableEvent(2, Time("10:01"), "client1", 1);
    service_->HandleEvent(took_table_event_1);
    
    auto arrival_event_2 = ClientArrivalEvent(1, Time("10:02"), "client2");
    service_->HandleEvent(arrival_event_2);

    auto took_table_event_2 = ClientTookTableEvent(2, Time("10:03"), "client2", 1);
    service_->HandleEvent(took_table_event_2);
    
    const auto& out = repository_->output_lines;
    EXPECT_EQ(out.back(), "10:03 13 PlaceIsBusy");
}

TEST_F(StatisticServiceTests, AutoLeaveAtClosingTime) {
    ClientArrivalEvent arrival(1, Time("20:55"), "client1");
    ClientTookTableEvent took_table(2, Time("20:56"), "client1", 1);

    service_->HandleEvent(arrival);
    service_->HandleEvent(took_table);
    repository_->Clear();

    service_->WriteTablesReport();
    
    const auto& out = repository_->output_lines;
    ASSERT_EQ(out.size(), 5);
    EXPECT_EQ(out[0], "21:00 11 client1");
    EXPECT_EQ(out[2], "1 100 00:04");
}

TEST_F(StatisticServiceTests, MultipleTablesRevenueCalculation) {
    ClientArrivalEvent arrival1(1, Time("10:00"), "client1");
    ClientTookTableEvent took_table1(2, Time("10:00"), "client1", 1);
    ClientArrivalEvent arrival2(1, Time("11:00"), "client2");
    ClientTookTableEvent took_table2(2, Time("11:00"), "client2", 2);
    ClientLeftEvent left1(4, Time("12:00"), "client1");
    ClientLeftEvent left2(4, Time("13:00"), "client2");

    service_->HandleEvent(arrival1);
    service_->HandleEvent(took_table1);
    service_->HandleEvent(arrival2);
    service_->HandleEvent(took_table2);
    service_->HandleEvent(left1);
    service_->HandleEvent(left2);
    repository_->Clear();

    service_->WriteTablesReport();
    
    const auto& out = repository_->output_lines;
    ASSERT_EQ(out.size(), 4);
    EXPECT_EQ(out[1], "1 200 02:00");
    EXPECT_EQ(out[2], "2 200 02:00");
    EXPECT_EQ(out[3], "3 0 00:00");
}

TEST_F(StatisticServiceTests, ClientWaitsAndGetsTableWhenAvailable) {

    ClientArrivalEvent arrival1(1, Time("10:00"), "client1");
    ClientTookTableEvent took1(2, Time("10:01"), "client1", 1);
    ClientArrivalEvent arrival2(1, Time("10:02"), "client2");
    ClientTookTableEvent took2(2, Time("10:03"), "client2", 2);
    ClientArrivalEvent arrival3(1, Time("10:04"), "client3");
    ClientTookTableEvent took3(2, Time("10:05"), "client3", 3);

    service_->HandleEvent(arrival1);
    service_->HandleEvent(took1);
    service_->HandleEvent(arrival2);
    service_->HandleEvent(took2);
    service_->HandleEvent(arrival3);
    service_->HandleEvent(took3);

    ClientArrivalEvent arrival4(1, Time("10:06"), "client4");
    ClientWaitEvent waitEvent(3, Time("10:07"), "client4");
    service_->HandleEvent(arrival4);
    service_->HandleEvent(waitEvent);

    ClientLeftEvent left2(4, Time("11:00"), "client2");
    service_->HandleEvent(left2);

    const auto& out = repository_->output_lines;
    ASSERT_GE(out.size(), 9);
    EXPECT_EQ(out[out.size() - 2], "11:00 4 client2");
    EXPECT_EQ(out.back(), "11:00 12 client4 2");
}

TEST_F(StatisticServiceTests, ArrivalAfterClosingTimeRejected) {
    ClientArrivalEvent late_arrival(1, Time("21:01"), "client1");
    service_->HandleEvent(late_arrival);

    const auto& out = repository_->output_lines;
    ASSERT_FALSE(out.empty());
    EXPECT_EQ(out.back(), "21:01 13 NotOpenYet");
}

TEST_F(StatisticServiceTests, TakeNonExistentTableRejected) {
    ClientArrivalEvent arrival(1, Time("10:00"), "client1");
    ClientTookTableEvent invalid_table(2, Time("10:01"), "client1", 4);

    service_->HandleEvent(arrival);
    EXPECT_THROW(service_->HandleEvent(invalid_table), std::runtime_error);
}

TEST_F(StatisticServiceTests, ClientLeavesWithoutTakingTable) {
    ClientArrivalEvent arrival(1, Time("10:00"), "client1");
    ClientLeftEvent left(4, Time("10:05"), "client1");

    service_->HandleEvent(arrival);
    service_->HandleEvent(left);

    const auto& out = repository_->output_lines;
    ASSERT_EQ(out.size(), 3);
    EXPECT_EQ(out[1], "10:00 1 client1");
    EXPECT_EQ(out[2], "10:05 4 client1");
}

TEST_F(StatisticServiceTests, MultipleClientsSameTableSequentially) {
    ClientArrivalEvent arrival1(1, Time("10:00"), "client1");
    ClientTookTableEvent took1(2, Time("10:00"), "client1", 1);
    ClientLeftEvent left1(4, Time("11:00"), "client1");

    ClientArrivalEvent arrival2(1, Time("11:00"), "client2");
    ClientTookTableEvent took2(2, Time("11:00"), "client2", 1);
    ClientLeftEvent left2(4, Time("12:00"), "client2");

    service_->HandleEvent(arrival1);
    service_->HandleEvent(took1);
    service_->HandleEvent(left1);
    service_->HandleEvent(arrival2);
    service_->HandleEvent(took2);
    service_->HandleEvent(left2);
    repository_->Clear();

    service_->WriteTablesReport();
    
    const auto& out = repository_->output_lines;
    ASSERT_GE(out.size(), 4);
    EXPECT_EQ(out[1], "1 200 02:00");
}