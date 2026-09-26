#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "bml-dsp/filter.h"
#include "bml-dsp/util/csv.h"

TEST_CASE("Sinc Filter")
{
    std::stringstream ss;
    std::filesystem::path currentPath(__FILE__);
    ss << currentPath.parent_path().string() << "/ground_truth/data/filter_h.csv";
    std::vector<double> h_truth = BML::readOneLineCSV(ss.str());

    std::vector<double> filter = BML::Filter::createLowPassFilter(
        20000.0,  // Sample rate
        25.0      // Cutoff Frequency
    );

    REQUIRE(h_truth.size() == filter.size());
    for (size_t i = 0; i < filter.size(); i++)
        REQUIRE_THAT(h_truth[0], Catch::Matchers::WithinAbs(filter[0], 0.0001));
}