//
// Created by Io on 11/04/2018.
//

#include "Symbol.hpp"

namespace lcode
{
    Symbol::Symbol()
    : m_identifier(std::numeric_limits<decltype(m_identifier)>::max())
    {
    }

    Symbol::Symbol(Integer const identifier)
    : m_identifier(identifier)
    {
    }

    std::string toString(Symbol const symbol)
    {
        return std::to_string(symbol.m_identifier);
    }

    bool operator ==(Symbol const left, Symbol const right)
    {
        return left.m_identifier == right.m_identifier;
    }

    bool operator !=(Symbol const left, Symbol const right)
    {
        return left.m_identifier != right.m_identifier;
    }

    bool operator <=(Symbol const left, Symbol const right)
    {
        return left.m_identifier <= right.m_identifier;
    }

    bool operator >=(Symbol const left, Symbol const right)
    {
        return left.m_identifier >= right.m_identifier;
    }

    bool operator <(Symbol const left, Symbol const right)
    {
        return left.m_identifier < right.m_identifier;
    }

    bool operator >(Symbol const left, Symbol const right)
    {
        return left.m_identifier > right.m_identifier;
    }

    bool Symbol::isNull() const
    {
        return m_identifier == std::numeric_limits<decltype(m_identifier)>::max();
    }
}