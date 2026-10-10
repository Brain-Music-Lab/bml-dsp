#ifndef _BML_DSP_FFT_H_
#define _BML_DSP_FFT_H_

#include <vector>
#include <complex>
#include <fftw3.h>

namespace BML
{
    namespace FFT
    {
        /**
        Perform an fft on a set of input samples.

        @param input The input samples. This must be a real-valued 1 dimensional
        vector.
        
        @return The fft. This takes the form of a vector of complex numbers.
        */
        void fft(const std::vector<double>& input, std::vector<std::complex<double>>& output);

        /**
        Perform an ifft on a set of input frequency values.

        @param input The input frequencies. This must be a vector of complex
        numbers.

        @return The result of the ifft.
        */
        void ifft(const std::vector<std::complex<double>>& input, std::vector<std::complex<double>>& output);
        
    }
}

#endif  // _BML_DSP_FFT_H_
