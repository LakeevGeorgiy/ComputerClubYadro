#pragma once

#include <memory>

#include "../models/BaseEvent.h"

class EventParserInterface {
protected:

    std::shared_ptr<EventParserInterface> next_;
    
public:
    
    virtual bool CheckIdEvent(uint32_t parser_id, std::string_view line);
    virtual std::shared_ptr<BaseEvent> ParseEvent(std::string_view line) = 0;
    virtual void SetNext(std::shared_ptr<EventParserInterface> next);

    virtual ~EventParserInterface() = default;

};