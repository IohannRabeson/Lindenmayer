//
// Created by Io on 28/04/2018.
//

#ifndef LINDENMAYER_CONTEXTHELPER_HPP
#define LINDENMAYER_CONTEXTHELPER_HPP
#include <generated/LSystemParser.h>
#include "Program.hpp"
#include "ModuleTable.hpp"

namespace lcode::ContextHelper
{
    Program::Error errorFromToken(antlr4::Token* token, std::string const& message);
    Module getModule(LSystemParser::ModuleContext* const context, ModuleTable const& table, std::vector<Program::Error>& errors);
    Modules getModules(std::vector<LSystemParser::ModuleContext*>::const_iterator begin,
                       std::vector<LSystemParser::ModuleContext*>::const_iterator end,
                       ModuleTable const& table,
                       std::vector<Program::Error>& errors);

    bool isError(antlr4::tree::TerminalNode* const node);


    Optional<long> getInteger(antlr4::tree::TerminalNode* const terminalNode, std::vector<Program::Error>& errors);
    Optional<float> getFloat(antlr4::tree::TerminalNode* const terminalNode, std::vector<Program::Error>& errors);
}

#endif //LINDENMAYER_CONTEXTHELPER_HPP
