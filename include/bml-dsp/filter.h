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

        /**
        Create a low pass filter

        @param samplerate The samplerate of the filter
        @param cutoffFreq The cutoff frequency of the filter
        @param transitionBand The bandwidth, in hertz of the transition band
        @param window The window to use.

        @return A vector the represents the filter. To apply the filter to a signal
        A, convolve the filter vector with A.
        */
        std::vector<double> createLowPassFilter(double samplerate, 
                                                double cutoffFreq, 
                                                double transitionBand, 
                                                Window window = Window::BLACKMAN);
    }
}

#endif  // _BML_DSP_FILTER_H_