//
// Created by Io on 13/04/2018.
//

#include "ModuleTable.hpp"

#include <cassert>
#include <iostream>

namespace lcode
{
    bool ModuleTable::registerModule(std::string const& identifier, Action&& action)
    {
        if (!isFreeIdentifier(identifier))
        {
            return false;
        }

        auto module = createNextModule();
        bool result = false;

        if (m_identifierTable.emplace(identifier, module).second)
        {
            result = m_actionTable.emplace(module, std::move(action)).second;
        }
        return result;
    }

    bool ModuleTable::registerModule(std::string const& identifier)
    {
        if (!isFreeIdentifier(identifier))
        {
            return false;
        }

        auto module = createNextModule();

        m_identifierTable.emplace(identifier, module);

        return true;
    }

    Module ModuleTable::createNextModule()
    {
        return Module(Symbol(m_nextIdentifier++));
    }

    void ModuleTable::execute(Modules const& modules) const
    {
        for (auto const& module : modules)
        {
            execute(module);
        }
    }

    void ModuleTable::execute(Module const& module) const
    {
        std::map<Module, Action>::const_iterator actionIt = m_actionTable.find(module);

        if (actionIt == m_actionTable.end())
        {
            auto const alias = m_aliasTable.find(module);

            actionIt = m_actionTable.find(alias->second);
        }

        if (actionIt != m_actionTable.end())
        {
            assert( actionIt->second );

            actionIt->second();
        }
    }

    Modules ModuleTable::createModules(std::initializer_list<std::string> const& identifiers) const
    {
        Modules results;

        for (auto const& identifier : identifiers)
        {
            results.emplace_back(createModule(identifier));
        }

        return results;
    }

    Modules ModuleTable::createModules(std::vector<std::string> const& identifiers) const
    {
        Modules results;

        for (auto const& identifier : identifiers)
        {
            results.emplace_back(createModule(identifier));
        }

        return results;
    }

    Module ModuleTable::createModule(std::string const& identifier) const
    {
        Module result;
        auto const it = m_identifierTable.find(identifier);

        if (it != m_identifierTable.end())
        {
            result = it->second;
        }

        return result;
    }

    bool ModuleTable::createAlias(std::string const& alias, std::string const& aliased)
    {
        bool result = false;

        if (registerModule(alias))
        {
            auto const aliasModuleIt = m_identifierTable.find(alias);
            auto const aliasedModuleIt = m_identifierTable.find(aliased);

            m_aliasTable.emplace(aliasModuleIt->second, aliasedModuleIt->second);
            result = true;
        }
        return result;
    }

    bool ModuleTable::isFreeIdentifier(std::string const& identifier) const
    {
        return m_identifierTable.find(identifier) == m_identifierTable.end();
    }
}
