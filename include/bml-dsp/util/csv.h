#ifndef BML_CSV_H
#define BML_CSV_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

namespace BML
{
    std::vector<double> readOneLineCSV(std::string filepath);
}

#endif  // BML_CSV_H