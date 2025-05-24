#pragma once

#include <vector>
#include <unordered_map>
#include <queue>

#include "../../domain/models/InputHeaderData.h"
#include "../../domain/services/StatisticServiceInterface.h"

struct TableStatistic {
    uint32_t revenue_;
    Time time_;
    TableStatistic(): revenue_(0), time_(0, 0) {}
};

class StatisticService : public StatisticServiceInterface, public std::enable_shared_from_this<StatisticService> {
private:

    InputHeaderData header_;
    std::vector<TableStatistic> statistic_;

    std::unordered_map<uint32_t, Time> busy_tables_;
    std::unordered_map<std::string, uint32_t> clients_;
    std::queue<std::string> waiting_clients_;

    std::shared_ptr<OutputRepositoryInterface> repository_;

    void LeftTable(const Time& cur_time, const std::string& client);
    
public:

    StatisticService(InputHeaderData& header, std::shared_ptr<OutputRepositoryInterface> repository);

    void HandleEvent(ClientArrivalEvent& event) override;
    void HandleEvent(ClientTookTableEvent& event) override;
    void HandleEvent(ClientWaitEvent& event) override;
    void HandleEvent(ClientLeftEvent& event) override;
    void HandleEvent(ErrorEvent& event) override;
    void WriteTablesReport() override;

};