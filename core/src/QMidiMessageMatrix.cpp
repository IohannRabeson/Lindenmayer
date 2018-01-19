//
// Created by Io on 17/01/2018.
//

#include "QMidiMessageMatrix.hpp"
#include <cassert>
#include <set>

namespace
{
    static int computeIndex(int const x, int const y, int const width)
    {
        assert( x < width );

        return x + (y * width);
    }
}

QMidiMessageMatrix::QMidiMessageMatrix()
: m_width(0)
, m_height(0)
{
}

QMidiMessageMatrix::QMidiMessageMatrix(int const width, int const height)
: m_connections(std::make_unique<bool[]>(width * height))
, m_width(width)
, m_height(height)
{
}

auto QMidiMessageMatrix::begin() const -> ConstIterator
{
    return m_connections.get();
}

auto QMidiMessageMatrix::end() const -> ConstIterator
{
    return begin() + m_width * m_height;
}

void QMidiMessageMatrix::foreach(std::function<void(int const x, int const y, bool const value)> const&& f) const
{
    int x = 0u;
    int y = 0u;

    for (auto const connection : *this)
    {
        f(x, y, connection);
        ++x;
        if (x >= m_width)
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
        if (x >= m_width)
        {
            ++y;
            x = 0u;
        }
    }
}

void QMidiMessageMatrix::setWidth(int const width)
{
    if (m_width == width)
    {
        return;
    }
    resize(width, m_height);
}

void QMidiMessageMatrix::setHeight(int const height)
{
    if (m_height == height)
    {
        return;
    }
    resize(m_width, height);
}

void QMidiMessageMatrix::resize(int const width, int const height)
{
    std::set<std::pair<int, int>> trueValues;

    foreach([&trueValues](int const x, int const y, bool const value)
            {
                if (value)
                {
                    trueValues.emplace(std::make_pair(x, y));
                }
            });

    m_connections = std::make_unique<bool[]>(width * height);
    m_width = width;
    m_height = height;

    std::fill(begin(), end(), false);

    for (auto const& truePair : trueValues)
    {
        set(truePair.first, truePair.second, true);
    }
}

void QMidiMessageMatrix::set(int const x, int const y, bool const value)
{
    auto const i = computeIndex(x, y, m_width);

    assert( i < m_width * m_height );

    m_connections[i] = value;
}

bool QMidiMessageMatrix::get(int const x, int const y) const
{
    auto const i = computeIndex(x, y, m_width);

    assert( i < m_width * m_height );

    return m_connections[i];
}

auto QMidiMessageMatrix::begin() -> Iterator
{
    return m_connections.get();
}

auto QMidiMessageMatrix::end() -> Iterator
{
    return m_connections.get() + m_width * m_height;
}

void QMidiMessageMatrix::foreachInLine(int const line, std::function<void(int const, int const, bool const)> const&& f) const
{
    int x = 0u;

    for (auto& connection : *this)
    {
        f(x, line, connection);
        ++x;
        if (x >= m_width)
        {
            x = 0u;
        }
    }
}
