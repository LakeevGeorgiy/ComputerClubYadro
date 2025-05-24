#pragma once

#include <fstream>
#include <vector>

#include "../../domain/repositories/InputRepositoryInterface.h"

class InputRepository : public InputRepositoryInterface {
private:

    std::ifstream file_;

    bool CheckHeader(const std::string& count_tables, const std::string& time, const std::string& cost);
    InputHeaderData ParseHeader(const std::string& count_tables, const std::string& time, const std::string& cost);

public:

    InputRepository(const std::string& path);
    InputRepository(InputRepository&& other);
    InputRepository& operator=(InputRepository&& other);
    ~InputRepository();

    InputHeaderData ReadHeader() override;
    bool HasNext() override;
    std::string GetNextLine() override;
};