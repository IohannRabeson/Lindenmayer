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
    Program::ALoader::~ALoader() = default;

    std::vector<Program::Error> Program::load(ALoader&& loader)
    {
        m_content = loader.load();
        return m_content.errors;
    }

    std::vector<Program::Error> Program::loadFromLCode(std::string const& lcode, ModuleTable const& moduleTable, ActionFactory const& actionTable)
    {
        return load(LoadFromLCode(lcode, moduleTable, actionTable));
    }

    std::vector<Program::Error> Program::loadFromLCode(std::string const& lcode, ActionFactory const& actionTable)
    {
        return load(LoadFromLCode(lcode, lcode::ModuleTable(), actionTable));
    }

    Modules Program::rewrite(unsigned int const iterations) const
    {
        return rewrote(m_content.rewriteRules, m_content.axiom.get_value_or({}), iterations);
    }

    Modules Program::rewrite() const
    {
        return rewrite(m_content.iteration.get_value_or(0u));
    }

    void Program::execute(unsigned int const iterations)
    {
        if (haveErrors())
        {
            return;
        }

        Modules modules = rewrite(iterations);

        content().moduleTable.execute(modules);
    }

    void Program::execute()
    {
        execute(m_content.iteration.get_value_or(0u));
    }

    bool Program::haveErrors() const
    {
        return !m_content.errors.empty();
    }
}