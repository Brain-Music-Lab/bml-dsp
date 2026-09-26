#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "bml-dsp/realtime.h"
#include "bml-dsp/util/csv.h"

namespace BML
{

// TEST_CASE("Toy case")
// {
//     // .0rom https://blog.robertelder.org/overlap-add-overlap-save/
//     std::vector<double> x({3.0, -1.0, 0.0, 3.0, 2.0, 0.0, 1.0, 2.0, 1.0});
//     std::vector<double> h({1.0, -1.0, 1.0});

//     std::vector<double> truth({3.0, -4.0, 4.0, 2.0, -1.0, 1.0, 3.0, 1.0, 0.0, 1.0, 1.0});

//     std::vector<double> x0(x.begin(), x.begin() + 3);
//     std::vector<double> x1(x.begin() + 3, x.begin() + 6);
//     std::vector<double> x2(x.begin() + 6, x.begin() + 9);
//     std::vector<double> x3({0.0, 0.0, 0.0});

//     RealTime::Convolution conv(h);
//     auto y0 = conv(x0);
//     auto y1 = conv(x1);
//     auto y2 = conv(x2);
//     auto y3 = conv(x3);

//     std::vector<double> y;
//     y.reserve(12);
//     y.insert(y.end(), y0.begin(), y0.end());
//     y.insert(y.end(), y1.begin(), y1.end());
//     y.insert(y.end(), y2.begin(), y2.end());
//     y.insert(y.end(), y3.begin(), y3.end());

//     size_t N = x.size() + h.size() - 1;
//     REQUIRE(y.size() >= N);
//     for (size_t i = 0; i < N; i++)
//     {
//         REQUIRE_THAT(y[i], Catch::Matchers::WithinAbs(truth[i], 0.00001));
//     }

// }


TEST_CASE("Convolution Testing")
{
    // Get python truth
    // y1
    std::stringstream ss;
    std::filesystem::path currentPath(__FILE__);
    ss << currentPath.parent_path().string() << "/ground_truth/data/convolution_test_x.csv";
    std::vector<double> x = BML::readOneLineCSV(ss.str());

    // y2
    ss.str("");
    ss << currentPath.parent_path().string() << "/ground_truth/data/convolution_test_h.csv";
    std::vector<double> h = BML::readOneLineCSV(ss.str());

    // y_conv
    ss.str("");
    ss << currentPath.parent_path().string() << "/ground_truth/data/convolution_test_y.csv";
    std::vector<double> yTruth = BML::readOneLineCSV(ss.str());

    std::vector<std::vector<double>> xBlocks;

    // Assuming a sample rate of 512 and a 2 second signal --> 1024 samples
    size_t N = x.size() + h.size() - 1;
    size_t B = 128;
    size_t idx = 0;

    while (idx <= N)
    {
        std::vector<double> test;
        for (size_t i = 0; i < B; i++)
        {
            if (i + idx >= x.size())
                break;
            
            test.push_back(x.at(i + idx));

        }

        if (test.size() < B)
        {
            size_t testSize = test.size();
            for (size_t i = testSize; i < B; i++)
            {
                test.push_back(0.0);
            }
        }

        xBlocks.push_back(test);
        idx += B;
    }

    size_t tracker = 0;
    for (size_t i = 0; i < xBlocks.size(); i++)
    {
        REQUIRE(xBlocks[i].size() == B);  // ensure size is correct

        for (size_t n = 0; n < B; n++)
        {
            REQUIRE(tracker == n + B * i);
            if (n + B * i >= x.size())
            {
                REQUIRE_THAT(xBlocks.at(i).at(n), Catch::Matchers::WithinAbs(0.0, 0.0001));
            }
            else 
            {
                REQUIRE_THAT(xBlocks.at(i).at(n), Catch::Matchers::WithinAbs(x.at(n + B * i), 0.0001));
            }

            tracker += 1;
        }
    }

    RealTime::Convolution conv(h);

    std::vector<std::vector<double>> resultsMat;
    std::vector<double> outVec;

    for (size_t i = 0; i < xBlocks.size(); i++)
    {
        resultsMat.push_back(conv(xBlocks[i]));
    }

    outVec.reserve(B * resultsMat.size());
    for (size_t i = 0; i < resultsMat.size(); i++)
    {
        outVec.insert(outVec.end(), resultsMat[i].begin(), resultsMat[i].end());
    }

    for (size_t i = 0; i < N; i++)
    {
        REQUIRE_THAT(outVec[i], Catch::Matchers::WithinAbs(yTruth[i], 1.0));
    }
}

}  // namespace BML
