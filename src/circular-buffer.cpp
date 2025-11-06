#include "circular-buffer.h"

#include <mutex>
#include <vector>

BML::CircularBuffer::CircularBuffer(size_t bufferSize) :
    m_size(bufferSize),
    m_dataVector(),
    m_readPointer(0),
    m_writerPointer(0),
    m_mut()
{
    for (int i = 0; i < m_size; i++)
    {
        m_dataVector.push_back(0.0);
    }
}

void BML::CircularBuffer::write(double value)
{
    std::lock_guard<std::mutex> guard(m_mut);
    m_dataVector[m_writerPointer] = value;
    m_writerPointer += 1;

    if (m_writerPointer >= m_size)
        m_writerPointer = 0;
}

void BML::CircularBuffer::write(std::vector<double>& values)
{
    for each (double value in values)
        write(value);
}

double BML::CircularBuffer::read()
{
    std::lock_guard<std::mutex> guard(m_mut);
    double returnValue = m_dataVector[m_readPointer];
    m_readPointer += 1;
    if (m_readPointer >= m_size)
        m_readPointer = 0;

    return returnValue;
}

std::vector<double> BML::CircularBuffer::read(int valuesToRead)
{
    std::vector<double> values;
    values.reserve(valuesToRead);
    while (values.size() < values.capacity())
        values.emplace_back(read());

    return values;
}

std::vector<double> BML::CircularBuffer::readNew()
{
    int writePos = m_writerPointer;

    if (writePos == m_readPointer)
        return {};

    int amountToRead;
    if (writePos > m_readPointer)
    {
        amountToRead = writePos - m_readPointer;
    }
    else
    {
        amountToRead = m_size - m_readPointer + writePos;
    }

    return read(amountToRead);
}
