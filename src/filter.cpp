#include "filter.h"
#include "util/bml-math.h"

namespace BML
{
    namespace Filter
    {
        FIRLowPassFilter::FIRLowPassFilter(
            double samplerate,
            double cutoffFreq,
            double transitionBandwidth
        )
        {
            // Calculate transition band for sample rate of 1 Hz
            double bw = transitionBandwidth / samplerate;

            // Calculate number of filter taps necessary
            int numFilterTaps = std::ceil(4.0 / bw);

            // number of filter taps should be odd
            if (static_cast<size_t>(numFilterTaps) % 2 == 0)
                numFilterTaps += 1.0;

            // Create the window and sinc filter
            std::vector<double> windowVector = BML::Math::blackman(static_cast<size_t>(numFilterTaps));
            std::vector<double> out;

            double arg;
            for (double n = 0; n < numFilterTaps; n++)
            {
                arg = cutoffFreq / samplerate * (n - (numFilterTaps - 1.0) / 2.0);
                if (arg == 0.0)
                    out.push_back(1.0);
                else
                    out.push_back(BML::Math::sinc(arg));
            }

            // Apply window
            std::transform(
                out.begin(),
                out.end(), 
                windowVector.begin(), 
                out.begin(), 
                [](const double val1, const double val2) {return val1 * val2;});

            // Normalize
            double sum = std::accumulate(out.begin(), out.end(), 0.0);
            std::transform(out.begin(), out.end(), out.begin(), [sum](const double val) {return val / sum;});

            m_filter = std::move(out);
        }

        std::vector<double> FIRLowPassFilter::operator()() { return m_filter; }
        int FIRLowPassFilter::Taps() { return m_nTaps; }
    }
}
