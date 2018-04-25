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
#include <functional>

namespace lcode
{
    class RewriteRules
    {
        struct RewriteInfo
        {
            Modules modules;
            lcode::Optional<float> probability;
        };

    public:
        using Rule = std::pair<Module, RewriteInfo>;
    private:
        using Rules = std::vector<Rule>;
        using Iterator = Rules::const_iterator;
        using Distribution = std::discrete_distribution<std::size_t>;
    public:
        void emplace(Rule&& rule);
        void emplace(Module const module, Modules&& replacement);
        void emplace(Module const module, Modules&& replacement, float const probability);

        bool matchRules(Module const module, std::function<void(Rule const&)> const& f) const;

        bool empty() const;
    private:
        Iterator find(Module const module) const;
        Iterator end() const;
        std::vector<std::reference_wrapper<RewriteRules::Rule const>> equalRange(Module const module) const;
        friend Rule makeRule(Symbol::Integer const origin, std::initializer_list<Symbol::Integer>&& replacement);

        void setupDistribution(std::vector<std::reference_wrapper<RewriteRules::Rule const>> const& rules) const;
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
