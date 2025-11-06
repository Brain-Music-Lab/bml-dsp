#ifndef BML_CIRCULAR_BUFF_H
#define BML_CIRCULAR_BUFF_H
#include <mutex>
#include <vector>

namespace BML
{
    class CircularBuffer
    {
    public:
        CircularBuffer(size_t bufferSize);

        void write(double value);

        void write(std::vector<double>& values);

        double read();

        std::vector<double> read(int valuesToRead);

        std::vector<double> readNew();

    private:
        size_t m_size;
        std::vector<double> m_dataVector;
        int m_readPointer;
        int m_writerPointer;
        std::mutex m_mut;
    };
}

#endif // BML_CIRCULAR_BUFF_H