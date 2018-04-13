//
// Created by Io on 11/04/2018.
//

#ifndef LINDENMAYER_REWRITE_HPP
#define LINDENMAYER_REWRITE_HPP
#include "Module.hpp"

#include <map>

namespace lcode
{
    using RewriteRules = std::map<Module, Modules>;
    using RewriteRule = RewriteRules::value_type;

    void rewrite(RewriteRules const& rules, Modules& modules, unsigned int const iterations);
    Modules rewrited(RewriteRules const& rules, Modules const& modules, unsigned int const iterations);
}

#endif //LINDENMAYER_REWRITE_HPP
