//
// Created by Io on 11/04/2018.
//

#ifndef LINDENMAYER_PROGRAM_HPP
#define LINDENMAYER_PROGRAM_HPP
#include "Module.hpp"
#include "RewriteRules.hpp"
#include "ModuleTable.hpp"
#include "Optional.hpp"

#include <vector>

namespace lcode
{
    class ProgramPrivate;

    class Program
    {
        class LoadFromLCode;
    public:
        struct Error
        {
            std::string message;
            std::size_t line;
            std::size_t charIndex;
        };

        struct Content
        {
            RewriteRules rewriteRules;
            Optional<Modules> axiom;
            Optional<unsigned int> iterations;
            Optional<float> distance;
            Optional<float> angle;
            Optional<float> initialAngle;
            std::vector<Error> errors;
            ModuleTable moduleTable;
        };

        class ALoader;

        std::vector<Error> load(ALoader&& loader);
        std::vector<Error> loadFromLCode(std::string const& lcode, ModuleTable const& table);
        Content const& content() const { return m_content; }
        Modules rewrite(unsigned int const iterations) const;
        Modules rewrite() const;
        void execute(unsigned int const iterations);
        void execute();
        bool haveErrors() const;
    private:
        Content m_content;
    };

    class Program::ALoader
    {
    public:
        virtual ~ALoader();
        virtual Content load() = 0;
    };
}

#endif //LINDENMAYER_PROGRAM_HPP
