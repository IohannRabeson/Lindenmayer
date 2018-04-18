//
// Created by Io on 11/04/2018.
//

#ifndef LINDENMAYER_MODULE_HPP
#define LINDENMAYER_MODULE_HPP
#include "Symbol.hpp"

#include <vector>
#include <string>
#include <map>

namespace lcode
{
    class Module
    {
    public:
        Module() = default;
        explicit Module(Symbol const symbol);
        Module(Module&&) = default;
        Module& operator = (Module&&) = default;
        Module(Module const&) = default;
        Module& operator = (Module const&) = default;

        bool isNull() const;
        friend std::string toString(Module const&);
        friend bool operator == (Module const&, Module const&);
        friend bool operator != (Module const&, Module const&);
        friend bool operator <= (Module const&, Module const&);
        friend bool operator >= (Module const&, Module const&);
        friend bool operator < (Module const&, Module const&);
        friend bool operator > (Module const&, Module const&);
    private:
        Symbol m_symbol;
    };

    using Modules = std::vector<Module>;

    class ModulePrototypes
    {
        std::map<std::string, Module> m_prototypes;
        Symbol::Integer m_symbolIdentifier = 0u;
    public:
        void registerModule(std::string const& identifier)
        {
            Symbol symbol(m_symbolIdentifier++);

            m_prototypes.insert(std::make_pair(identifier, Module(symbol)));
        }

        Module createModule(std::string const& identifier) const
        {
            Module result;
            auto const it = m_prototypes.find(identifier);

            if (it != m_prototypes.end())
            {
                result = it->second;
            }

            return result;
        }

        Modules createModules(std::vector<std::string> const& identifiers) const
        {
            Modules results;

            for (auto const& identifier : identifiers)
            {
                results.emplace_back(createModule(identifier));
            }

            return results;
        }
    };

    Modules makeModules(std::initializer_list<Symbol::Integer>&& identifiers);
}

#endif //LINDENMAYER_MODULE_HPP
