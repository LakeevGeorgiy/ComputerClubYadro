#pragma once

#include <fstream>

#include "../../domain/repositories/OutputRepositoryInterface.h"

class OutputRepository : public OutputRepositoryInterface {
private:

    std::ofstream file_;

public:

    OutputRepository(const std::string& path);
    OutputRepository(OutputRepository&& other);
    OutputRepository& operator=(OutputRepository&& other);
    ~OutputRepository();

    void WriteLine(std::string_view line) override;
};