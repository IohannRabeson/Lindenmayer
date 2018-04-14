//
// Created by Io on 13/04/2018.
//

#ifndef LINDENMAYER_SYMBOLTABLE_HPP
#define LINDENMAYER_SYMBOLTABLE_HPP
#include "Module.hpp"

#include <functional>
#include <string>
#include <map>

namespace lcode
{
    class ModuleTable
    {
    public:
        using Action = std::function<void()>;

        void registerModule(std::string const& identifier, Action&& action);
        void registerModule(std::string const& identifier);

        bool createAlias(std::string const& alias, std::string const& aliased);
        Module createModule(std::string const& identifier) const;
        Modules createModules(std::vector<std::string> const& identifiers) const;

        void execute(Modules const& modules) const;
    private:
        void execute(Module const& module) const;
        Module createNextModule();
    private:
        std::map<std::string, Module> m_identifierTable;
        std::map<Module, Action> m_actionTable;

        std::uint16_t m_nextIdentifier = 0u;
    };
}

#endif //LINDENMAYER_SYMBOLTABLE_HPP
