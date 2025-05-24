#pragma once

class ClientArrivalEvent;
class ClientTookTableEvent;
class ClientWaitEvent;
class ClientLeftEvent;
class ErrorEvent;

#include "../repositories/OutputRepositoryInterface.h"


class StatisticServiceInterface {
public:

    virtual void HandleEvent(ClientArrivalEvent& event) = 0;
    virtual void HandleEvent(ClientTookTableEvent& event) = 0;
    virtual void HandleEvent(ClientWaitEvent& event) = 0;
    virtual void HandleEvent(ClientLeftEvent& event) = 0;
    virtual void HandleEvent(ErrorEvent& event) = 0;
    virtual void WriteTablesReport() = 0;
    
    virtual ~StatisticServiceInterface() = default;
};