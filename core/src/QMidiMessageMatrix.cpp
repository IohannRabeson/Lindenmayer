//
// Created by Io on 17/01/2018.
//

#include "QMidiMessageMatrix.hpp"
#include <cassert>
#include <utility>

namespace imp
{
    inline int computeIndex(int const x, int const y, int const output)
    {
        assert( x < output );

        return x + (y * output);
    }

    inline void removeInput(std::vector<std::pair<int, int>>& values, int const inputToRemove)
    {
        auto it = values.begin();

        while (it != values.end())
        {
            auto const inputIndex = it->second;

            if (inputIndex == inputToRemove)
            {
                it = values.erase(it);
            }
            else if (inputIndex > inputToRemove)
            {
                it->second -= 1;
                ++it;
            }
            else
            {
                ++it;
            }
        }
    }

    inline void removeOutput(std::vector<std::pair<int, int>>& values, int const outputToRemove)
    {
        auto it = values.begin();

        while (it != values.end())
        {
            auto const outputIndex = it->first;

            if (outputIndex == outputToRemove)
            {
                it = values.erase(it);
            }
            else if (outputIndex > outputToRemove)
            {
                it->first -= 1;
                ++it;
            }
            else
            {
                ++it;
            }
        }
    }
}


QMidiMessageMatrix::QMidiMessageMatrix()
: m_outputCount(0)
, m_inputCount(0)
{
}

QMidiMessageMatrix::QMidiMessageMatrix(int const outputCount, int const inputCount)
: m_connections(std::make_unique<bool[]>(outputCount * inputCount))
, m_outputCount(outputCount)
, m_inputCount(inputCount)
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

void QMidiMessageMatrix::foreach(std::function<void(int const outputIndex, int const inputIndex, bool& value)> const&& f)
{
    int outputIndex = 0u;
    int inputIndex = 0u;

    for (auto& connection : *this)
    {
        f(outputIndex, inputIndex, connection);
        ++outputIndex;
        if (outputIndex >= m_outputCount)
        {
            ++inputIndex;
            outputIndex = 0u;
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
    Values trueValues = collectValues(true);

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
    auto const i = imp::computeIndex(x, y, m_outputCount);

    assert( i < m_outputCount * m_inputCount );

    m_connections[i] = value;
}

bool QMidiMessageMatrix::get(int const x, int const y) const
{
    auto const i = imp::computeIndex(x, y, m_outputCount);

    assert( i < m_outputCount * m_inputCount );

    return m_connections[i];
}

void QMidiMessageMatrix::removeInput(const int input)
{
    if (m_outputCount > 0 && m_inputCount > 1)
    {
        std::size_t const newSize = static_cast<std::size_t>(m_outputCount * (m_inputCount - 1));
        auto newConnections = std::make_unique<bool[]>(newSize);
        auto trueValues = collectValues(true);

        imp::removeInput(trueValues, input);

        m_connections = std::move(newConnections);
        --m_inputCount;
        restoreValues(trueValues);
    }
}

void QMidiMessageMatrix::removeOutput(const int output)
{
    if (m_outputCount > 1 && m_inputCount > 0)
    {
        std::size_t const newSize = static_cast<std::size_t>((m_outputCount - 1) * m_inputCount);
        auto newConnections = std::make_unique<bool[]>(newSize);
        auto trueValues = collectValues(true);

        imp::removeOutput(trueValues, output);

        m_connections = std::move(newConnections);
        --m_outputCount;
        restoreValues(trueValues);
    }
}

auto QMidiMessageMatrix::collectValues(bool const valueToCollect) const -> Values
{
    Values trueValues;

    foreach([&trueValues, valueToCollect](int const x, int const y, bool const value)
            {
                if (value == valueToCollect)
                {
                    trueValues.emplace_back(x, y);
                }
            });

    return trueValues;
}

void QMidiMessageMatrix::restoreValues(Values const& values)
{
    std::fill(begin(), end(), false);
    foreach([&values](int const x, int const y, bool& value)
            {
                auto const pair = std::make_pair(x, y);
                auto const it = std::find(values.begin(), values.end(), pair);

                value = (it != values.end());
            });
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
    for (auto x = 0; x < outputCount(); ++x)
    {
        if (get(x, inputPortIndex))
        {
            f(x, inputPortIndex);
        }
    }
}
