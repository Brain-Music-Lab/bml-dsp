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

TEST_CASE("Testing FFT")
{
    SECTION("Testing for equivalence with scipy.fft.fft ground truth")
    {
        // Get Python truth for real
        std::stringstream ss;
        std::filesystem::path currentPath(__FILE__);
        ss << currentPath.parent_path().string() << "/ground_truth/fft_test_real.csv";
        std::vector<double> realTruth = BML::readOneLineCSV(ss.str());

        // and for imaginary
        ss.str("");
        ss << currentPath.parent_path().string() << "/ground_truth/fft_test_imag.csv";
        std::vector<double> imagTruth = BML::readOneLineCSV(ss.str());

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
        REQUIRE(output.size() == realTruth.size());
        for (size_t i = 0; i < 1000; i++)
        {
            REQUIRE_THAT(output[i].real(), Catch::Matchers::WithinAbs(realTruth[i], 0.0001));
            REQUIRE_THAT(output[i].imag(), Catch::Matchers::WithinAbs(imagTruth[i], 0.0001));
        }
    }

    SECTION("Catch runtime error when array size is 0.")
    {
        std::vector<double> y;
        REQUIRE(y.size() == 0);

        try
        {
            auto output = BML::FFT::fft(y);
        }
        catch (const std::runtime_error& e) 
        {
            REQUIRE(true);
            return;
        }
        // failure. error not caught
        REQUIRE(false);
    }
}

TEST_CASE("Testing IFFT")
{
    SECTION("Testing against scipy ground truth")
    {
        // Get Python truth for real
        std::stringstream ss;
        std::filesystem::path currentPath(__FILE__);
        ss << currentPath.parent_path().string() << "/ground_truth/ifft_test_real.csv";
        std::vector<double> realTruth = BML::readOneLineCSV(ss.str());

        // and for imaginary
        ss.str("");
        ss << currentPath.parent_path().string() << "/ground_truth/ifft_test_imag.csv";
        std::vector<double> imagTruth = BML::readOneLineCSV(ss.str());

        // Replicate with c++
        std::vector<double> t = BML::Math::arange(0.0, 2.0, 1.0/44100.0);
        double f0 = 40;
        double f1 = 103;
        double f2 = 724;

        std::vector<double> y;
        y.reserve(t.size());
        double val;
        for (size_t i = 0; i < t.size(); i++)
        {
            val = 0.0;
            val += std::sin(f0 * 2.0 * PI * t[i]);
            val += 0.8 * std::sin(f1 * 2.0 * PI * t[i]);
            val += 0.5 * std::sin(f2 * 2.0 * PI * t[i]);
            y.emplace_back(val);
        }

        auto complexOut = BML::FFT::fft(y);
        auto output = BML::FFT::ifft(complexOut);

        REQUIRE(output.size() == realTruth.size());
        for (size_t i = 0; i < 1000; i++)
        {
            REQUIRE_THAT(output[i].real(), Catch::Matchers::WithinAbs(realTruth[i], 0.0001));
            REQUIRE_THAT(output[i].imag(), Catch::Matchers::WithinAbs(imagTruth[i], 0.0001));
        }
    }

    SECTION("Catch runtime error when array size is 0.")
    {
        std::vector<std::complex<double>> y;
        REQUIRE(y.size() == 0);

        try
        {
            auto output = BML::FFT::ifft(y);
        }
        catch (const std::runtime_error& e) 
        {
            REQUIRE(true);
            return;
        }
        // failure. error not caught
        REQUIRE(false);
    }
}