#pragma once

#include "../../../domain/services/EventParserInterface.h"
#include "../../../domain/models/ClientWaitEvent.h"

class ClientWaitParser : public EventParserInterface {
public:

    std::shared_ptr<BaseEvent> ParseEvent(std::string_view line) override;
};