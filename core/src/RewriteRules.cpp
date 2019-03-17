//
// Created by Io on 11/04/2018.
//

#include "RewriteRules.hpp"
#include <iostream>

namespace lcode
{
    void rewrite(RewriteRules const& rules, Modules& modules, unsigned int const iterations)
    {
        std::cout << "Rewrite " << modules.size() << " instructions with " << iterations << " iterations: ";
        for (auto i = 0u; i < iterations; ++i)
        {
            auto moduleIt = modules.begin();

            while (moduleIt != modules.end())
            {
                if (!rules.matchRules(*moduleIt,
                                      [&](RewriteRules::Rule const& rule)
                    {
                        moduleIt = modules.erase(moduleIt);
                        moduleIt = modules.insert(moduleIt, rule.second.modules.begin(), rule.second.modules.end());
                        moduleIt += rule.second.modules.size();
                    }))
                {
                    ++moduleIt;
                }
            }
        }
        std::cout << modules.size() << " modules\n";
    }

    Modules rewrote(RewriteRules const& rules, Modules const& modules, unsigned int const iterations)
    {
        Modules rewroteModules = modules;

        rewrite(rules, rewroteModules, iterations);

        return rewroteModules;
    }

    RewriteRule makeRule(Symbol::Integer const origin, std::initializer_list<Symbol::Integer>&& replacement)
    {
        RewriteRules::RewriteInfo info;

        info.modules = lcode::makeModules(std::move(replacement));
        info.probability = 1.;
        return RewriteRule{lcode::Module(lcode::Symbol(origin)), std::move(info)};
    }

    void RewriteRules::emplace(RewriteRules::Rule&& rule)
    {
        emplace(rule.first, std::move(rule.second.modules));
    }

    void RewriteRules::emplace(Module const module, Modules&& replacement)
    {
        RewriteInfo info;

        info.modules = std::move(replacement);
        info.probability = 1.f;

        m_rules.emplace_back(module, std::move(info));
    }

    void RewriteRules::emplace(Module const module, Modules&& replacement, float const probability)
    {
        RewriteInfo info;

        info.modules = std::move(replacement);
        info.probability.emplace(probability);
        m_rules.emplace_back(module, std::move(info));
    }

    RewriteRules::Iterator RewriteRules::end() const
    {
        return m_rules.end();
    }

    bool RewriteRules::empty() const
    {
        return m_rules.empty();
    }

    bool RewriteRules::matchRules(Module const module, std::function<void(Rule const&)> const& f) const
    {
        bool matched = false;

        auto const range = equalRange(module);

        if (range.size() == 1u)
        {
            if (range.front().get().first == module)
            {
                f(range.front().get());
                matched = true;
            }
        }
        else if (range.size() > 1u)
        {
            setupDistribution(range);

            auto const index = m_distribution(m_random);

            f(range[index].get());
            matched = true;
        }

        return matched;
    }

    void RewriteRules::setupDistribution(std::vector<std::reference_wrapper<RewriteRules::Rule const>> const& rules) const
    {
        std::vector<float> weights;

        for (auto const& rule : rules)
        {
            // In case of multiple rules, probabilities must be defined.
            assert( rule.get().second.probability );

            weights.push_back(rule.get().second.probability.value());
        }

        m_distribution.param(Distribution::param_type{weights.begin(), weights.end()});
    }

    std::vector<std::reference_wrapper<RewriteRules::Rule const>> RewriteRules::equalRange(Module const module) const
    {
        std::vector<std::reference_wrapper<Rule const>> range;

        for (auto const& rule : m_rules)
        {
            if (rule.first == module)
            {
                range.emplace_back(std::ref(rule));
            }
        }

        return range;
    }
}