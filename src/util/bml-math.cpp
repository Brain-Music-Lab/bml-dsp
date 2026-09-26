#include "util/bml-math.h"
#include <exception>
#include <algorithm>
#include <numeric>

namespace BML
{
    namespace Math
    {
        size_t findGcf(size_t a, size_t b)
        {
            if (a == 0)
                return b;
                
            return findGcf(b % a, a);
        }

        
        size_t findLcm(size_t a, size_t b)
        { 
            size_t num = a * b;
            size_t den = findGcf(a, b);

            return num / den;
        }

        std::vector<double> arange(double start, double end, double step)
        {
            std::vector<double> out;  // Allocate memory
            while (start < end)       // Construct the vector
            {
                out.push_back(start);
                start += step;
            }

            return out;
        }

        std::vector<double> arange(double end) { return arange(0.0, end, 1.0); }

        std::vector<double> arange(double start, double end) { return arange(start, end, 1.0); }

        double sinc(double x)
        {
            if (x == 0.0)
                return 1.0;

            else
                return std::sin(PI * x) / (PI * x);
        }

        std::vector<double> sinc(const std::vector<double>& x) 
        { 
            std::vector<double> out;
            out.reserve(x.size());

            for (size_t i = 0; i < x.size(); i++)
                out.emplace_back(sinc(x[i]));

            return out;
        }

        std::vector<double> blackman(size_t windowSize)
        {
            if (windowSize == 0)
                return {};

            std::vector<double> out;
            out.reserve(windowSize);
            double M = static_cast<double>(windowSize);

            for (double i = 0.0; i < M; i++)
            {
                out.emplace_back(
                    0.42 - 0.5 * std::cos(2.0 * PI * i / (M - 1.0)) + 0.08 * std::cos(4.0 * PI * i / (M - 1.0))
                );
            }

            return out;        
        }

        double lerp(double val, double x1, double y1, double x2, double y2)
        {
            return y1 + ((y2 - y1)/(x2 - x1)) * (val - x1);
        }

        size_t firstGreaterThan(double val, const std::vector<double>& arr) noexcept
        {
            for (size_t i = 0; i < arr.size(); i++)
            {
                double x2 = arr[i];
                if (arr[i] > val)
                    return i;
            }

            return arr.size();
        }

        double mean(const std::vector<double>& arr, bool excludeOutliers)
        {
            // if (excludeOutliers)
            // {
            //     std::vector<double> arrayCopy;
            //     arrayCopy.reserve(arr.size());
            //     std::copy(arr.begin(), arr.end(), arrayCopy.begin());
            // }

            // Not going to exclude outliers
            double avg = std::accumulate(arr.begin(), arr.end(), 0.0);
            return avg / static_cast<double>(arr.size());
        }

        std::vector<double> normalize(const std::vector<double>& arr)
        {
            // Calculate the average, subtract that from each element to center on 0
            double avg = mean(arr);
            std::vector<double> normArr;
            normArr.reserve(arr.size());
            for (size_t i = 0; i < arr.size(); i++)
            {
                normArr.emplace_back(arr[i] - avg);
            }

            // Find maximum value of the absolute value of the array's elements
            std::vector<double> normArrCopy;
            normArrCopy.reserve(normArr.size());

            for (size_t i = 0; i < normArrCopy.size(); i++)
            {
                normArrCopy.emplace_back(std::abs(normArr[i]));
            }
            auto maxElement = std::max_element(normArrCopy.begin(), normArrCopy.end());
            double maxVal = *maxElement;
            normArrCopy.clear();  // free up memory

            // Divide the centered array by this max value
            std::vector<double> resultArr;
            resultArr.reserve(normArr.size());
            for (size_t i = 0; i < normArr.size(); i++)
            {
                resultArr[i] = normArr[i] / maxVal;
            }

            return resultArr;
        }

        std::vector<double> genSineWave(
            const std::vector<double>& amps, 
            const std::vector<double>& freqs, 
            const std::vector<double>& t)
        {
            std::vector<double> out;
            out.reserve(t.size());
            double outVal;

            for (size_t i = 0; i < t.size(); i++)
            {
                outVal = 0.0;
                for (size_t n = 0; n < amps.size(); n++)
                {
                    outVal += amps[n] * std::sin(freqs[n] * 2.0 * PI * t[i]);
                }

                out.emplace_back(outVal);
            }

            return out;
        }
    }
}