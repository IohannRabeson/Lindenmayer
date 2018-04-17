//
// Created by Io on 11/04/2018.
//

#ifndef LINDENMAYER_REWRITE_HPP
#define LINDENMAYER_REWRITE_HPP
#include "Module.hpp"
#include "Optional.hpp"

#include <map>
#include <random>
#include <iterator>

namespace lcode
{
    class RewriteRules
    {
        struct RewriteInfo
        {
            Modules modules;
            float probability;
        };

        using Rules = std::multimap<Module, RewriteInfo>;
        using Iterator = Rules::const_iterator;
        using Distribution = std::discrete_distribution<float>;
    public:
        using Rule = std::pair<Module, Modules>;

        void emplace(Rule&& rule, bool const equalizeProbabilities = true)
        {
            emplace(rule.first, std::move(rule.second), equalizeProbabilities);
        }

        void emplace(Module const module, Modules&& replacement, bool const equalizeProbabilities = true)
        {
            RewriteInfo info;

            info.modules = std::move(replacement);
            info.probability = 1.f;

            auto const result = m_rules.insert(std::make_pair(module, std::move(info)));

            if (equalizeProbabilities && result != m_rules.end())
            {
                updateProbabities(result->first);
            }
        }

        void emplace(Module const module, Modules&& replacement, float const probability)
        {
            RewriteInfo info;

            info.modules = std::move(replacement);
            info.probability = probability;
            m_rules.insert(std::make_pair(module, std::move(info)));
        }

        lcode::Optional<Modules const&> getModules(Module const module) const
        {
            lcode::Optional<Modules const&> result;
            auto it = find(module);

            if (it != end())
            {
                result.emplace(it->second.modules);
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
            auto const range = m_rules.equal_range(module);
            auto const size = std::distance(range.first, range.second);
            auto result = range.first;

            if (size > 1)
            {
                std::vector<double> weights;

                for (auto it = range.first; it != range.second; ++it)
                {
                    weights.emplace_back(it->second.probability);
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

        Iterator end() const
        {
            return m_rules.end();
        }

        void updateProbabities(Module const module)
        {
            auto const range = m_rules.equal_range(module);
            auto const total = static_cast<float>(std::distance(range.first, range.second));
            auto const equalDistribution = 1.f / total;

            for (auto it = range.first; it != range.second; ++it)
            {
                it->second.probability = equalDistribution;
            }
        }
    private:
        Rules m_rules;
        mutable std::mt19937_64 m_random;
        mutable Distribution m_distribution;
    };

    using RewriteRule = RewriteRules::Rule;

    RewriteRule makeRule(Symbol::Integer const origin, std::initializer_list<Symbol::Integer>&& replacement);

    void rewrite(RewriteRules const& rules, Modules& modules, unsigned int const iterations);
    Modules rewrote(RewriteRules const& rules, Modules const& modules, unsigned int const iterations);
}

#endif //LINDENMAYER_REWRITE_HPP
