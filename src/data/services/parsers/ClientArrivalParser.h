#pragma once

#include "../../../domain/models/Time.h"
#include "../../../domain/services/EventParserInterface.h"
#include "../../../domain/models/ClientArrivalEvent.h"

class ClientArrivalParser : public EventParserInterface {
public:

    std::shared_ptr<BaseEvent> ParseEvent(std::string_view line) override;
    
};