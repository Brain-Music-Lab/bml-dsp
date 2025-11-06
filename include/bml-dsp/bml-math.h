#ifndef _BML_DSP_BML_MATH_H_
#define _BML_DSP_BML_MATH_H_

#define _USE_MATH_DEFINES
#include <cmath>

namespace BML
{
    namespace Math
    {
        /**
        Find the greatest common factor of two values

        @param a first value
        @param b second value

        @return The greatest common factor of the two values
        */
        inline size_t findGcf(size_t a, size_t b)
        {
            if (a == 0)
                return b;
                
            return findGcf(b % a, a);
        }

        /**
        Find the least common multiple of two values

        @param a first value
        @param b second value

        @return The least common multiple of the two values
        */
        inline size_t findLcm(size_t a, size_t b)
        { 
            size_t num = a * b;
            size_t den = findGcf(a, b);

            return num / den;
        }

        /**
        Return evenly spaced values within the interval [0.0, end). The spacing between intervals
        is equal to step.

        @param start Start of interval. The interval includes this value.
        @param end End of interval. The interval does not include this value.
        @param step Spacing between values.

        @return Array of evenly spaced values
        */
        inline std::vector<double> arange(double start, double end, double step)
        {
            std::vector<double> out;  // Allocate memory
            while (start < end)       // Construct the vector
            {
                out.push_back(start);
                start += step;
            }

            return out;
        }

        /**
        Return evenly spaced values within the interval [0.0, end). The spacing between intervals
        is equal to 1.0.

        @param end End of interval. The interval does not include this value.

        @return Array of evenly spaced values
        */
        inline std::vector<double> arange(double end) { return arange(0.0, end, 1.0); }

        /**
        Return evenly spaced values within the interval [start, end). The spacing between intervals
        is equal to 1.0.

        @param start Start of interval. The interval includes this value.
        @param end End of interval. The interval does not include this value.

        @return Array of evenly spaced values
        */
        inline std::vector<double> arange(double start, double end) { return arange(start, end, 1.0); }
    }
}

#endif  // _BML_DSP_BML_MATH_H_
