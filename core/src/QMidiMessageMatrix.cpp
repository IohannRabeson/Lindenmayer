//
// Created by Io on 17/01/2018.
//

#include "QMidiMessageMatrix.hpp"
#include <cassert>
#include <set>

namespace
{
    static int computeIndex(int const x, int const y, int const output)
    {
        assert( x < output );

        return x + (y * output);
    }
}

QMidiMessageMatrix::QMidiMessageMatrix()
: m_outputCount(0)
, m_inputCount(0)
{
}

QMidiMessageMatrix::QMidiMessageMatrix(int const output, int const input)
: m_connections(std::make_unique<bool[]>(output * input))
, m_outputCount(output)
, m_inputCount(input)
{
}

auto QMidiMessageMatrix::begin() const -> ConstIterator
{
    return m_connections.get();
}

auto QMidiMessageMatrix::end() const -> ConstIterator
{
    return begin() + m_outputCount * m_inputCount;
}

void QMidiMessageMatrix::foreach(std::function<void(int const x, int const y, bool const value)> const&& f) const
{
    int x = 0u;
    int y = 0u;

    for (auto const connection : *this)
    {
        f(x, y, connection);
        ++x;
        if (x >= m_outputCount)
        {
            ++y;
            x = 0u;
        }
    }
}

void QMidiMessageMatrix::foreach(std::function<void(int const x, int const y, bool& value)> const&& f)
{
    int x = 0u;
    int y = 0u;

    for (auto& connection : *this)
    {
        f(x, y, connection);
        ++x;
        if (x >= m_outputCount)
        {
            ++y;
            x = 0u;
        }
    }
}

void QMidiMessageMatrix::setOutputCount(int const output)
{
    if (m_outputCount == output)
    {
        return;
    }
    resize(output, m_inputCount);
}

void QMidiMessageMatrix::setInputCount(int const input)
{
    if (m_inputCount == input)
    {
        return;
    }
    resize(m_outputCount, input);
}

void QMidiMessageMatrix::resize(int const output, int const input)
{
    std::set<std::pair<int, int>> trueValues;

    foreach([&trueValues](int const x, int const y, bool const value)
            {
                if (value)
                {
                    trueValues.emplace(std::make_pair(x, y));
                }
            });

    m_connections = std::make_unique<bool[]>(output * input);
    m_outputCount = output;
    m_inputCount = input;

    std::fill(begin(), end(), false);

    for (auto const& truePair : trueValues)
    {
        if (truePair.first < outputCount() && truePair.second < inputCount())
        {
            set(truePair.first, truePair.second, true);
        }
    }
}

void QMidiMessageMatrix::set(int const x, int const y, bool const value)
{
    auto const i = computeIndex(x, y, m_outputCount);

    assert( i < m_outputCount * m_inputCount );

    m_connections[i] = value;
}

bool QMidiMessageMatrix::get(int const x, int const y) const
{
    auto const i = computeIndex(x, y, m_outputCount);

    assert( i < m_outputCount * m_inputCount );

    return m_connections[i];
}

auto QMidiMessageMatrix::begin() -> Iterator
{
    return m_connections.get();
}

auto QMidiMessageMatrix::end() -> Iterator
{
    return m_connections.get() + m_outputCount * m_inputCount;
}

/*!
 * \brief Call f for each
 * \param inputPortIndex
 * \param f
 */
void QMidiMessageMatrix::forachInput(int const inputPortIndex, std::function<void(int const, int const)> const&& f) const
{
    int x = 0u;

    for (auto& connection : *this)
    {
        if (connection)
        {
            f(x, inputPortIndex);
        }
        ++x;
        if (x >= m_outputCount)
        {
            x = 0u;
        }
    }
}
