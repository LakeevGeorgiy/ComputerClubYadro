#pragma once

#include <memory>
#include <string_view>

class OutputRepositoryInterface {
public:

    virtual void WriteLine(std::string_view line) = 0;
    virtual ~OutputRepositoryInterface() = default;
};