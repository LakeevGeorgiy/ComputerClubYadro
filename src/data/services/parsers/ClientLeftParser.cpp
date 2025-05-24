#include "ClientLeftParser.h"

#include <regex>
#include <exception>
#include <sstream>

#include "../../../domain/models/Time.h"

std::shared_ptr<BaseEvent> ClientLeftParser::ParseEvent(std::string_view line){

    const uint8_t kClientLeft = 4;
    if (!CheckIdEvent(kClientLeft, line)){
        if (next_ == nullptr) {
            throw std::invalid_argument("This event is unknown");
        }
        return next_->ParseEvent(line);
    }

    std::string copy_line(line);
    const std::string kTimeRegex = "^[\\d]{2}:[\\d]{2}";
    const std::string kIdRegex = "[\\d]{1,9}";
    const std::string kClientRegex = "[a-z,0-9,_]+$";

    const uint8_t kTimeLength = 5;

    std::smatch match;
    std::regex regex(kTimeRegex + " " + kIdRegex + " " + kClientRegex);
    if (!std::regex_search(copy_line, match, regex)) {
        throw std::invalid_argument("Client arrival event isn't formed correct");
    }

    uint32_t event_id;
    std::string time;
    std::string client;

    std::istringstream stream { copy_line };
    stream >> time >> event_id >> client;
    return std::make_shared<ClientLeftEvent>(event_id, Time(time), client);
}