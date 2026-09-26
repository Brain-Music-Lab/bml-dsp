#ifndef _BML_DSP_BML_MATH_H_
#define _BML_DSP_BML_MATH_H_

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

namespace BML
{
    namespace Math
    {
        const double PI = 2.0 * std::acos(0.0);
        /**
        Find the greatest common factor of two values

        @param a first value
        @param b second value

        @return The greatest common factor of the two values
        */
        size_t findGcf(size_t a, size_t b);

        /**
        Find the least common multiple of two values

        @param a first value
        @param b second value

        @return The least common multiple of the two values
        */
        size_t findLcm(size_t a, size_t b);

        /**
        Return evenly spaced values within the interval [0.0, end). The spacing between intervals
        is equal to step.

        @param start Start of interval. The interval includes this value.
        @param end End of interval. The interval does not include this value.
        @param step Spacing between values.

        @return Array of evenly spaced values
        */
        std::vector<double> arange(double start, double end, double step);

        /**
        Return evenly spaced values within the interval [0.0, end). The spacing between intervals
        is equal to 1.0.

        @param end End of interval. The interval does not include this value.

        @return Array of evenly spaced values
        */
        std::vector<double> arange(double end);
        
        /**
        Return evenly spaced values within the interval [start, end). The spacing between intervals
        is equal to 1.0.

        @param start Start of interval. The interval includes this value.
        @param end End of interval. The interval does not include this value.

        @return Array of evenly spaced values
        */
        std::vector<double> arange(double start, double end);

        double sinc(double x);
        std::vector<double> sinc(const std::vector<double>& x);

        std::vector<double> blackman(size_t windowSize);

        size_t firstGreaterThan(double val, const std::vector<double>& arr) noexcept;

        double lerp(double val, double x1, double y1, double x2, double y2);

        double mean(const std::vector<double>& arr, bool excludeOutliers = false);

        std::vector<double> normalize(const std::vector<double>& arr);

        std::vector<double> genSineWave(
            const std::vector<double>& amps, 
            const std::vector<double>& freqs, 
            const std::vector<double>& t);
    }
}

#endif  // _BML_DSP_BML_MATH_H_
