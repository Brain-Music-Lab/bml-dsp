#ifndef _BML_DSP_FILTER_H_
#define _BML_DSP_FILTER_H_

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <algorithm>
#include <numeric>


namespace BML
{
    namespace Filter
    {
        /**
        Find the maximum necessary bandwith for a filter. This function simply returns
        2 * fc. It is recommended to utilize a smaller value such as 0.75 * maxBandwidth
        or 0.5 * maxBandwidth for the bandwidth in a filter to ensure time aliasing is 
        avoided.

        @param fc The cutoff frequency

        @return the absolute max bandwidth (2 * fc)
        */
        double findMaxBandwidth(double fc);

        /**
        An enum class representing the types of windows that can be used in a filter.
        */
        enum class Window
        {
            RECTANGULAR,
            BLACKMAN,
            HAMMING,
            KAISER
        };

        class LowPassFilter
        {
        public:
            /**
            Create a windowed-sinc low pass filter. 

            References:
                - https://tomroelandts.com/articles/how-to-create-a-simple-low-pass-filter
                - https://fiiir.com/
                - https://www.dspguide.com/ch16/2.htm

            @param samplerate The samplerate of the filter
            @param cutoffFreq The cutoff frequency of the filter
            @param bandwidthAdj The max transition bandwidth (which the number of taps is based on) is equal to 
            2.0 * cutoffFrq / samplerate, but bandwidthAdj can alter this. The full calculation for the the transition 
            bandwidth is tapsAdj * 2.0 * cutoffFrq / samplerate. A value of 1 leaves this unchanged. A value of less 
            than one will shrink the filter transition bandwidth and increase the number of taps. A value of greater 
            than one will grow the filter transition bandwidth and decrease the number of taps. Default to 1.0.
            */
            LowPassFilter(double samplerate, double cutoffFreq, double filterBandwidth);

            std::vector<double> operator()();
            int Taps();

        private:
            std::vector<double> m_filter;
            int m_nTaps;
        };
    }
}

#endif  // _BML_DSP_FILTER_H_