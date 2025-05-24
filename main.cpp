#include <iostream>
#include <regex>

#include "src/data/services/ClubService.h"
#include "src/data/services/StatisticService.h"
#include "src/data/repositories/InputRepository.h"
#include "src/data/repositories/OutputRepository.h"
#include "src/data/services/parsers/ClientArrivalParser.h"
#include "src/data/services/parsers/ClientLeftParser.h"
#include "src/data/services/parsers/ClientTookTableParser.h"
#include "src/data/services/parsers/ClientWaitParser.h"

std::shared_ptr<EventParserInterface> ParserChainFactory() {

    auto client_arrival_parser = std::make_shared<ClientArrivalParser>();
    auto client_took_parser = std::make_shared<ClientTookTableParser>();
    auto client_wait_parser = std::make_shared<ClientWaitParser>();
    auto client_left_parser = std::make_shared<ClientLeftParser>();

    client_arrival_parser->SetNext(client_took_parser);
    client_took_parser->SetNext(client_wait_parser);
    client_wait_parser->SetNext(client_left_parser);
    
    return client_arrival_parser;
}

int main() {

    auto input_repository = std::make_shared<InputRepository>("../files/input.txt");
    auto header_data = input_repository->ReadHeader();
    auto parser_chain = ParserChainFactory();
    auto output_repository = std::make_shared<OutputRepository>("../files/output.txt");
    auto stat_service = std::make_shared<StatisticService>(header_data, output_repository);

    std::shared_ptr<ClubServiceInterface> club_service = std::make_shared<ClubService>(
        parser_chain,
        stat_service,
        input_repository
    );
    club_service->MakeReport();

    return 0;
}