#include "ClientArrivalParser.h"

#include <regex>
#include <exception>

#include "../../../domain/models/ClientArrivalEvent.h"

std::shared_ptr<BaseEvent> ClientArrivalParser::ParseEvent(std::string_view line){

    const uint8_t kClientArrivalId = 1;
    if (!CheckIdEvent(kClientArrivalId, line)){
        if (next_ == nullptr) {
            throw std::invalid_argument("This event is unknown");
        }
        return next_->ParseEvent(line);
    }

    std::string copy_line(line);
    const std::string kTimeRegex = "^[\\d]{2}:[\\d]{2}";
    const std::string kIdRegex = "[\\d]{1,9}";
    const std::string kClientRegex = "[\\w]+";

    const uint8_t kTimeLength = 5;

    std::regex regex(kTimeRegex + " " + kIdRegex + " " + kClientRegex);
    if (!std::regex_match(copy_line, regex)) {
        throw std::invalid_argument("Client arrival event isn't formed correct");
    }

    uint32_t event_id;
    std::string time;
    std::string client;
    
    std::istringstream stream { copy_line };
    stream >> time >> event_id >> client;
    
    return std::make_shared<ClientArrivalEvent>(event_id, Time(time), client);
}