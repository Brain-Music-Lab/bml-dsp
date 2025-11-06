#include "realtime.h"

#include "filter.h"
#include "bml-math.h"

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

        Convolution::Convolution(const std::vector<double>& signal, Method method) :
                m_signal(signal),
                m_signalSize(m_signal.size()),
                m_overlap(std::vector<double>(m_signalSize - 1, 0.0)),
                m_convMethod(method)
            {}

        std::vector<double> Convolution::convolve(const std::vector<double>& block)
        {
            switch (m_convMethod)
            {
                case Method::OVERLAP_ADD:
                    return overlapAdd(block);

                case Method::OVERLAP_SAVE:
                    return overlapSave(block);

                default:
                    return block;
            }
        }


        std::vector<double> Convolution::overlapAdd(const std::vector<double>& block)
        {
            // Allocate memory
            std::vector<double> out(block.size(), 0.0);  // Vector ultimately returned from function
            size_t block_size = block.size();  // Save block size
            std::vector<double> tempFilter = m_signal;  // Create temporary filter

            // Zero pad the filter
            for (size_t i = 0; i < block_size - 1; i++)
                tempFilter.push_back(0.0);

            // zero pad the block
            std::vector<double> zeroPaddedBlock = block;
            for (size_t i = 0; i < m_signalSize - 1; i++)
                zeroPaddedBlock.push_back(0.0);

            // Perform circular convolution
            std::vector<std::complex<double>> blockFft = FFT::fft(zeroPaddedBlock);
            std::vector<std::complex<double>> filterFft = FFT::fft(tempFilter);
            assert(blockFft.size() == filterFft.size());
            std::vector<std::complex<double>> multiplication;
            multiplication.reserve(blockFft.size());
            for (size_t i = 0; i < blockFft.size(); i++)
                multiplication.emplace_back(blockFft[i] * filterFft[i]);
            auto circConv = FFT::ifft(multiplication);

            // Store the entire circConv vector in out except for the last m_signalSize - 1 elements
            // while i < m_signalSize, add the overlapped values from last iteration
            // The first iteration overlap values are 0
            for (size_t i = 0; i < out.size(); i++)
            {
                out[i] = circConv[i];
                if (i < m_signalSize - 1)
                    out[i] += m_overlap[i];
            }

            // Store the last m_signalSize - 1 elements of circConv in m_overlap
            auto start = circConv.size() - (m_signalSize - 1);
            for (size_t i = start; i < circConv.size(); i++)
                m_overlap[i - start] = circConv[i];
                
            // Finally return the convolved block
            return out;
        }

        std::vector<double> Convolution::overlapSave(const std::vector<double>& block)
        {
            std::cout << "Overlap-Save is not yet implemented. Using the Overlap-Add method...";
            return overlapAdd(block);
        }

        Resample::Resample(double oldFs, double newFs) :
            m_oldFs(oldFs),
            m_newFs(newFs),
            m_rationalFactor(findRationalFactor(m_oldFs, m_newFs)),
            m_convolutionUp(),
            m_convolutionDown()
        {
            // If necessary, create a convolution object for upsampling
            // If we are upsampling at all, we need the convolution object for upsampling
            if (m_rationalFactor.upsample > 1)
            {
                // Create a low-pass filter
                double bw = Filter::findMaxBandwidth(oldFs);
                auto lpfUp = Filter::createLowPassFilter(newFs, oldFs / 2.0, bw);

                // Scale filter
                std::vector<double> filter = lpfUp;
                for (size_t i = 0; i < filter.size(); i++)
                    filter[i] *= m_rationalFactor.upsample;

                // Create the convolution object for upsampling
                m_convolutionUp = std::make_unique<Convolution>(filter);
            }
            else
                m_convolutionUp = nullptr;  // TODO: Is this how we should set a smartpointer to null?


            // If necessary, create a convolution object for downsampling
            // We only need the downwards convolution object if we are resampling to a lower frequency.
            if (m_rationalFactor.upsample < m_rationalFactor.downsample)
            {
                double bwDown = Filter::findMaxBandwidth(newFs);
                auto lpfDown = Filter::createLowPassFilter(oldFs * m_rationalFactor.upsample, newFs / 2.0, bwDown);
                m_convolutionDown = std::make_unique<Convolution>(lpfDown);
            }
            else
                m_convolutionDown = nullptr;  // TODO: Is this how we should set a smartpointer to null?

        }

        std::vector<double> Resample::resample(const std::vector<double>& block)
        {
            return block;
            // If the rational factor is equal to 1, there is no resampling to do.
            if (m_rationalFactor.upsample == m_rationalFactor.downsample)
                return block;

            // Upsample if necessary
            std::vector<double> upsampledBlock;
            if (m_rationalFactor.upsample > 1) // If value is 1 no need to upsample
            {
                // Create zero-padded version of signal
                size_t zeroPadLen = block.size() * (size_t)m_rationalFactor.upsample;
                std::vector<double> zeroPadded(zeroPadLen, 0.0);
                for (size_t i = 0; i < zeroPadded.size(); i++)
                {
                    if (i % m_rationalFactor.upsample == 0)
                        zeroPadded[i] = block[i / m_rationalFactor.upsample];
                }

                // Get upsampled block
                upsampledBlock = m_convolutionUp->convolve(zeroPadded);
            }
            else
                upsampledBlock = block;


            // return upsampledBlock;
            std::vector<double> out;  // Allocate memory for output vector
    
            // Downsample if necessary
            if (m_rationalFactor.downsample > 1)  // If value is 1 no need to downsample
            {
                std::vector<double> filteredBlock;  // Allocate memory

                // If we upsample more than we downsample, we don't need to convolve again. 
                // Therefore, there was no need to create the m_convolutionDown instance.
                if (m_convolutionDown != nullptr)
                    filteredBlock = m_convolutionDown->convolve(upsampledBlock);

                for (size_t i = 0; i < filteredBlock.size(); i++)
                    if (i % m_rationalFactor.downsample == 0)
                        out.push_back(filteredBlock[i]);
            }
            else
                out = upsampledBlock;

            return out;
        }

        std::vector<double> Resample::zeroPad(const std::vector<double>& vector_to_pad, int num_zeros)
        {
            // Allocate memory and right pad.
            std::vector<double> out(vector_to_pad.begin(), vector_to_pad.end());
            for (int i = 0; i < num_zeros; i++)
                out.push_back(0.0);

            return out;
        }
    }
}