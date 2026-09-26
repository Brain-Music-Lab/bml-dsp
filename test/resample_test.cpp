#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "bml-dsp/realtime.h"
#include "bml-dsp/util/csv.h"

namespace BML { 


TEST_CASE("Toy example")
{
    /*
    After resampling, signal size M is 200. 

    sr_old = 100
    sr_new = 200
    
    transition bandwidth = 100/200 = 0.5
    Filter size L is equal to ceil(4 / 0.5) => 8 => 8 + 1 = 9 so it's odd

    Convolution length should be 200 + 9 - 1 = 208, so 8 zeros are sent through the system to get the final result

    The resampled signal is from L//2 to L//2 + M ==> 4 to 204
    */
    // Get python truth.
    std::stringstream ss;
    std::filesystem::path currentPath(__FILE__);
    ss << currentPath.parent_path().string() << "/ground_truth/data/resample_test0_y.csv";
    std::vector<double> y = BML::readOneLineCSV(ss.str());

    std::vector<double> endZeros(8, 0.0);

    ss.str("");
    ss << currentPath.parent_path().string() << "/ground_truth/data/resample_test0_y_truth.csv";
    std::vector<double> yTruth = BML::readOneLineCSV(ss.str());

    RealTime::Resample resample(100.0, 200.0);
    auto resampY = resample(y);
    auto end = resample(endZeros);

    std::vector<double> full = resampY;
    for (size_t i = 0; i < end.size(); i++)
    {
        full.push_back(end[i]);
    }

    std::vector resampSig(full.begin() + 4, full.begin() + 204);

    REQUIRE(resampSig.size() == yTruth.size());
    for (size_t i = 0; i < yTruth.size(); i++)
    {
        REQUIRE_THAT(resampSig[i], Catch::Matchers::WithinAbs(yTruth[i], 0.01));
    }
}

TEST_CASE("More involved upsampling")
{
    /*
    original signal is 1024
    After upsampling by 4, signal size M is 4096. 

    sr_old = 512
    sr_new = 2048
    
    transition bandwidth = 512/2048 = 0.25
    Filter size L is equal to ceil(4 / 0.25) => 16 => 16 + 1 = 17 so it's odd

    Convolution length should be 4096 + 17 - 1 = 4112, so 16 zeros are sent through the system to get the final result

    The resampled signal is from L//2 to L//2 + M ==> 8 to 4104
    */
    // Get python truth.
    std::stringstream ss;
    std::filesystem::path currentPath(__FILE__);
    ss << currentPath.parent_path().string() << "/ground_truth/data/resample_test1_y.csv";
    std::vector<double> y = BML::readOneLineCSV(ss.str());

    std::vector<double> endZeros(16, 0.0);

    ss.str("");
    ss << currentPath.parent_path().string() << "/ground_truth/data/resample_test1_y_truth.csv";
    std::vector<double> yTruth = BML::readOneLineCSV(ss.str());

    RealTime::Resample resample(512.0, 2048.0);
    auto resampY = resample(y);
    auto end = resample(endZeros);

    std::vector<double> full = resampY;
    for (size_t i = 0; i < end.size(); i++)
    {
        full.push_back(end[i]);
    }

    std::vector resampSig(full.begin() + 8, full.begin() + 4104);

    REQUIRE(resampSig.size() == yTruth.size());
    for (size_t i = 0; i < yTruth.size(); i++)
    {
        REQUIRE_THAT(resampSig[i], Catch::Matchers::WithinAbs(yTruth[i], 0.0019));
    }

    std::cout << "\n";
}

} // namespace BML