#pragma once

#include <string>
#include <cstdint>

#include "../../domain/services/CommandLineParserInterface.h"

class CommandLineParser : public CommandLineParserInterface {
private:

    constexpr static uint8_t kNumberOfFlags = 2;
    constexpr static uint8_t kNumberOfValues = 2;

    Files files_;

public:

    CommandLineParser();
    
    
    Files ParseCommandLine(int argc, char* argv[]) override;

private:

    std::string GetAbsolutePath(std::string_view path);
    bool IsValidPath(std::string_view path);
    void SetValue(std::string_view flag, std::string_view value);
};