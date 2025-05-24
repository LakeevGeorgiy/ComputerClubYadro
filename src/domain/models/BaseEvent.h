#pragma once 

#include <string>
#include <cstdint>
#include <memory>

#include "Time.h"
#include "../services/StatisticServiceInterface.h"

class BaseEvent {
public:

    const uint32_t id_;
    const Time time_;

public:

    BaseEvent(uint32_t event_id, const Time& time)
        : id_(event_id)
        , time_(time)
    {}

    virtual void AcceptVisitor(std::shared_ptr<StatisticServiceInterface> visitor) = 0;
    virtual std::string GetString() const = 0;
    
    virtual ~BaseEvent() = default;

};