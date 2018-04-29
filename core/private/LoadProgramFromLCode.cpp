//
// Created by Io on 13/04/2018.
//

#include "LoadProgramFromLCode.hpp"
#include "ContextHelper.hpp"

#include <string>

namespace lcode
{
    Program::LoadFromLCode::LoadFromLCode(std::string const& lcode, ModuleTable const& table)
            : m_lcode(lcode)
    {
        m_parseResult.moduleTable = table;
    }

    void Program::LoadFromLCode::enterTransformation(LSystemParser::TransformationContext* context)
    {
        auto const moduleContexts = context->module();
        auto const probabilityContext = context->probability();

        if (moduleContexts.size() > 1u)
        {
            Module assignedModule = ContextHelper::getModule(moduleContexts.front(), m_parseResult.moduleTable, m_parseResult.errors);
            auto replacementModules = ContextHelper::getModules(moduleContexts.begin() + 1, moduleContexts.end(), m_parseResult.moduleTable, m_parseResult.errors);

            if (probabilityContext && !ContextHelper::isError(probabilityContext->Float()))
            {
                auto probabilityValue = ContextHelper::getFloat(probabilityContext->Float(), m_parseResult.errors);

                assert( probabilityValue );

                m_parseResult.rewriteRules.emplace(std::move(assignedModule), std::move(replacementModules), probabilityValue.value());
            }
            else
            {
                m_parseResult.rewriteRules.emplace(std::move(assignedModule), std::move(replacementModules));
            }
        }
    }

    Program::Content Program::LoadFromLCode::load()
    {
        antlr4::ANTLRInputStream inputStream(m_lcode);
        LSystemLexer lexer(&inputStream);
        antlr4::CommonTokenStream tokenStream(&lexer);
        LSystemParser parser(&tokenStream);
        antlr4::tree::ParseTreeWalker treeWalker;

        lexer.removeErrorListener(&antlr4::ConsoleErrorListener::INSTANCE);
        lexer.addErrorListener(this);
        parser.removeErrorListener(&antlr4::ConsoleErrorListener::INSTANCE);
        parser.addErrorListener(this);

        // Parse!

        auto* const tree = parser.program();

        treeWalker.walk(this, tree);

        return m_parseResult;
    }

    void Program::LoadFromLCode::syntaxError(antlr4::Recognizer*, antlr4::Token*, size_t line,
                                             std::size_t charPositionInLine, const std::string& msg, std::exception_ptr)
    {
        Error error;

        error.charIndex = charPositionInLine;
        error.line = line;
        error.message = msg;
        m_parseResult.errors.emplace_back(std::move(error));
    }

    void Program::LoadFromLCode::reportAmbiguity(antlr4::Parser*, antlr4::dfa::DFA const&, std::size_t, std::size_t, bool,
                                                 const antlrcpp::BitSet&, antlr4::atn::ATNConfigSet*)
    {
        // Do nothing
    }

    void Program::LoadFromLCode::reportAttemptingFullContext(antlr4::Parser*, antlr4::dfa::DFA const&, size_t, size_t,
                                                             const antlrcpp::BitSet&, antlr4::atn::ATNConfigSet*)
    {
        // Do nothing
    }

    void Program::LoadFromLCode::reportContextSensitivity(antlr4::Parser*, antlr4::dfa::DFA const&,
                                                          size_t, size_t, size_t, antlr4::atn::ATNConfigSet*)
    {
        // Do nothing
    }

    void Program::LoadFromLCode::enterAxiom(LSystemParser::AxiomContext* axiomContext)
    {
        auto const& moduleContexts = axiomContext->module();

        m_parseResult.axiom = ContextHelper::getModules(moduleContexts.begin(), moduleContexts.end(),
                                                        m_parseResult.moduleTable, m_parseResult.errors);
    }

    void Program::LoadFromLCode::enterIterations(LSystemParser::IterationsContext* context)
    {
        auto const tokens = context->getTokens(LSystemParser::Integer);

        // TODO: resolve expression here instead of just parsing a simple number
        if (tokens.size() == 1u)
        {
            auto* const token = tokens.front();
            auto number = ContextHelper::getInteger(token, m_parseResult.errors);

            if (number)
            {
                if (number.value() >= 0)
                {
                    m_parseResult.iterations.emplace(number.value());
                }
                else
                {
                    auto error = ContextHelper::errorFromToken(token->getSymbol(), "iterations value must be unsigned");

                    m_parseResult.errors.emplace_back(std::move(error));
                }
            }
        }
    }

    void Program::LoadFromLCode::enterDistance(LSystemParser::DistanceContext* context)
    {
        auto const tokens = context->getTokens(LSystemParser::Float);

        // TODO: resolve expression here instead of just parsing a simple number
        if (tokens.size() == 1u)
        {
            auto const number = ContextHelper::getFloat(context->Float(), m_parseResult.errors).value();

            if (number)
            {
                if (number >= 0)
                {
                    m_parseResult.distance = number;
                }
                else
                {
                    auto const text = tokens.front()->getText();
                    Program::Error error = ContextHelper::errorFromToken(tokens.front()->getSymbol(),
                                                                         "Invalid distance: '" + text + "'");

                    m_parseResult.errors.emplace_back(std::move(error));
                }
            }
        }
    }

    void Program::LoadFromLCode::enterAngle(LSystemParser::AngleContext* context)
    {
        auto const tokens = context->getTokens(LSystemParser::Float);

        // TODO: resolve expression here instead of just parsing a simple number
        if (tokens.size() == 1u)
        {
            m_parseResult.angle = std::stod(tokens.front()->getText());
        }
    }

    void Program::LoadFromLCode::enterInitial_angle(LSystemParser::Initial_angleContext* context)
    {
        m_parseResult.initialAngle = ContextHelper::getFloat(context->Float(), m_parseResult.errors);
    }

    void Program::LoadFromLCode::enterAlias(LSystemParser::AliasContext* context)
    {
        auto const tokens = context->getTokens(LSystemParser::Identifier);

        if (tokens.size() != 2u)
        {
            Program::Error error = ContextHelper::errorFromToken(context->getStart(), "Invalid alias");

            m_parseResult.errors.emplace_back(std::move(error));
            return;
        }

        auto const aliasIdentifier = tokens[0u]->getText();
        auto const aliasedIdentifier = tokens[1u]->getText();

        if (!m_parseResult.moduleTable.createAlias(aliasIdentifier, aliasedIdentifier))
        {
            Program::Error error = ContextHelper::errorFromToken(context->getStart(), "Can't create alias '" + aliasIdentifier + "'");

            m_parseResult.errors.emplace_back(std::move(error));
        }
    }
}