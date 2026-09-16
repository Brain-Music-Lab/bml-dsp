#include "util/csv.h"


std::vector<double> BML::readOneLineCSV(std::string filepath)
{
    std::fstream csvFile(filepath);
    std::vector<double> outVec;
    std::string value;

    while (std::getline(csvFile, value, ','))
        outVec.push_back(std::stod(value));

    return outVec;
}