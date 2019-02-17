//
// Created by Io on 13/04/2018.
//

#ifndef LINDENMAYER_LOADPROGRAMFROMLCODE_HPP
#define LINDENMAYER_LOADPROGRAMFROMLCODE_HPP
#include <generated/LSystemParser.h>
#include <generated/LSystemLexer.h>
#include <generated/LSystemBaseListener.h>
#include "Program.hpp"
#include "ModuleTable.hpp"

#include <vector>

namespace lcode
{
    class Program::LoadFromLCode final : public ALoader,
                                         public LSystemBaseListener,
                                         public antlr4::ANTLRErrorListener
    {
        std::string const& m_lcode;
        Content m_parseResult;
        ActionFactory m_actionTable;
    public:
        LoadFromLCode(std::string const& lcode, ModuleTable const& moduleTable, ActionFactory const& actionTable);

        Content load() override;

        // LSystemBaseListener required overrides
        void syntaxError(antlr4::Recognizer*, antlr4::Token*, size_t line, size_t charPositionInLine, const std::string& msg, std::exception_ptr) override;
        void reportAmbiguity(antlr4::Parser*, antlr4::dfa::DFA const&, size_t, size_t, bool, const antlrcpp::BitSet&, antlr4::atn::ATNConfigSet*) override;
        void reportAttemptingFullContext(antlr4::Parser*, antlr4::dfa::DFA const&, size_t, size_t, const antlrcpp::BitSet&, antlr4::atn::ATNConfigSet*) override;
        void reportContextSensitivity(antlr4::Parser*, antlr4::dfa::DFA const&, size_t, size_t, size_t, antlr4::atn::ATNConfigSet*) override;

        // Global variables
        void enterAxiom(LSystemParser::AxiomContext* context) override;
        void enterIteration(LSystemParser::IterationContext* context) override;
        void enterDistance(LSystemParser::DistanceContext* context) override;
        void enterAngle(LSystemParser::AngleContext* context) override;

        // Definitions
        void enterAlias_def(LSystemParser::Alias_defContext* context) override;
        void enterModule_def(LSystemParser::Module_defContext* context) override;
        void enterTransformation(LSystemParser::TransformationContext* context) override;
    private:
        void pushError(antlr4::Token* token, std::string const&message);
        void pushError(antlr4::tree::TerminalNode* node, std::string const& message);
        void pushError(Program::Error&& error);
    };
}

#endif //LINDENMAYER_LOADPROGRAMFROMLCODE_HPP
