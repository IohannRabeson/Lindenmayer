//
// Created by Io on 11/04/2018.
//

#include "Program.hpp"
#include "LoadProgramFromLCode.hpp"

#include <generated/LSystemParser.h>
#include <generated/LSystemLexer.h>
#include <generated/LSystemBaseListener.h>

#include <antlr4-runtime.h>

#include <algorithm>
#include <string>

namespace lcode
{
    std::vector<Program::Error> Program::load(ALoader&& loader)
    {
        m_content = loader.load();
        m_loaded = m_content.errors.empty();
        return m_content.errors;
    }

    Program::ALoader::~ALoader() = default;

    std::vector<Program::Error> Program::loadFromLCode(std::string const& lcode, ModuleTable const& table)
    {
        return load(LoadFromLCode(lcode, table));
    }

    Modules Program::rewrite(unsigned int const iterations) const
    {
        return rewrote(m_content.rewriteRules, m_content.axiom, iterations);
    }

    void Program::execute(unsigned int const iterations)
    {
        Modules modules = rewrite(iterations);

        content().moduleTable.execute(modules);
    }
}