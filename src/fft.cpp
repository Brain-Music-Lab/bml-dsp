#include "fft.h"

#include <iostream>
#include <exception>
#include <cstring>

namespace BML
{
    namespace FFT
    {
        void fft(const std::vector<double>& input, std::vector<std::complex<double>>& output)
        {
            size_t N = input.size();
            if (N == 0) 
                throw std::runtime_error("Cannot calculate FFT from an empty array.");
            
            // Create variables
            double *in;
            fftw_complex *out;
            fftw_plan p;

            // Allocate memory
            in = (double*)fftw_malloc(sizeof(double) * N);

            size_t out_size = N / 2 + 1;
            out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * out_size);

            // Create FFT plan
            p = fftw_plan_dft_r2c_1d(N, in, out, FFTW_ESTIMATE);

            //  Set input vector
            std::copy(input.begin(), input.end(), in);

            // Execute FFT and save result to output vector
            fftw_execute(p);
            output.clear();
            output.resize(N);
            
            std::memcpy(output.data(), out, out_size * sizeof(std::complex<double>));

            for (int i = out_size; i < N; i++)
            {
                output[i] = std::conj(output[N-i]);
            }

            // Free memory
            fftw_destroy_plan(p);
            fftw_free(in);
            fftw_free(out);
        }

        void ifft(const std::vector<std::complex<double>>& input, std::vector<std::complex<double>>& output)
        {
            output.clear();
            size_t N = input.size();
            if (N == 0)
                throw std::runtime_error("Cannot calculate IFFT from an empty array.");

            // Create variables
            fftw_complex *in;
            fftw_complex *out;
            fftw_plan p;

            // Allocate memory
            in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N);
            out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N);

            // Create FFT plan
            p = fftw_plan_dft_1d(N, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);

            //  Set input vector
            for (int i = 0; i < N; i++)
            {
                in[i][0] = input[i].real();
                in[i][1] = input[i].imag();
            }

            // Execute FFT and save result to output vector
            fftw_execute(p);
            double N_d = static_cast<double>(N);
            output.clear();
            output.resize(N);
            for (int i = 0; i < N; i++)
            {
                output[i] = std::complex(
                    (1.0/N_d) * out[i][0],
                    (1.0/N_d) * out[i][1]
                );
            }

            // Free memory
            fftw_destroy_plan(p);
            fftw_free(in);
            fftw_free(out);
        }
    }
}