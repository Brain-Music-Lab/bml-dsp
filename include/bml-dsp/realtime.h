#ifndef _BML_DSP_REALTIME_H_
#define _BML_DSP_REALTIME_H_

#include "fft.h"
#include <cassert>
#include <memory>

namespace BML
{
    namespace RealTime
    {
        /**
        A structure containing the upsample and downsample factors that make up the rational factor
        to do any resampling from any sample rate to any other sample rate.
        */
        struct RationalFactor
        {
            size_t upsample;
            size_t downsample;
        };
        
        /**
        Find the rational factor to correctly resample (upsample then downsample) a time series
        signal

        @param oldFs The original samplerate
        @param newFs The destination samplerate

        @return The rational factor that determines by how much to upsample and downsample
        */
        RationalFactor findRationalFactor(size_t oldFs, size_t newFs);

        /**
        Perform real-time convolution using the overlap-add or overlap-save method.
        */
        class Convolution
        {
        public:

            /**
            Construct an instance of the Convolution class

            @param lpf A filter to convolve with a blocked signal in the "convolve" method
            @param method Choose the method to use for convolution
            */
            Convolution(const std::vector<double>& filter);

            /**
            Convolve a block of a time series signal with the signal specified from the constructor
            of the class

            @param block An input block signal to use in convolution with the filter passed
            to the constructor

            @return The input block convolved with the signal passed to the constructor.
            */
            std::vector<double> operator()(const std::vector<double>& block);

            size_t Taps();

        private:
            std::vector<double> m_filter;  // The signal passed to the constructor
            size_t m_filterSize;           // The size of the signal passed to the constructor
            std::vector<double> m_history; // Memory allocation for overlap portion of the convolution

            /**
            The overlap-save method of convolution

            @param block The input block to convolve against the signal passed to the constructor

            @return The convolved block
            */
            std::vector<double> overlapSave(const std::vector<double>& block);
        };

        /**
        Resample a time series signal from any sample rate to any other sample rate
        */
        class Resample
        {
        public:

            /**
            Construct an instance of the Resample class

            @param oldFs the original samplerate
            @param newFs the destination samplerate
            */
            Resample(double oldFs, double newFs, double filterBandwidth);

            /**
            Resample a given block of a time series signal
            
            @param block The block to resample

            @return The resampled block
            */
            std::vector<double> operator()(const std::vector<double>& block);
            
            size_t Taps();

        private:

            double m_oldFs;  // The original sample rate
            double m_newFs;  // The destination sample rate
            RationalFactor m_rationalFactor;  // the rational factor to use in resampling

            std::unique_ptr<Convolution> m_convolution;  // Convolution instance needed in resampling

        };
    }
}

#endif  // _BML_DSP_REALTIME_H_
