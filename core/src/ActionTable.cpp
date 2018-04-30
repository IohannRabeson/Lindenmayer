//
// Created by Io on 29/04/2018.
//

#include "ActionTable.hpp"

namespace lcode
{
    bool ActionTable::registerAction(std::string const& identifier, Action&& action)
    {
        auto it = m_actions.find(identifier);

        if (it != m_actions.end())
        {
            return false;
        }

        m_actions.insert(it, std::make_pair(identifier, std::move(action)));

        return true;
    }

    auto ActionTable::get(std::string const& identifier) const -> Optional<Action>
    {
        Optional<Action> result;

        auto it = m_actions.find(identifier);

        if (it != m_actions.end())
        {
            result.emplace(it->second);
        }

        return result;
    }

    bool ActionTable::contains(std::string const& identifier) const
    {
        return m_actions.find(identifier) != m_actions.end();
    }
}