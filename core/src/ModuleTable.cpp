//
// Created by Io on 13/04/2018.
//

#include "ModuleTable.hpp"

namespace lcode
{
    void ModuleTable::registerModule(std::string const& identifier, Action&& action)
    {
        auto module = createNextModule();

        m_identifierTable.emplace(identifier, module);
        m_actionTable.emplace(module, std::move(action));
        ++m_nextIdentifier;
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
        auto const actionIt = m_actionTable.find(module);

        if (actionIt != m_actionTable.end())
        {
            actionIt->second();
        }
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
}