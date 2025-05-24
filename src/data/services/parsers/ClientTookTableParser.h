#pragma once

#include "../../../domain/services/EventParserInterface.h"
#include "../../../domain/models/ClientTookTableEvent.h"

class ClientTookTableParser : public EventParserInterface {
public:

    std::shared_ptr<BaseEvent> ParseEvent(std::string_view line) override;
    
};