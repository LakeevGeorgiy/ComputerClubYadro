#include "InputRepository.h"

#include <regex>

InputRepository::InputRepository(const std::string &path)
    : file_(path)
{ }

InputRepository::InputRepository(InputRepository&& other) {
    if (this == &other) {
        return;
    }
    file_= std::move(other.file_);
    other.file_.close();
}

InputRepository &InputRepository::operator=(InputRepository&& other) {
    if (this == &other) {
        return *this;
    }
    file_= std::move(other.file_);
    other.file_.close();
    return *this;
}

InputRepository::~InputRepository() {
    file_.close();
}

bool InputRepository::CheckHeader(const std::string& count_tables, const std::string& time, const std::string& cost) {
    
    const std::string kCountTables = "^[\\s]*[\\d]{1,9}[\\s]*$";
    const std::string kStartEndTime = "^[\\s]*[\\d]{2}:[\\d]{2} [\\d]{2}:[\\d]{2}[\\s]*$";
    const std::string kHourCost = "^[\\s]*[\\d]{1,9}[\\s]*$";

    std::regex count_regex(kCountTables);
    if (std::regex_match(count_tables, count_regex)) {
        throw std::invalid_argument("First row must contain exactly one integer value");
    }

    std::regex time_regex(kStartEndTime);
    if (std::regex_match(time, time_regex)) {
        throw std::invalid_argument("Second row must contain exactly two timestamp values");
    }

    std::regex cost_regex(kHourCost);
    if (std::regex_match(cost, cost_regex)) {
        throw std::invalid_argument("Third row must contain exactly one integer value");
    }

    return true;

}

InputHeaderData InputRepository::ParseHeader(const std::string &count_tables, const std::string &time, const std::string &cost){
    std::istringstream count_stream { count_tables };
    uint32_t tables;
    count_stream >> tables;

    std::istringstream time_stream { time };
    std::string start_time;
    std::string end_time;
    time_stream >> start_time >> end_time;

    std::istringstream cost_stream { cost };
    uint32_t hour_cost;
    cost_stream >> hour_cost;

    return InputHeaderData(tables, Time(start_time), Time(end_time), hour_cost);
}

InputHeaderData InputRepository::ReadHeader() {
    file_.clear();
    file_.seekg(0, std::ios::beg);

    std::string count;
    getline(file_, count);

    if (!HasNext()) {
        std::runtime_error("Input file is closed unexpectedly");
    }
    std::string time;
    getline(file_, time);

    if (!HasNext()) {
        std::runtime_error("Input file is closed unexpectedly");
    }
    std::string cost;
    getline(file_, cost);
    CheckHeader(count, time, cost);
    return ParseHeader(count, time, cost);
}

bool InputRepository::HasNext()
{
    return file_.good();
}

std::string InputRepository::GetNextLine() {
    std::string line;
    getline(file_, line);
    return line;
}