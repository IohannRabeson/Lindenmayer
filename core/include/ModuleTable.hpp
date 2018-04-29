//
// Created by Io on 13/04/2018.
//

#ifndef LINDENMAYER_MODULETABLE_HPP
#define LINDENMAYER_MODULETABLE_HPP
#include "Module.hpp"

#include <functional>
#include <string>
#include <map>
#include <initializer_list>

namespace lcode
{
    class ModuleTable
    {
    public:
        using Action = std::function<void()>;

        bool registerModule(std::string const& identifier, Action&& action);
        bool registerModule(std::string const& identifier);

        bool createAlias(std::string const& alias, std::string const& aliased);
        Module createModule(std::string const& identifier) const;
        Modules createModules(std::initializer_list<std::string> const& identifiers) const;
        Modules createModules(std::vector<std::string> const& identifiers) const;

        void execute(Modules const& modules) const;
        void execute(Module const& module) const;
    private:
        Module createNextModule();
        bool isFreeIdentifier(std::string const& identifier) const;
    private:
        std::map<std::string, Module> m_identifierTable;
        std::map<Module, Action> m_actionTable;
        std::map<Module, Module> m_aliasTable;

        std::uint16_t m_nextIdentifier = 0u;
    };
}

#endif //LINDENMAYER_SYMBOLTABLE_HPP
