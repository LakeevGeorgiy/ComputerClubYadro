#include "ClientTookTableParser.h"

#include <exception>
#include <regex>

#include "../../../domain/models/Time.h"

std::shared_ptr<BaseEvent> ClientTookTableParser::ParseEvent(std::string_view line) {

    const uint8_t kTookTableId = 2;
    if (!CheckIdEvent(kTookTableId, line)){
        if (next_ == nullptr) {
            throw std::invalid_argument("This event is unknown");
        }
        return next_->ParseEvent(line);
    }

    std::string copy_line(line);
    const std::string kTimeRegex = "^[\\d]{2}:[\\d]{2}";
    const std::string kIdRegex = "[\\d]{1,9}";
    const std::string kClientRegex = "[a-z,0-9,_]+";

    std::regex regex(kTimeRegex + " " + kIdRegex + " " + kClientRegex + " " + kIdRegex + "$");
    if (!std::regex_match(copy_line, regex)) {
        throw std::invalid_argument("Client took table event isn't formed correct");
    }

    uint32_t event_id;
    std::string time;
    std::string client;
    uint32_t table_id;

    std::istringstream stream { copy_line };
    stream >> time >> event_id >> client >> table_id;

    if (table_id < 1) {
        throw std::invalid_argument("Table id must be greater than zero");
    }

    return std::make_shared<ClientTookTableEvent>(event_id, Time(time), client, table_id);
}