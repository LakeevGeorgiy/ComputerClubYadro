#pragma once

#include "BaseEvent.h"

class ClientArrivalEvent : public BaseEvent {
public:

    const std::string client_;
    
public:

    ClientArrivalEvent(uint32_t id, const Time& time, std::string_view client)
        : BaseEvent(id, time)
        , client_(client)
    {}

    void AcceptVisitor(std::shared_ptr<StatisticServiceInterface> visitor) override {
        visitor->HandleEvent(*this);
    }

    std::string GetString() const override {
        return time_.GetString() + " " + std::to_string(id_) + client_;
    }
};

