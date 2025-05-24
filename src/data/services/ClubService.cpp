#include "ClubService.h"

ClubService::ClubService(
    std::shared_ptr<EventParserInterface> event_parser, 
    std::shared_ptr<StatisticServiceInterface> stat_service, 
    std::shared_ptr<InputRepositoryInterface> input_repository, 
    std::shared_ptr<OutputRepositoryInterface> output_repository)
    : event_parser_(event_parser)
    , stat_service_(stat_service)
    , input_repository_(input_repository)
    , output_repository_(output_repository)
{ }

void ClubService::MakeReport() {
    while (input_repository_->HasNext()) {
        auto line = input_repository_->GetNextLine();
        auto event = event_parser_->ParseEvent(line);
        event->AcceptVisitor(stat_service_);
    }

    stat_service_->WriteTablesReport();
}
