#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <sstream>
#include <fstream>
#include <filesystem>
#include <iostream>

#include "bml-dsp/util/csv.h"
#include "bml-dsp/fft.h"
#include "bml-dsp/bml-math.h"
#include <cmath>

const double PI = 2.0 * std::acos(0.0);

TEST_CASE("Testing FFT based off of Scipy.fft.fft ground truth")
{

    // Get Python truth
    std::stringstream ss;
    std::filesystem::path currentPath(__FILE__);
    ss << currentPath.parent_path().string() << "/ground-truth/fft_test.csv";
    std::vector<double> truth = BML::readOneLineCSV(ss.str());

    // Replicate with C++
    std::vector<double> t = BML::Math::arange(0.0, 3.0, 1.0 / 22050.0);
    double f0 = 250.0; 
    double f1 = 500.0;
    double f2 = 1000.0;

    std::vector<double> y;
    y.reserve(t.size());
    for (size_t i = 0; i < t.size(); i++)
    {
        y.emplace_back(
            std::sin(f0 * 2.0 * PI * t[i]) + std::sin(f1 * 2.0 * PI * t[i]) + std::sin(f2 * 2.0 * PI * t[i]));
    }

    auto output = BML::FFT::fft(y);
    REQUIRE(output.size() == truth.size());
    for (size_t i = 0; i < 1000; i++)
    {
        // std::cout << output.size() << "\n";
        REQUIRE_THAT(std::abs(output[i]), Catch::Matchers::WithinAbs(truth[i], 0.0001));
    }

}