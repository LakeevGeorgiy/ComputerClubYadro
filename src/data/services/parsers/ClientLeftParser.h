#pragma once

#include "../../../domain/services/EventParserInterface.h"
#include "../../../domain/models/ClientLeftEvent.h"

class ClientLeftParser : public EventParserInterface {
public:

    std::shared_ptr<BaseEvent> ParseEvent(std::string_view line) override;
};