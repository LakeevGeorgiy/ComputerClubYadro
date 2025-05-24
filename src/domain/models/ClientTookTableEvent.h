#pragma once

#include "BaseEvent.h"

class ClientTookTableEvent : public BaseEvent {
public:

    const std::string client_;
    const uint32_t table_id_;

public:

    ClientTookTableEvent(uint32_t id, const Time& time, std::string_view client, uint32_t table_id)
        : BaseEvent(id, time)
        , client_(client)
        , table_id_(table_id)
    { }

    void AcceptVisitor(std::shared_ptr<StatisticServiceInterface> visitor) override {
        visitor->HandleEvent(*this);
    }

    std::string GetString() const override {
        return time_.GetString() + " " + std::to_string(id_) + " " + client_ + " " + std::to_string(table_id_);
    }
};