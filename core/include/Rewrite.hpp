//
// Created by Io on 11/04/2018.
//

#ifndef LINDENMAYER_REWRITE_HPP
#define LINDENMAYER_REWRITE_HPP
#include "Module.hpp"
#include "Optional.hpp"

#include <map>

namespace lcode
{
    class RewriteRules
    {
    public:
        using Rule = std::pair<Module, Modules>;
        using Iterator = std::map<Module, Modules>::const_iterator;

        template <class ... A>
        void emplace(A&& ... args)
        {
            m_rules.emplace(std::forward<A>(args)...);
        }

        lcode::Optional<Modules const&> getModules(Module const module) const
        {
            lcode::Optional<Modules const&> result;
            auto it = find(module);

            if (it != end())
            {
                result.emplace(it->second);
            }

            return result;
        }


        bool empty() const
        {
            return m_rules.empty();
        }

    private:
        Iterator find(Module const module) const
        {
            return m_rules.find(module);
        }

        Iterator end() const
        {
            return m_rules.end();
        }

    private:
        std::map<Module, Modules> m_rules;
    };

    using RewriteRule = RewriteRules::Rule;

    RewriteRule makeRule(Symbol::Integer const origin, std::initializer_list<Symbol::Integer>&& replacement);

    void rewrite(RewriteRules const& rules, Modules& modules, unsigned int const iterations);
    Modules rewrote(RewriteRules const& rules, Modules const& modules, unsigned int const iterations);
}

#endif //LINDENMAYER_REWRITE_HPP
