#pragma once

#include "../../../domain/services/EventParserInterface.h"

class ClientWaitParser : public EventParserInterface {
public:

    std::shared_ptr<BaseEvent> ParseEvent(std::string_view line) override;
};