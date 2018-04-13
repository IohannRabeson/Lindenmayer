//
// Created by Io on 11/04/2018.
//

#include "Module.hpp"

namespace lcode
{
    Module::Module(Symbol const symbol)
    : m_symbol(symbol)
    {
    }

    std::string toString(Module const& module)
    {
        return "[" + toString(module.m_symbol) + "]";
    }

    bool operator == (Module const& left, Module const& right)
    {
        return left.m_symbol == right.m_symbol;
    }

    bool operator != (Module const& left, Module const& right)
    {
        return left.m_symbol != right.m_symbol;
    }

    bool operator <= (Module const& left, Module const& right)
    {
        return left.m_symbol <= right.m_symbol;
    }

    bool operator >= (Module const& left, Module const& right)
    {
        return left.m_symbol >= right.m_symbol;
    }

    bool operator < (Module const& left, Module const& right)
    {
        return left.m_symbol < right.m_symbol;
    }

    bool operator > (Module const& left, Module const& right)
    {
        return left.m_symbol > right.m_symbol;
    }

    bool Module::isNull() const
    {
        return m_symbol.isNull();
    }

    Modules makeModules(std::initializer_list<Symbol::Integer>&& identifiers)
    {
        Modules modules;

        modules.reserve(identifiers.size());

        for (auto const identifier : identifiers)
        {
            modules.emplace_back(Symbol(identifier));
        }

        return modules;
    }
}