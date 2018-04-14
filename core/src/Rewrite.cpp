//
// Created by Io on 11/04/2018.
//

#include "Rewrite.hpp"

namespace lcode
{
    namespace
    {
        RewriteRules::const_iterator selectRule(Module const& module, RewriteRules const& rules)
        {
            return rules.find(module);
        }
    }

    void rewrite(RewriteRules const& rules, Modules& modules, unsigned int const iterations)
    {
        for (auto i = 0u; i < iterations; ++i)
        {
            auto moduleIt = modules.begin();

            while (moduleIt != modules.end())
            {
                auto rule = selectRule(*moduleIt, rules);

                if (rule != rules.end())
                {
                    moduleIt = modules.erase(moduleIt);
                    moduleIt = modules.insert(moduleIt, rule->second.begin(), rule->second.end());
                    moduleIt += rule->second.size();
                }
                else
                {
                    ++moduleIt;
                }
            }
        }
    }

    Modules rewrited(RewriteRules const& rules, Modules const& modules, unsigned int const iterations)
    {
        Modules rewrited = modules;

        rewrite(rules, rewrited, iterations);

        return rewrited;
    }
}