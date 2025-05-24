#include "OutputRepository.h"

#include <iostream>

OutputRepository::OutputRepository(const std::string& path)
    : file_(path)
{}

OutputRepository::OutputRepository(OutputRepository &&other)
    : file_(std::move(other.file_))
{ }

OutputRepository &OutputRepository::operator=(OutputRepository &&other) {
    if (this == &other) {
        return *this;
    }
    file_ = std::move(other.file_);
    return *this;
}

OutputRepository::~OutputRepository() {
    file_.close();
}

void OutputRepository::WriteLine(std::string_view line) {
    file_ << line << "\n";
}