#ifndef _BML_DSP_TIMER_H_
#define _BML_DSP_TIMER_H_

#include <string>
#include <chrono>
#include <iostream>

namespace BML
{
    /**
    Custom benchmarking tools
    */
    namespace Benchmark
    {
        /**
        A stopwatch used for timing the duration of code within a scope. A stopwatch 
        starts upon instantiation and stops on destruction. There is no need for 
        start or stop methods.
        */
        class Stopwatch
        {
        public:

            /**
            Construct a stopwatch

            @param title A title for the stop watch. This helps to differentiate different 
            instances of the timer from each other when text is printed to the console.
            */
            Stopwatch(std::string title) :
                m_mess(message)
            {
                // Set start timepoint to time when stopwatch was created
                m_startTp = std::chrono::high_resolution_clock::now();
            }

            /**
            Stop the stopwatch. Prints the elapsed time to the console. Destroy the instance.
            */
            ~Stopwatch()
            {
                // Get current time. Convert start and end timepoints to doubles.
                auto endTp = std::chrono::high_resolution_clock::now();
                auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTp).time_since_epoch().count();
                auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTp).time_since_epoch().count();

                // Find duration in milliseconds
                auto duration = end - start;
                double ms = duration * 0.001;

                // Print result to console
                std::cout << m_mess << ": " << ms << "ms elapsed.\n";
            }

        private:
            std::chrono::time_point<std::chrono::high_resolution_clock> m_startTp;  // The starting point
            std::string m_title;  // The title given to the stopwatch instance
        };
    }
}

#endif  // _BML_DSP_TIMER_H_
