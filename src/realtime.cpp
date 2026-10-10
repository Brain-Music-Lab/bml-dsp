#include "realtime.h"

#include "filter.h"
#include "util/bml-math.h"

#include <iostream>

namespace BML
{
    namespace RealTime
    {
        RationalFactor findRationalFactor(size_t oldFs, size_t newFs)
        {
            // Find least common multiple
            size_t lcm = BML::Math::findLcm(oldFs, newFs);

            // Assertions
            assert (lcm % oldFs == 0);
            assert (lcm % newFs == 0);

            return { lcm/oldFs, lcm/newFs};
        }

        Convolution::Convolution(const std::vector<double>& filter) :
                m_filter(filter),
                m_filterSize(filter.size()),
                m_history(std::vector<double>(filter.size() - 1, 0.0))
            {}

        void Convolution::operator()(const std::vector<double>& block, std::vector<double>& out)
        {
            // Implementation is overlap-save //
            out.clear();
            out.resize(0);

            size_t B = block.size();  // B is the length of the block
            size_t L = m_filter.size();  // L is the filter size
            size_t N = B + L - 1;  // N is the size of the resultant convolution

            // Get block with prepended history
            std::vector<double>& paddedBlock = m_paddedBlock;
            paddedBlock.clear();
            paddedBlock.reserve(N);
            paddedBlock.insert(paddedBlock.end(), m_history.begin(), m_history.end());
            paddedBlock.insert(paddedBlock.end(), block.begin(), block.end());

            // Pad the filter
            std::vector<double>& paddedFilter = m_paddedFilter;
            paddedFilter.clear();
            paddedFilter.reserve(N);
            paddedFilter.insert(paddedFilter.end(), m_filter.begin(), m_filter.end());
            for (size_t i = L; i < N; i++)
            {
                paddedFilter.emplace_back(0.0);
            }

            // Update history
            std::copy(paddedBlock.end() - L + 1, paddedBlock.end(), m_history.begin());

            // Sanity check
            assert(paddedFilter.size() == paddedBlock.size());

            // Circular Convolution
            std::vector<std::complex<double>>& blockFft = m_blockFft;
            std::vector<std::complex<double>>& filterFft = m_filterFft;
            blockFft.clear();
            filterFft.clear();
            FFT::fft(paddedBlock, blockFft);
            FFT::fft(paddedFilter, filterFft);

            assert(blockFft.size() == filterFft.size());
            std::vector<std::complex<double>> multiplication;
            multiplication.reserve(blockFft.size());

            for (size_t i = 0; i < blockFft.size(); i++)
                multiplication.emplace_back(blockFft[i] * filterFft[i]);

            std::vector<std::complex<double>> circConv;
            FFT::ifft(multiplication, circConv);

            // Save the last B samples. The first L - 1 are garbage. Return the result
            out.clear();
            out.reserve(B);
            for (size_t i = L - 1; i < N; i++)
                out.emplace_back(circConv[i].real());
        }

        size_t Convolution::Taps() { return m_filterSize; }

        Resample::Resample(double oldFs, double newFs, double transitionBandwidth) :
            m_oldFs(oldFs),
            m_newFs(newFs),
            m_rationalFactor(findRationalFactor(m_oldFs, m_newFs)),
            m_convolution()
        {
            // If sample rates are equal, there is no point.
            if (m_rationalFactor.upsample == m_rationalFactor.downsample)
                return;

            // Make the convolution object based on which rational factor is larger
            if (m_rationalFactor.upsample > m_rationalFactor.downsample)
            {
                Filter::FIRLowPassFilter lpf(
                    oldFs * static_cast<double>(m_rationalFactor.upsample),
                    oldFs / 2.0, 
                    transitionBandwidth);
                m_convolution = std::make_unique<Convolution>(lpf());
            }
            else
            {
                Filter::FIRLowPassFilter lpf(
                    oldFs * static_cast<double>(m_rationalFactor.upsample),
                    newFs / 2.0,
                    transitionBandwidth);
                m_convolution = std::make_unique<Convolution>(lpf());
            }
        }

        void Resample::operator()(const std::vector<double>& block, std::vector<double>& out)
        {
            out.clear();
            // If the rational factor is equal to 1, there is no resampling to do.
            if (m_rationalFactor.upsample == m_rationalFactor.downsample)
            {
                out = block;
                return;
            }

            // Upsample if necessary
            std::vector<double> upsampledBlock;
            if (m_rationalFactor.upsample > 1) // If value is 1 no need to upsample
            {
                // Create zero-padded version of signal
                size_t zeroPadLen = block.size() * (size_t)m_rationalFactor.upsample;
                std::vector<double> zeroPadded(zeroPadLen, 0.0);
                for (size_t i = 0; i < zeroPadLen; i+= m_rationalFactor.upsample)
                {
                    zeroPadded[i] = block[i / m_rationalFactor.upsample];
                }

                // Get upsampled block
                m_convolution->operator()(zeroPadded, upsampledBlock);

                // Scale up by upsampling factor
                double mult = static_cast<double>(m_rationalFactor.upsample);
                std::transform(
                    upsampledBlock.begin(), 
                    upsampledBlock.end(),
                    upsampledBlock.begin(),
                    [mult](double value) {return value * mult;}
                );
            }
            else
                upsampledBlock = block;
                
            // Downsample if necessary
            if (m_rationalFactor.downsample > 1)  // If value is 1 no need to downsample
            {
                std::vector<double> filteredBlock;  // Allocate memory

                // If we upsample more than we downsample, we don't need to convolve.
                if (m_rationalFactor.upsample > m_rationalFactor.downsample)
                    filteredBlock = upsampledBlock;
                else
                    m_convolution->operator()(upsampledBlock, filteredBlock);                

                out.reserve(filteredBlock.size());
                for (size_t i = 0; i < filteredBlock.size(); i++)
                {
                    if (i % m_rationalFactor.downsample == 0)
                    {
                        out.emplace_back(filteredBlock[i]);
                    }
                }
            }
            else
                out = std::move(upsampledBlock);
        }

        size_t Resample::Taps() { return m_convolution.get()->Taps(); }
    }
}