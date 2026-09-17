#include "util/csv.h"
#include <filesystem>

std::vector<double> BML::readOneLineCSV(std::string filepath)
{
    std::filesystem::path path(filepath);
    if (!std::filesystem::exists(path))
        throw std::runtime_error("CSV file does not exist.");

    std::fstream csvFile(filepath);
    std::vector<double> outVec;
    std::string value;

    while (std::getline(csvFile, value, ','))
        outVec.push_back(std::stod(value));

    return outVec;
}