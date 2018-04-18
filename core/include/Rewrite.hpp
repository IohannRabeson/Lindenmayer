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
            float probability = 1.f;
        };

        using Rules = std::multimap<Module, RewriteInfo>;
        using Iterator = Rules::const_iterator;
        using Distribution = std::discrete_distribution<std::size_t>;
    public:
        using Rule = Rules::value_type;

        void emplace(Rule&& rule, bool const equalizeProbabilities = true);

        void emplace(Module const module, Modules&& replacement, bool const equalizeProbabilities = true);

        void emplace(Module const module, Modules&& replacement, float const probability);

        lcode::Optional<Modules const&> getModules(Module const module) const;

        bool empty() const;
    private:
        Iterator find(Module const module) const;

        Iterator end() const;

        void updateProbabities(Module const module);

        friend Rule makeRule(Symbol::Integer const origin, std::initializer_list<Symbol::Integer>&& replacement);
    private:
        Rules m_rules;
        mutable std::mt19937_64 m_random;
        mutable Distribution m_distribution;
    };

    using RewriteRule = RewriteRules::Rule;

    RewriteRules::Rule makeRule(Symbol::Integer const origin, std::initializer_list<Symbol::Integer>&& replacement);
    void rewrite(RewriteRules const& rules, Modules& modules, unsigned int const iterations);
    Modules rewrote(RewriteRules const& rules, Modules const& modules, unsigned int const iterations);
}

#endif //LINDENMAYER_REWRITE_HPP
