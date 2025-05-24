#include "ClubService.h"

#include <iostream>

ClubService::ClubService(
    std::shared_ptr<EventParserInterface> event_parser, 
    std::shared_ptr<StatisticServiceInterface> stat_service, 
    std::shared_ptr<InputRepositoryInterface> input_repository)
    : event_parser_(event_parser)
    , stat_service_(stat_service)
    , input_repository_(input_repository)
{ }

void ClubService::MakeReport() {
    while (input_repository_->HasNext()) {
        auto line = input_repository_->GetNextLine();
        std::cout << "cur line: " << line << "\n";
        auto event = event_parser_->ParseEvent(line);
        event->AcceptVisitor(stat_service_);
    }

    stat_service_->WriteTablesReport();
}
