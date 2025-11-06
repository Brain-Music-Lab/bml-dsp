#include <fft.h>

namespace BML
{
    namespace FFT
    {
        std::vector<std::complex<double>> fft(std::vector<double> input)
        {
            // Create variables
            fftw_complex *in;
            fftw_complex *out;
            fftw_plan p;

            // Allocate memory
            size_t N = input.size();
            in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N);
            out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N);

            // Create FFT plan
            p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

            //  Set input vector
            for (int i = 0; i < N; i++)
            {
                in[i][0] = input[i];
                in[i][1] = 0.0;
            }

            // Execute FFT and save result to output vector
            fftw_execute(p);
            std::vector<std::complex<double>> output;
            output.reserve(N);
            for (int i = 0; i < N; i++)
            {
                output.emplace_back(std::complex<double>(out[i][0], out[i][1]));
            }

            // Free memory
            fftw_destroy_plan(p);
            fftw_free(in);
            fftw_free(out);

            // Return result
            return output;
        }

        std::vector<double> ifft(std::vector<std::complex<double>> input)
        {
            // Create variables
            fftw_complex *in;
            fftw_complex *out;
            fftw_plan p;

            // Allocate memory
            size_t N = input.size();
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
            std::vector<double> output;
            output.reserve(N);
            for (int i = 0; i < N; i++)
            {
                output.emplace_back((1.0 / N) * out[i][0]);
            }

            // Free memory
            fftw_destroy_plan(p);
            fftw_free(in);
            fftw_free(out);

            // Return result
            return output;
        }
    }
}