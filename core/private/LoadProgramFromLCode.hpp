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

namespace lcode
{
    class Program::LoadFromLCode : public ALoader,
                                   public LSystemBaseListener,
                                   public antlr4::ANTLRErrorListener
    {
        std::string const& m_lcode;
        Content m_parseResult;
    public:
        LoadFromLCode(std::string const& lcode, ModuleTable const& table)
        : m_lcode(lcode)
        {
            m_parseResult.moduleTable = table;
        }

        Content load() override;


        void syntaxError(antlr4::Recognizer*, antlr4::Token*,
                size_t line, size_t charPositionInLine, const std::string& msg, std::exception_ptr) override;

        void reportAmbiguity(antlr4::Parser*, antlr4::dfa::DFA const&, size_t, size_t, bool, const antlrcpp::BitSet&,
                antlr4::atn::ATNConfigSet*) override;

        void reportAttemptingFullContext(antlr4::Parser*, antlr4::dfa::DFA const&, size_t, size_t, const antlrcpp::BitSet&,
                antlr4::atn::ATNConfigSet*) override;

        void reportContextSensitivity(antlr4::Parser*, antlr4::dfa::DFA const&, size_t, size_t, size_t,
                antlr4::atn::ATNConfigSet*) override;


        template <class T>
        using FunctionType = T(*)(std::string const&, std::size_t*);

        template <class T, class F>
        Optional<T> toT(antlr4::Token* const token, std::vector<Error>& errors, F f);


        void enterAxiom(LSystemParser::AxiomContext* context) override;

        void enterTransformation(LSystemParser::TransformationContext* context) override;

        void enterIterations(LSystemParser::IterationsContext* context) override;

        void enterDistance(LSystemParser::DistanceContext* context) override;

        void enterAngle(LSystemParser::AngleContext* context) override;

        void enterInitial_angle(LSystemParser::Initial_angleContext* context) override;

        void enterAlias(LSystemParser::AliasContext* context) override;

    };
}

#endif //LINDENMAYER_LOADPROGRAMFROMLCODE_HPP
