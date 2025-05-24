#pragma once 

#include <string>

struct Files {
    
    std::string input_file_;
    std::string output_file_;

    Files() = default;
    Files(std::string_view input, std::string_view output)
        : input_file_(input)
        , output_file_(output)
    {}
};

class CommandLineParserInterface {
public:

    virtual Files ParseCommandLine(int argc, char* argv[]) = 0;

    virtual ~CommandLineParserInterface() = default;
};