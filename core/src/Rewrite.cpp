//
// Created by Io on 11/04/2018.
//

#include "Rewrite.hpp"
#include <iostream>

namespace lcode
{
    void printModules(std::string const& label, Modules const& modules)
    {
        std::cout << label << ": ";

        for (auto const module : modules)
        {
            std::cout << toString(module);
        }

        std::cout << std::endl;
    }

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
        RewriteRules::RewriteInfo info;

        info.modules = lcode::makeModules(std::move(replacement));
        info.probability = 1.;
        return RewriteRule{lcode::Module(lcode::Symbol(origin)), std::move(info)};
    }

    void RewriteRules::emplace(RewriteRules::Rule&& rule, bool const equalizeProbabilities)
    {
        emplace(rule.first, std::move(rule.second.modules), equalizeProbabilities);
    }

    void RewriteRules::emplace(Module const module, Modules&& replacement, bool const equalizeProbabilities)
    {
        RewriteInfo info;

        info.modules = std::move(replacement);
        info.probability = 1.f;

        auto const result = m_rules.insert(std::make_pair(module, std::move(info)));

        if (equalizeProbabilities && result != m_rules.end())
        {
            updateProbabilities(result->first);
        }
    }

    void RewriteRules::emplace(Module const module, Modules&& replacement, float const probability)
    {
        RewriteInfo info;

        info.modules = std::move(replacement);
        info.probability.emplace(probability);
        m_rules.insert(std::make_pair(module, std::move(info)));
    }

    lcode::Optional<Modules const&> RewriteRules::getModules(Module const module) const
    {
        lcode::Optional<Modules const&> result;
        auto it = find(module);

        if (it != end())
        {
            result.emplace(it->second.modules);
        }

        return result;
    }

    bool RewriteRules::haveProbability(RewriteRules::Rules::value_type const& rule)
    {
        return rule.second.probability.is_initialized();
    }

    RewriteRules::Iterator RewriteRules::find(Module const module) const
    {
        auto const range = m_rules.equal_range(module);
        auto const size = std::distance(range.first, range.second);
        auto result = range.first;

        if (size > 1 && std::all_of(range.first, range.second, haveProbability))
        {
            std::vector<double> weights;

            for (auto it = range.first; it != range.second; ++it)
            {
                weights.emplace_back(it->second.probability.value());
            }

            m_distribution = Distribution(weights.begin(), weights.end());
            m_distribution.param(Distribution::param_type{weights.begin(), weights.end()});

            auto const index = m_distribution(m_random);

            assert( index < weights.size() );

            result = range.first;

            std::advance(result, index);
        }

        return result;
    }

    RewriteRules::Iterator RewriteRules::end() const
    {
        return m_rules.end();
    }

    void RewriteRules::updateProbabilities(Module const module)
    {
        auto const range = m_rules.equal_range(module);
        auto const total = static_cast<float>(std::distance(range.first, range.second));
        auto const equalDistribution = 1.f / total;

        for (auto it = range.first; it != range.second; ++it)
        {
            it->second.probability = equalDistribution;
        }
    }

    bool RewriteRules::empty() const
    {
        return m_rules.empty();
    }
}