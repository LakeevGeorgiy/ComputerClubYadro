#include <stdexcept>
#include <filesystem>
#include <iostream>

#include "CommandLineParser.h"

namespace fs = std::filesystem;

CommandLineParser::CommandLineParser(): files_() {}

Files CommandLineParser::ParseCommandLine(int argc, char *argv[])
{

    if (argc > kNumberOfFlags + kNumberOfValues + 1) {
        throw std::invalid_argument("Too many arguments");
    }

    for (uint8_t i = 1; i < argc; ++i) {

        std::string_view arg = argv[i];
        if (arg[0] != '-') {
            continue;
        }

        if (i + 1 >= argc) {
            throw std::invalid_argument("Passing flag without value");
        }

        std::string_view value = argv[i + 1];
        SetValue(arg, value);
        ++i;

    }
    return files_;
}

std::string CommandLineParser::GetAbsolutePath(std::string_view path) {
    return fs::absolute(path);
}

bool CommandLineParser::IsValidPath(std::string_view path) {

    if (fs::exists(path) && fs::is_regular_file(path)) {
        return true;
    }
    return false;
}

void CommandLineParser::SetValue(std::string_view flag, std::string_view value) {

    auto absolute_path = GetAbsolutePath(value);

    if (!IsValidPath(value)) {
        throw std::runtime_error("File doesn't exist: " + absolute_path);
    }

    if (flag == "-i" || flag == "--input-file") {
        files_.input_file_ = absolute_path;
    } else if (flag == "-o" || flag == "--output-file") {
        files_.output_file_ = absolute_path;
    } else {
        const std::string unsupported_flag = flag.data();
        throw std::invalid_argument("This flag isn't support " + unsupported_flag);
    }
}