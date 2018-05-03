//
// Created by Io on 29/04/2018.
//

#ifndef LINDENMAYER_ACTIONTABLE_HPP
#define LINDENMAYER_ACTIONTABLE_HPP
#include "Optional.hpp"
#include "ModuleAction.hpp"

#include <map>
#include <string>

namespace lcode
{
    class ActionFactory
    {
    public:
        bool registerAction(std::string const& identifier, Action&& action);
        Optional<Action> get(std::string const& identifier) const;
        bool contains(std::string const& identifier) const;
    private:
        std::map<std::string, Action> m_actions;
    };
}

#endif //LINDENMAYER_ACTIONTABLE_HPP
