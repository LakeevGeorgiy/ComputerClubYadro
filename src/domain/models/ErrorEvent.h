#pragma once

#include "BaseEvent.h"

class ErrorEvent : public BaseEvent {
public:

    const std::string error_;
    
public:

    ErrorEvent(uint32_t id, const Time& time, std::string_view error)
        : BaseEvent(id, time)
        , error_(error)
    {}

    void AcceptVisitor(std::shared_ptr<StatisticServiceInterface> visitor) override {
        visitor->HandleEvent(*this);
    }

    std::string GetString() const override {
        return time_.GetString() + " " + std::to_string(id_) + " " + error_;
    }
};