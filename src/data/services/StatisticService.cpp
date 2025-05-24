#include "StatisticService.h"

#include <algorithm>
#include <cmath>

#include "../../domain/models/ErrorEvent.h"
#include "../../domain/models/ClientArrivalEvent.h"
#include "../../domain/models/ClientTookTableEvent.h"
#include "../../domain/models/ClientWaitEvent.h"
#include "../../domain/models/ClientLeftEvent.h"

void StatisticService::LeftTable(const Time &cur_time, const std::string& client) {
    if (clients_table_.find(client) == clients_table_.end()) {
        return;
    }

    uint32_t table_id = clients_table_[client];
    Time busy_time = cur_time - busy_tables_[table_id];
    std::cout << "client: " << client << " table: " << table_id << " time: " << busy_time.GetString() << "\n";
    uint32_t hours = std::ceil(static_cast<double>(busy_time.TimeInMinute()) / 60);
    uint32_t revenue = header_.hour_cost_ * hours;

    statistic_[table_id].time_ += busy_time;
    statistic_[table_id].revenue_ += revenue;
    busy_tables_.erase(table_id);
}

void StatisticService::TakeTable(const Time& cur_time, const std::string& client, uint32_t table_id) {
    busy_tables_.emplace(table_id, cur_time);
    clients_table_.emplace(client, table_id);
}

StatisticService::StatisticService(InputHeaderData &header, std::shared_ptr<OutputRepositoryInterface> repository)
    : header_(header)
    , statistic_(header.count_tables_ + 1)
    , clients_in_club_()
    , busy_tables_()
    , clients_table_()
    , waiting_clients_()
    , repository_(repository)
{
    repository_->WriteLine(header.start_time_.GetString());
}

void StatisticService::HandleEvent(ClientArrivalEvent& event) {
    repository_->WriteLine(event.GetString());
    const uint8_t kErrorId = 13;

    if (clients_in_club_.find(event.client_) != clients_in_club_.end()) {
        ErrorEvent error_event(kErrorId, event.time_, "YouShallNotPass");
        error_event.AcceptVisitor(shared_from_this());
        return;
    }

    if (event.time_ < header_.start_time_) {
        ErrorEvent error_event(kErrorId, event.time_, "NotOpenYet");
        error_event.AcceptVisitor(shared_from_this());
        return;
    }

    clients_in_club_.emplace(event.client_);
}

void StatisticService::HandleEvent(ClientTookTableEvent& event) {
    repository_->WriteLine(event.GetString());
    const uint8_t kErrorId = 13;

    if (busy_tables_.find(event.table_id_) != busy_tables_.end()){
        ErrorEvent error_event(kErrorId, event.time_, "PlaceIsBusy");
        error_event.AcceptVisitor(shared_from_this());
        return;
    }

    if (clients_in_club_.find(event.client_) == clients_in_club_.end()) {
        ErrorEvent error_event(kErrorId, event.time_, "ClientUnknown");
        error_event.AcceptVisitor(shared_from_this());
        return;
    }

    LeftTable(event.time_, event.client_);
    TakeTable(event.time_, event.client_, event.table_id_);
}

void StatisticService::HandleEvent(ClientWaitEvent &event){
    repository_->WriteLine(event.GetString());
    const uint8_t kErrorId = 13;
    const uint8_t kErrorLeftId = 11;

    if (header_.count_tables_ > busy_tables_.size()) {
        ErrorEvent error_event(kErrorId, event.time_, "ICanWaitNoLonger");
        error_event.AcceptVisitor(shared_from_this());
        return;
    }
    if (waiting_clients_.size() >= header_.count_tables_) {
        ClientLeftEvent left_event(kErrorLeftId, event.time_, event.client_);
        return;
    }
    waiting_clients_.emplace(event.client_);
}

void StatisticService::HandleEvent(ClientLeftEvent &event) {
    repository_->WriteLine(event.GetString());
    const uint8_t kErrorId = 13;

    if (clients_in_club_.find(event.client_) == clients_in_club_.end()) {
        ErrorEvent error_event(kErrorId, event.time_, "ClientUnknown");
        error_event.AcceptVisitor(shared_from_this());
        return;
    }

    auto table_id = clients_table_[event.client_];
    LeftTable(event.time_, event.client_);
    clients_in_club_.erase(event.client_);
    
    if (waiting_clients_.empty()) {
        return;
    }
    auto client = waiting_clients_.front();
    waiting_clients_.pop();
    ClientTookTableEvent took_event(12, event.time_, client, table_id);
    took_event.AcceptVisitor(shared_from_this());
}

void StatisticService::HandleEvent(ErrorEvent &event) {
    repository_->WriteLine(event.GetString());
}

void StatisticService::WriteTablesReport() {
    std::vector<std::string> clients(clients_in_club_.begin(), clients_in_club_.end());
    std::sort(clients.begin(), clients.end());

    const uint8_t kClientLeft = 11;
    for (const auto& client : clients) {
        ClientLeftEvent event(kClientLeft, header_.end_time_, client);
        event.AcceptVisitor(shared_from_this());
        // repository_->WriteLine(event.GetString());
    }

    repository_->WriteLine(header_.end_time_.GetString());

    for (size_t i = 1; i < statistic_.size(); ++i) {
        auto& stat = statistic_[i];
        const std::string report = std::to_string(i) + " " + std::to_string(stat.revenue_) + " " + stat.time_.GetString();
        repository_->WriteLine(report);
    }

}