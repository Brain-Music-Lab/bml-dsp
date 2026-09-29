#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "bml-dsp/realtime.h"
#include "bml-dsp/util/csv.h"

#include<cmath>
#include<numeric>
#include<algorithm>

namespace BML { 


TEST_CASE("Toy example")
{
    // Get python truth.
    std::stringstream ss;
    std::filesystem::path currentPath(__FILE__);
    ss << currentPath.parent_path().string() << "/ground_truth/data/resample_test0_y.csv";
    std::vector<double> y = BML::readOneLineCSV(ss.str());

    ss.str("");
    ss << currentPath.parent_path().string() << "/ground_truth/data/resample_test0_y_truth.csv";
    std::vector<double> yTruth = BML::readOneLineCSV(ss.str());

    RealTime::Resample resample(100.0, 200.0, 50.0);
    size_t numTaps = resample.Taps();
    auto resampY = resample(y);

    std::vector<double> endZeros(numTaps-1, 0.0);
    auto end = resample(endZeros);

    std::vector<double> full = resampY;
    for (size_t i = 0; i < end.size(); i++)
    {
        full.push_back(end[i]);
    }

    size_t startIdx = (numTaps-1)/2;
    size_t endIdx = full.size() - end.size() + (numTaps - 1) / 2;
    std::vector resampSig(full.begin() + startIdx, full.begin() + endIdx);

    REQUIRE(resampSig.size() == yTruth.size());
    
    // At 200hz, 4 samples is approximately 20 ms
    for (size_t i = 4; i < yTruth.size() - 4; i++)
    {
        REQUIRE_THAT(resampSig[i], Catch::Matchers::WithinAbs(yTruth[i], 0.001));
    }
}

TEST_CASE("More involved upsampling")
{
    // Get python truth.
    std::stringstream ss;
    std::filesystem::path currentPath(__FILE__);
    ss << currentPath.parent_path().string() << "/ground_truth/data/resample_test1_y.csv";
    std::vector<double> y = BML::readOneLineCSV(ss.str());

    ss.str("");
    ss << currentPath.parent_path().string() << "/ground_truth/data/resample_test1_y_truth.csv";
    std::vector<double> yTruth = BML::readOneLineCSV(ss.str());

    RealTime::Resample resample(512.0, 2048.0, 100.0);
    size_t numTaps = resample.Taps();
    std::vector<double> endZeros(numTaps-1, 0.0);  // Needed to get tail of convolution

    auto resampY = resample(y);
    auto end = resample(endZeros);

    std::vector<double> full = resampY;
    for (size_t i = 0; i < end.size(); i++)
    {
        full.push_back(end[i]);
    }

    size_t startIdx = (numTaps-1)/2;
    size_t endIdx = full.size() - end.size() + (numTaps - 1) / 2;
    std::vector resampSig(full.begin() + startIdx, full.begin() + endIdx);

    REQUIRE(resampSig.size() == yTruth.size());

    // At a sampling rate of 4096, 44 samples is approximately 10ms
    std::vector<double> errors(yTruth.size() - 88, 0.0);
    for (size_t i = 44; i < yTruth.size() - 44; i++)
    {
        REQUIRE_THAT(resampSig[i], Catch::Matchers::WithinAbs(yTruth[i], 0.1));  // Max error is 0.1
        errors[i-44] = resampSig[i] - yTruth[i];
    }

    std::vector<double> sqrError = errors;
    double mean = std::accumulate(errors.begin(), errors.end(), 0.0) / static_cast<double>(errors.size());

    std::sort(errors.begin(), errors.end());
    for (size_t i = 1; i < errors.size(); i++)  // Proof of being sorted for median calculation
    {
        REQUIRE(errors[i - 1] <= errors[i]);
    }

    size_t idx;
    if (errors.size() % 2 == 0)
    {
        idx = errors.size() / 2;
    }
    else
    {
        idx = (errors.size() - 1) / 2;
    }

    // Average and median errors are *much* less than max error.
    REQUIRE(mean < 0.00001);
    REQUIRE(errors[idx] < 0.00001);  // median
}

TEST_CASE("Downsampling")
{
    // Get python truth.
    std::stringstream ss;
    std::filesystem::path currentPath(__FILE__);
    ss << currentPath.parent_path().string() << "/ground_truth/data/resample_test2_y.csv";
    std::vector<double> y = BML::readOneLineCSV(ss.str());

    ss.str("");
    ss << currentPath.parent_path().string() << "/ground_truth/data/resample_test2_y_truth.csv";
    std::vector<double> yTruth = BML::readOneLineCSV(ss.str());

    RealTime::Resample resample(22050, 11025.0, 1.0);
    size_t numTaps = resample.Taps();
    std::vector<double> endZeros(numTaps-1, 0.0);  // Needed to get tail of convolution

    auto resampY = resample(y);
    auto end = resample(endZeros);

    std::vector<double> full = resampY;
    for (size_t i = 0; i < end.size(); i++)
    {
        full.push_back(end[i]);
    }

    // Extra division by two because of decimation by two. In effect, dividing filter size by 2
    size_t startIdx = ((numTaps-1) / 2) / 2;
    size_t endIdx = full.size() - end.size() + ((numTaps - 1) / 2) / 2;
    std::vector resampSig(full.begin() + startIdx, full.begin() + endIdx);

    REQUIRE(resampSig.size() == yTruth.size());

    // At a sampling rate of 11025, 44 samples is approximately 10ms
    std::vector<double> errors(yTruth.size() - 220, 0.0);
    for (size_t i = 110; i < yTruth.size() - 110; i++)
    {
        REQUIRE_THAT(resampSig[i], Catch::Matchers::WithinAbs(yTruth[i], 0.1));  // Max error is 0.1
        errors[i-110] = resampSig[i] - yTruth[i];
    }

    std::vector<double> sqrError = errors;
    double mean = std::accumulate(errors.begin(), errors.end(), 0.0) / static_cast<double>(errors.size());

    std::sort(errors.begin(), errors.end());
    for (size_t i = 1; i < errors.size(); i++)  // Proof of being sorted for median calculation
    {
        REQUIRE(errors[i - 1] <= errors[i]);
    }

    size_t idx;
    if (errors.size() % 2 == 0)
    {
        idx = errors.size() / 2;
    }
    else
    {
        idx = (errors.size() - 1) / 2;
    }

    // Average and median errors are *much* less than max error.
    REQUIRE(mean < 0.00001);
    REQUIRE(errors[idx] < 0.00001);  // median
}

TEST_CASE("Fractional resampling (512hz to 48000 hertz)")
{
    // Get python truth.
    std::stringstream ss;
    std::filesystem::path currentPath(__FILE__);
    ss << currentPath.parent_path().string() << "/ground_truth/data/resample_test3_y.csv";
    std::vector<double> y = BML::readOneLineCSV(ss.str());

    ss.str("");
    ss << currentPath.parent_path().string() << "/ground_truth/data/resample_test3_y_truth.csv";
    std::vector<double> yTruth = BML::readOneLineCSV(ss.str());

    RealTime::Resample resample(512.0, 48000.0, 300.0);
    auto resampY = resample(y);
    size_t numTaps = ((resample.Taps() - 1) / 4) + 1;

    std::vector<double> endZeros(numTaps-1, 0.0);  // Needed to get tail of convolution
    auto end = resample(endZeros);

    std::vector<double> full = resampY;
    for (size_t i = 0; i < end.size(); i++)
    {
        full.push_back(end[i]);
    }

    size_t startIdx = ((numTaps-1) / 2);
    size_t endIdx = full.size() - end.size() + ((numTaps - 1) / 2);
    std::vector resampSig(full.begin() + startIdx, full.begin() + endIdx);

    REQUIRE(resampSig.size() == yTruth.size());

    // At a sampling rate of 48000, 480 samples is approximately 10ms
    std::vector<double> errors(yTruth.size() - 960, 0.0);
    for (size_t i = 480; i < yTruth.size() - 480; i++)
    {
        // std::cout << resampSig[i] << ", ";
        REQUIRE_THAT(resampSig[i], Catch::Matchers::WithinAbs(yTruth[i], 0.01));  // Max error is 0.1
        errors[i-480] = resampSig[i] - yTruth[i];
    }

    // std::cout << "\n" << yTruth.size() / 4 << "\n";

    std::vector<double> sqrError = errors;
    double mean = std::accumulate(errors.begin(), errors.end(), 0.0) / static_cast<double>(errors.size());

    std::sort(errors.begin(), errors.end());
    for (size_t i = 1; i < errors.size(); i++)  // Proof of being sorted for median calculation
    {
        REQUIRE(errors[i - 1] <= errors[i]);
    }

    size_t idx;
    if (errors.size() % 2 == 0)
    {
        idx = errors.size() / 2;
    }
    else
    {
        idx = (errors.size() - 1) / 2;
    }

    // Average and median errors are *much* less than max error.
    REQUIRE(mean < 0.00001);
    REQUIRE(errors[idx] < 0.00001);  // median
}

} // namespace BML