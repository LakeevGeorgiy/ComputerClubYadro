#pragma once

#include "Time.h"

#include <stdexcept>

class InputHeaderData {
public:

    const uint32_t count_tables_;
    const Time start_time_;
    const Time end_time_;
    const uint32_t hour_cost_;

public:

    InputHeaderData(
        uint32_t count_tables, 
        const Time& start_time, 
        const Time& end_time, 
        uint32_t hour_cost
    )
        : count_tables_(count_tables)
        , start_time_(start_time)
        , end_time_(end_time)
        , hour_cost_(hour_cost)
    {
        if (end_time < start_time) {
            throw std::invalid_argument("Start time must be less than end time");
        }
    }
};