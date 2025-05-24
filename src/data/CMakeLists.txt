project(DataLayer)

add_library(${PROJECT_NAME}
    repositories/InputRepository.h 
    repositories/InputRepository.cpp 
    repositories/OutputRepository.h 
    repositories/OutputRepository.cpp 
    services/parsers/ClientArrivalParser.h 
    services/parsers/ClientArrivalParser.cpp 
    services/parsers/ClientLeftParser.h 
    services/parsers/ClientLeftParser.cpp
    services/parsers/ClientTookTableParser.h 
    services/parsers/ClientTookTableParser.cpp 
    services/parsers/ClientWaitParser.h 
    services/parsers/ClientWaitParser.cpp
    services/ClubService.h 
    services/ClubService.cpp 
    services/EventService.h 
    services/EventService.cpp 
    services/StatisticService.h 
    services/StatisticService.cpp 
    services/CommandLineParser.h 
    services/CommandLineParser.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC DomainLayer)