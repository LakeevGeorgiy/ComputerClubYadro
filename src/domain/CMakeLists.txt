project(DomainLayer)

add_library(${PROJECT_NAME}
    services/StatisticServiceInterface.h
    models/BaseEvent.h 
    models/ClientArrivalEvent.h 
    models/ClientLeftEvent.h 
    models/ClientTookTableEvent.h 
    models/ClientWaitEvent.h 
    models/Time.h 
    models/Time.cpp
    models/InputHeaderData.h
    repositories/InputRepositoryInterface.h 
    repositories/OutputRepositoryInterface.h 
    services/ClubServiceInterface.h 
    services/EventParserInterface.h 
    services/EventParserInterface.cpp
    services/EventServiceInterface.h 
)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)