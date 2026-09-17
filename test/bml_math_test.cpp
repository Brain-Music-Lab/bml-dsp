#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "bml-dsp/bml-math.h"
#include <string>
#include <sstream>

#include <iostream>

namespace BML
{
    namespace Math
    {
        TEST_CASE("Greatest common factor")
        {
            std::cout << __cplusplus << "\n";

            REQUIRE(findGcf(9, 18) == 9);
            REQUIRE(findGcf(4684, 11710) == 2342);
            REQUIRE(findGcf(11710, 4684) == 2342);
            REQUIRE(findGcf(7321, 4441) == 1);
            REQUIRE(findGcf(1112, 0) == 1112);
            REQUIRE(findGcf(0, 1112) == 1112);
        }

        TEST_CASE("Least common multiple")
        {
            REQUIRE(findLcm(12, 15) == 60);
            REQUIRE(findLcm(12, 11) == 132);
            REQUIRE(findLcm(124, 14231) == 1764644);
            REQUIRE(findLcm(8121, 2) == 16242);
        }

        TEST_CASE("Arange")
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

    }
}
