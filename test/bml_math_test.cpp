#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "bml-dsp/util/bml-math.h"
#include "bml-dsp/util/csv.h"

#include <string>
#include <sstream>

#include <iostream>
#include <csignal>



namespace BML
{
    namespace Math
    {
        TEST_CASE("findGcf")
        {
            REQUIRE(findGcf(9, 18) == 9);
            REQUIRE(findGcf(4684, 11710) == 2342);
            REQUIRE(findGcf(11710, 4684) == 2342);
            REQUIRE(findGcf(7321, 4441) == 1);
            REQUIRE(findGcf(1112, 0) == 1112);
            REQUIRE(findGcf(0, 1112) == 1112);
        }

        TEST_CASE("findLcm")
        {
            REQUIRE(findLcm(12, 15) == 60);
            REQUIRE(findLcm(12, 11) == 132);
            REQUIRE(findLcm(124, 14231) == 1764644);
            REQUIRE(findLcm(8121, 2) == 16242);
        }

        TEST_CASE("arange")
        {
            REQUIRE(
                std::vector<double>(
                    {0.0, 0.5, 1.0, 1.5, 
                2.0, 2.5, 3.0, 3.5, 4.0, 
                4.5, 5.0, 5.5, 6.0, 6.5, 
                7.0, 7.5, 8.0, 8.5, 9.0, 9.5}) == arange(0, 10, 0.5));
            std::vector<double> output({
                1.3,   3.17,   5.04,   6.91,   8.78,  10.65,  12.52,  14.39,
                16.26,  18.13,  20.0,  21.87,  23.74,  25.61,  27.48,  29.35,
                31.22,  33.09,  34.96,  36.83,  38.7,  40.57,  42.44,  44.31,
                46.18,  48.05,  49.92,  51.79,  53.66,  55.53,  57.4,  59.27,
                61.14,  63.01,  64.88,  66.75,  68.62,  70.49,  72.36,  74.23,
                76.1,  77.97,  79.84,  81.71,  83.58,  85.45,  87.32,  89.19,
                91.06,  92.93,  94.8,  96.67,  98.54, 100.41, 102.28, 104.15,
                106.02, 107.89, 109.76, 111.63, 113.5, 115.37, 117.24, 119.11,
                120.98, 122.85
            });
            
            std::vector<double> from_arange = arange(1.3, 123.2, 1.87);
            REQUIRE(output.size() == from_arange.size());
            for (size_t i = 0; i < output.size(); i++)
            {
                REQUIRE_THAT(output[i], Catch::Matchers::WithinAbs(from_arange[i], 0.0001));
            }

            output = std::vector<double>({3.0, 3.92,  4.84,  5.76,  6.68,  7.6 ,  8.52,  9.44, 10.36,
                11.28, 12.2 , 13.12, 14.04, 14.96, 15.88, 16.8});
            from_arange = arange(3, 17, 0.92);
            REQUIRE(output.size() == from_arange.size());
            for (size_t i = 0; i < output.size(); i++)
            {
                REQUIRE_THAT(output[i], Catch::Matchers::WithinAbs(from_arange[i], 0.0001));
            }
        }

        TEST_CASE("sinc")
        {
            // Current path
            std::filesystem::path currentPath(__FILE__);

            // Read in all python ground truth
            std::stringstream ss;
            ss << currentPath.parent_path().string() << "/ground_truth/data/sinc_test.csv";
            std::vector<double> sincTruth = BML::readOneLineCSV(ss.str());

            auto t = arange(-50.0, 50.0, 0.1);
            auto sincVals = sinc(t);

            REQUIRE(sincVals.size() == sincTruth.size());
            for (size_t i = 0; i < sincVals.size(); i++)
            {
                REQUIRE_THAT(sincVals[i], Catch::Matchers::WithinAbs(sincTruth[i], 0.0001));
            }

            REQUIRE(sinc(0.0) == 1.0);
        }

        TEST_CASE("blackman")
        {
            // Current path
            std::filesystem::path currentPath(__FILE__);

            // Read in all python ground truth
            std::stringstream ss;
            ss << currentPath.parent_path().string() << "/ground_truth/data/blackman_window_test.csv";
            std::vector<double> windowTruth = BML::readOneLineCSV(ss.str());

            auto window = blackman(200);

            REQUIRE(window.size() == windowTruth.size());
            for (size_t i = 0; i < window.size(); i++)
            {
                REQUIRE_THAT(window[i], Catch::Matchers::WithinAbs(windowTruth[i], 0.0001));
            }
        }

        TEST_CASE("firstGreaterThan")
        {
            // First scenario
            double val = 10.3;
            std::vector<double> arr({1.4, 2.6, 1.1, 9.8, 20.5, 60.2, 1.6, 20.1, 206.1});
            REQUIRE(firstGreaterThan(val, arr) == 4);

            // Second scenario
            val = 0.0;
            arr = std::vector<double>({-3.8, -2.0, -10.3, 1.2, -105.9, -1.0});
            REQUIRE(firstGreaterThan(val, arr) == 3);

            // Third scenario (handle when the value is in the array -- not less than, so not chosen)
            val = 12.0;
            arr = std::vector<double>({10.2, 12.0, 103.2, 101.8, 4.0, 104.3, 20.7});
            REQUIRE(firstGreaterThan(val, arr) == 2);

            // Fourth scenario (first value greater than)
            val = 4.0;
            arr = std::vector<double>({5.6, 105.1, 1.9, 0.111, 3.8});
            REQUIRE(firstGreaterThan(val, arr) == 0);

            // Fifth scenario (No value greater than -- returns size of array)
            val = 100.0;
            arr = std::vector<double>({1.20, 1.23, 90.1, 100.0, 91.9});
            REQUIRE(firstGreaterThan(val, arr) == 5);
        }

        TEST_CASE("lerp")
        {
            // FROM PYTHON TRUTH GENERATION
            const double FS = 512.0;
            const double N_SECS = 3.0;

            // Current path
            std::filesystem::path currentPath(__FILE__);

            // Read in all python ground truth
            std::stringstream ss;
            ss << currentPath.parent_path().string() << "/ground_truth/data/lerp_test_fake_timestamps.csv";
            std::vector<double> timestamps = BML::readOneLineCSV(ss.str());

            ss.str("");
            ss << currentPath.parent_path().string() << "/ground_truth/data/lerp_test_fake_out_data.csv";
            std::vector<double> data = BML::readOneLineCSV(ss.str());

            ss.str("");
            ss << currentPath.parent_path().string() << "/ground_truth/data/lerp_test_fake_interp_data.csv";
            std::vector<double> interpTruth = BML::readOneLineCSV(ss.str());

            // Interpolate the raw data
            std::vector<double> t = arange(0.0, N_SECS, 1.0/FS);
            std::vector<double> interpY;

            double val;
            double x1;
            double y1;
            double x2;
            double y2;

            for (size_t i = 0; i < t.size(); i++)
            {
                val = t[i];
                double firstTime = timestamps[0];
                double lastTime = timestamps[timestamps.size() - 1];

                if (val < firstTime)
                {
                    interpY.push_back(data[0]);
                    continue;
                }

                else if (val > lastTime)
                {
                    interpY.push_back(data[data.size()-1]);
                    continue;
                }
                
                size_t idx = firstGreaterThan(val, timestamps);
                x1 = timestamps[idx-1];
                x2 = timestamps[idx];
                y1 = data[idx-1];
                y2 = data[idx];

                interpY.push_back(lerp(val, x1, y1, x2, y2));
            }

            REQUIRE(interpY.size() == interpTruth.size());
            for (size_t i = 0; i < 10; i++)
            {
                REQUIRE_THAT(interpY[i], Catch::Matchers::WithinAbs(interpTruth[i], 0.0001));
            }
        }

        // TEST_CASE("mean")
        // {
        //     REQUIRE(false);
        // }

        // TEST_CASE("normalize")
        // {
        //     REQUIRE(false);
        // }

        TEST_CASE("genSineWave")
        {
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

            std::vector<double> yOther = BML::Math::genSineWave({1.0, 1.0, 1.0}, {f0, f1, f2}, t);

            REQUIRE(y.size() == yOther.size());
            for (size_t i = 0; i < y.size(); i++)
            {
                REQUIRE_THAT(y[i], Catch::Matchers::WithinAbs(yOther[i], 0.0001));
            }
        }
    }
}
