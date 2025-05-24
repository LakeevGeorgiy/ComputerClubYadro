#include "EventParserInterface.h"

#include <regex>

bool EventParserInterface::CheckIdEvent(uint32_t parser_id, std::string_view line) {
    std::string copy_line(line);
    const uint8_t kLengthOfTimeAndSpace = 6;
    const std::string kIdRegex = "[\\d]{1,9}";
    copy_line = copy_line.substr(kLengthOfTimeAndSpace);

    std::regex id_regex(kIdRegex);
    std::smatch match;
    if (!std::regex_search(copy_line, match, id_regex)) {
        throw std::invalid_argument("There is no event's id or it's number too long");
    }
    
    std::string number = match[0];
    return std::stoi(number) == parser_id;
}

void EventParserInterface::SetNext(std::shared_ptr<EventParserInterface> next){
    next_ = next;
}