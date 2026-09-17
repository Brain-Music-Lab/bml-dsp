#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <iostream>
#include <sstream>

#include "bml-dsp/util/csv.h"
#include <vector>
#include <filesystem>
#include <exception>


TEST_CASE("Reading a basic CSV file")
{
    std::stringstream ss;
    std::filesystem::path currentPath(__FILE__);
    ss << currentPath.parent_path().string() << "/ground_truth/read_csv_test.csv";
    std::vector<double> output = BML::readOneLineCSV(ss.str());   
    std::vector<double> truth({4.1, 105.2, 3.8, 105.14, 20659.1, 20.2, -92.1});

    REQUIRE(truth.size() == output.size());
    for (size_t i = 0; i < truth.size(); i++)
    {
        REQUIRE_THAT(output[i], Catch::Matchers::WithinAbs(truth[i], 0.0001));
    }
}

TEST_CASE("Catching an Error when the file doesn't exist")
{
    try
    {
        std::vector<double> output = BML::readOneLineCSV("this/file/does/not/exist.csv");
    }
    catch (const std::runtime_error& e)
    {
        return;  // test case passed
    }

    REQUIRE(false);  // test case failed
}