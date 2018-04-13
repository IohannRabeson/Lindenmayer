//
// Created by Io on 11/04/2018.
//

#ifndef LINDENMAYER_PROGRAM_HPP
#define LINDENMAYER_PROGRAM_HPP
#include "Module.hpp"
#include "Rewrite.hpp"
#include "ModuleTable.hpp"

#include <vector>

namespace lcode
{
    class ProgramPrivate;

    class Program
    {
        class LoadFromLCode;
    public:
        class ALoader;

        struct Error
        {
            std::string message;
            std::size_t line;
            std::size_t charIndex;
        };

        std::vector<Error> load(ALoader&& loader);
        std::vector<Error> loadFromLCode(std::string const& lcode, ModuleTable const& table);
        Modules rewrite(unsigned int const iterations) const;
    private:
        RewriteRules m_rewriteRules;
        Modules m_axiom;
    };

    class Program::ALoader
    {
    public:
        struct ParseResult
        {
            RewriteRules rewriteRules;
            Modules axiom;
            std::vector<Error> errors;
        };

        virtual ~ALoader();
        virtual ParseResult load() = 0;
    };
}

#endif //LINDENMAYER_PROGRAM_HPP
