#include "filter.h"

namespace BML
{
    namespace Filter
    {
        double findMaxBandwidth(double fc) { return 2.0 * fc; }
        
        
        std::vector<double> createLowPassFilter(double samplerate, double cutoffFreq, double transitionBand, Window window)
        {
            // Calculate transition band for sample rate of 1 Hz
            double bw = transitionBand / samplerate;

            // Calculate number of filter taps necessary
            int numFilterTaps = std::ceil(4.7 / bw);

            // number of filter taps should be odd
            if (static_cast<size_t>(numFilterTaps) % 2 == 0)
                numFilterTaps += 1.0;

            // Create the window
            std::vector<double> windowVector;
            switch (window)
            {
                case Window::BLACKMAN:
                {
                    double cosArg1;
                    double cosArg2;
                    for (double n = 0; n < numFilterTaps; n++)
                    {
                        cosArg1 = (2.0 * M_PI * n) / (numFilterTaps - 1.0);
                        cosArg2 = (4.0 * M_PI * n) / (numFilterTaps - 1.0);
                        windowVector.push_back(0.42 - 0.5 * std::cos(cosArg1) + 0.08 * std::cos(cosArg2));
                    }
                    break;
                }

                default:
                    break;
            }

            // Create sinc filter
            auto sinc = [](double x) -> double { return std::sin(M_PI * x) / (M_PI * x); };
            std::vector<double> out;
            double arg;
            for (double n = 0; n < numFilterTaps; n++)
            {
                arg = 2.0 * cutoffFreq / samplerate * (n - (numFilterTaps - 1.0) / 2.0);
                if (arg == 0.0)
                    out.push_back(1.0);
                else
                    out.push_back(sinc(arg));
            }

            // Apply window
            std::transform(out.begin(), out.end(), windowVector.begin(), out.begin(), [](const double val1, const double val2) {return val1 * val2;});

            // Normalize
            double sum = std::accumulate(out.begin(), out.end(), 0.0);
            std::transform(out.begin(), out.end(), out.begin(), [sum](const double val) {return val / sum;});

            return out;
        }
    }
}
