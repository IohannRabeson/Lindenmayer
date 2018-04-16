//
// Created by Io on 11/04/2018.
//

#include "Rewrite.hpp"

namespace lcode
{
    void rewrite(RewriteRules const& rules, Modules& modules, unsigned int const iterations)
    {
        for (auto i = 0u; i < iterations; ++i)
        {
            auto moduleIt = modules.begin();

            while (moduleIt != modules.end())
            {
                auto replacement = rules.getModules(*moduleIt);

                if (replacement)
                {
                    auto const& replacementModules = replacement.value();

                    moduleIt = modules.erase(moduleIt);
                    moduleIt = modules.insert(moduleIt, replacementModules.begin(), replacementModules.end());
                    moduleIt += replacementModules.size();
                }
                else
                {
                    ++moduleIt;
                }
            }
        }
    }

    Modules rewrote(RewriteRules const& rules, Modules const& modules, unsigned int const iterations)
    {
        Modules rewroteModules = modules;

        rewrite(rules, rewroteModules, iterations);

        return rewroteModules;
    }

    RewriteRule makeRule(Symbol::Integer const origin, std::initializer_list<Symbol::Integer>&& replacement)
    {
        return RewriteRule{lcode::Module(lcode::Symbol(origin)), lcode::makeModules(std::move(replacement))};
    }
}