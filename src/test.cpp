std::vector<std::complex<double>> fft(std::vector<double> input)
{
    size_t N = input.size();
    if (N == 0) return {};

    // 1. Allocate memory
    fftw_complex *in  = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    fftw_complex *out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

    // 2. Set input vector FIRST (Always initialize data before planning!)
    for (size_t i = 0; i < N; i++)
    {
        in[i][0] = input[i]; // Real part
        in[i][1] = 0.0;      // Imaginary part
    }

    // 3. Create FFT plan AFTER input data is ready
    fftw_plan p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    // 4. Execute FFT
    fftw_execute(p);

    // 5. Save result to output vector
    std::vector<std::complex<double>> output;
    output.reserve(N);
    for (size_t i = 0; i < N; i++)
    {
        output.emplace_back(out[i][0], out[i][1]);
    }

    // 6. Free memory
    fftw_destroy_plan(p);
    fftw_free(in);
    fftw_free(out);

    // 7. Return the actual result (Fixed from return {};)
    return output;
}