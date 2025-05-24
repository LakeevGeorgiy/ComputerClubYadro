#pragma once

#include <memory>
#include <iostream>

#include "../../domain/services/ClubServiceInterface.h"
#include "../../domain/services/StatisticServiceInterface.h"
#include "../../domain/services/EventServiceInterface.h"
#include "../../domain/services/EventParserInterface.h"
#include "../../domain/repositories/InputRepositoryInterface.h"
#include "../../domain/repositories/OutputRepositoryInterface.h"
#include "../../domain/models/InputHeaderData.h"

class ClubService : public ClubServiceInterface {
private:

    std::shared_ptr<EventParserInterface> event_parser_;
    std::shared_ptr<StatisticServiceInterface> stat_service_;
    std::shared_ptr<InputRepositoryInterface> input_repository_;

public:

    ClubService(
        std::shared_ptr<EventParserInterface> event_parser,
        std::shared_ptr<StatisticServiceInterface> stat_service,
        std::shared_ptr<InputRepositoryInterface> input_repository
    );

    void MakeReport() override;

};