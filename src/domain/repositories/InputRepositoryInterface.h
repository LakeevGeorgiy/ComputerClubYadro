#pragma once

#include <string>

#include "../models/InputHeaderData.h"

class InputRepositoryInterface {
public:

    virtual InputHeaderData ReadHeader() = 0;
    virtual bool HasNext() = 0;
    virtual std::string GetNextLine() = 0;

    virtual ~InputRepositoryInterface() = default;
};