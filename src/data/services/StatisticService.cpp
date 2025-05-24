#include "StatisticService.h"

#include "../../domain/models/ErrorEvent.h"
#include "../../domain/models/ClientArrivalEvent.h"
#include "../../domain/models/ClientTookTableEvent.h"
#include "../../domain/models/ClientWaitEvent.h"
#include "../../domain/models/ClientLeftEvent.h"

void StatisticService::LeftTable(const Time &cur_time, const std::string& client) {
    uint32_t table_id = clients_[client];
    Time busy_time = cur_time - busy_tables_[table_id];
    statistic_[table_id].time_ += busy_time;
    busy_tables_.erase(table_id);
}

StatisticService::StatisticService(InputHeaderData &header, std::shared_ptr<OutputRepositoryInterface> repository)
    : header_(header)
    , statistic_(header.count_tables_)
    , busy_tables_()
    , clients_()
    , waiting_clients_()
    , repository_(repository)
{ }

void StatisticService::HandleEvent(ClientArrivalEvent& event) {
    repository_->WriteLine(event.GetString());
    const uint8_t kErrorId = 13;

    if (clients_[event.client_] != UINT32_MAX) {
        ErrorEvent error_event(kErrorId, event.time_, "YouShallNotPass");
        error_event.AcceptVisitor(shared_from_this());
        return;
    }

    if (header_.end_time_ < event.time_) {
        ErrorEvent error_event(kErrorId, event.time_, "NotOpenYet");
        error_event.AcceptVisitor(shared_from_this());
        return;
    }
}

void StatisticService::HandleEvent(ClientTookTableEvent& event) {
    repository_->WriteLine(event.GetString());
    const uint8_t kErrorId = 13;

    if (busy_tables_.find(event.table_id_) != busy_tables_.end()){
        ErrorEvent error_event(kErrorId, event.time_, "PlaceIsBusy");
        error_event.AcceptVisitor(shared_from_this());
        return;
    }

    if (clients_[event.client_] == UINT32_MAX) {
        ErrorEvent error_event(kErrorId, event.time_, "ClientUnknown");
        error_event.AcceptVisitor(shared_from_this());
        return;
    }

    LeftTable(event.time_, event.client_);
    busy_tables_[event.table_id_] = event.time_;
    clients_[event.client_] = event.table_id_;
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

    if (clients_.find(event.client_) == clients_.end()) {
        ErrorEvent error_event(kErrorId, event.time_, "ClientUnknown");
        error_event.AcceptVisitor(shared_from_this());
        return;
    }

    auto table_id = clients_[event.client_];
    LeftTable(event.time_, event.client_);

    auto client = waiting_clients_.front();
    waiting_clients_.pop();
    busy_tables_[table_id] = event.time_;
}

void StatisticService::HandleEvent(ErrorEvent &event) {
    repository_->WriteLine(event.GetString());
}

void StatisticService::WriteTablesReport() {
}