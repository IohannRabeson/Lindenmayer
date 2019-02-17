//
// Created by Io on 13/04/2018.
//

#include "LoadProgramFromLCode.hpp"
#include "ContextHelper.hpp"

#include <string>

namespace lcode
{
    Program::LoadFromLCode::LoadFromLCode(std::string const& lcode,
                                          ModuleTable const& moduleTable,
                                          ActionFactory const& actionTable)
    : m_lcode(lcode)
    , m_actionTable(actionTable)
    {
        m_parseResult.moduleTable = moduleTable;
    }

    void Program::LoadFromLCode::syntaxError(antlr4::Recognizer*, antlr4::Token*, size_t line,
            std::size_t charPositionInLine, const std::string& msg, std::exception_ptr)
    {
        Error error;

        error.charIndex = charPositionInLine;
        error.line = line;
        error.message = msg;
        pushError(std::move(error));
    }

    void Program::LoadFromLCode::reportAmbiguity(antlr4::Parser*,
            antlr4::dfa::DFA const&, std::size_t, std::size_t, bool,
            const antlrcpp::BitSet&, antlr4::atn::ATNConfigSet*)
    {
        // Do nothing
    }

    void Program::LoadFromLCode::reportAttemptingFullContext(antlr4::Parser*,
            antlr4::dfa::DFA const&, size_t, size_t,
            const antlrcpp::BitSet&, antlr4::atn::ATNConfigSet*)
    {
        // Do nothing
    }

    void Program::LoadFromLCode::reportContextSensitivity(antlr4::Parser*,
            antlr4::dfa::DFA const&,
            size_t, size_t, size_t,
            antlr4::atn::ATNConfigSet*)
    {
        // Do nothing
    }

    void Program::LoadFromLCode::pushError(antlr4::Token* token, std::string const& message)
    {
        pushError(ContextHelper::makeError(token, message));
    }

    void Program::LoadFromLCode::pushError(antlr4::tree::TerminalNode* node, std::string const& message)
    {
        pushError(ContextHelper::makeError(node, message));
    }

    void Program::LoadFromLCode::pushError(Program::Error&& error)
    {
        m_parseResult.errors.emplace_back(std::move(error));
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

    void Program::LoadFromLCode::enterAxiom(LSystemParser::AxiomContext* axiomContext)
    {
        auto const moduleContexts = axiomContext->module();

        if (m_parseResult.axiom)
        {
            pushError(moduleContexts.front()->StringIdentifier(), "Axiom identifier '%t' already used");
            return;
        }

        m_parseResult.axiom = ContextHelper::getModules(moduleContexts.begin(), moduleContexts.end(),
                                                        m_parseResult.moduleTable, m_parseResult.errors);
    }

    void Program::LoadFromLCode::enterIteration(LSystemParser::IterationContext* context)
    {
        auto const tokens = context->getTokens(LSystemParser::IntegerConstant);

        // TODO: resolve expression here instead of just parsing a simple number
        if (tokens.size() == 1u)
        {
            auto* const token = tokens.front();

            if (m_parseResult.iteration)
            {
                pushError(token, "Iteration already defined");
                return;
            }

            auto number = ContextHelper::getInteger(token, m_parseResult.errors);

            if (number)
            {
                if (number.value() >= 0)
                {
                    m_parseResult.iteration.emplace(number.value());
                }
                else
                {
                    pushError(token->getSymbol(), "Iteration value must be unsigned");
                }
            }
        }
    }

    void Program::LoadFromLCode::enterDistance(LSystemParser::DistanceContext* context)
    {
        // TODO: resolve expression here instead of just parsing a simple number
        if (context->FloatConstant())
        {
            if (m_parseResult.distance)
            {
                pushError(context->FloatConstant(), "Distance already defined");
                return;
            }

            auto const number = ContextHelper::getFloat(context->FloatConstant(), m_parseResult.errors);

            if (number)
            {
                if (number.value() < 0.f)
                {
                    pushError(context->FloatConstant(), "Distance must be positive: '%t' < 0");
                }
                else
                {
                    // Notice usage of optional::value() to allow implicit type conversion.
                    m_parseResult.distance.emplace(number.value());
                }
            }
        }
    }

    void Program::LoadFromLCode::enterAngle(LSystemParser::AngleContext* context)
    {
        // TODO: resolve expression here instead of just parsing a simple number
        if (context->FloatConstant())
        {
            if (m_parseResult.angle)
            {
                pushError(context->FloatConstant(), "Angle already defined");
                return;
            }

            m_parseResult.angle = std::stod(context->FloatConstant()->getText());
        }
    }

    void Program::LoadFromLCode::enterInitial_angle(LSystemParser::Initial_angleContext* context)
    {
        // TODO: resolve expression here instead of just parsing a simple number
        if (context->FloatConstant())
        {
            if (m_parseResult.initialAngle)
            {
                pushError(context->FloatConstant(), "Initial angle already defined");
                return;
            }
            // TODO: is initial angle is really usefull??? => https://github.com/IohannRabeson/Lindenmayer/issues/32
            m_parseResult.initialAngle = std::stod(context->FloatConstant()->getText());
        }
    }

    void Program::LoadFromLCode::enterAlias_def(LSystemParser::Alias_defContext* context)
    {
        auto const tokens = context->StringIdentifier();

        if (tokens.size() != 2u)
        {
            pushError(context->getStart(), "Invalid alias '%t'");
            return;
        }

        auto const aliasIdentifier = tokens[0u]->getText();
        auto const aliasedIdentifier = tokens[1u]->getText();

        if (!m_parseResult.moduleTable.createAlias(aliasIdentifier, aliasedIdentifier))
        {
            pushError(context->getStart(), "Can't create alias '%t'");
            return;
        }
    }

    void Program::LoadFromLCode::enterModule_def(LSystemParser::Module_defContext* context)
    {
        auto const stringIdentifiers = context->StringIdentifier();

        // Parser ensure the module definition has the form <StringIdentifier> = <StringIdentifier>
        assert( stringIdentifiers.size() == 2u );

        if (ContextHelper::isError(stringIdentifiers.front()))
        {
            pushError(stringIdentifiers.front(), "Expected module identifier instead of '%t'");
            return;
        }

        if (ContextHelper::isError(stringIdentifiers.back()))
        {
            pushError(stringIdentifiers.back(), "Expected string identifier instead of '%t'");
            return;
        }

        auto const moduleIdentifierText = stringIdentifiers.front()->getText();
        auto const actionIdentifierText = stringIdentifiers.back()->getText();

        if (m_parseResult.moduleTable.contains(moduleIdentifierText))
        {
            pushError(stringIdentifiers.front(), "Module identifier '%t' already used");
            return;
        }

        auto action = m_actionTable.get(actionIdentifierText);

        if (!action)
        {
            pushError(stringIdentifiers.back(), "Unknown action identifier: '%t'");
            return;
        }

        m_parseResult.moduleTable.registerModule(moduleIdentifierText, std::move(action.value()));
    }

    void Program::LoadFromLCode::enterTransformation(LSystemParser::TransformationContext* context)
    {
        auto const moduleContexts = context->module();
        auto const probabilityContext = context->probability();

        if (moduleContexts.size() > 1u)
        {
            Module assignedModule = ContextHelper::getModule(moduleContexts.front(), m_parseResult.moduleTable, m_parseResult.errors);
            auto replacementModules = ContextHelper::getModules(moduleContexts.begin() + 1, moduleContexts.end(), m_parseResult.moduleTable, m_parseResult.errors);

            if (probabilityContext && !ContextHelper::isError(probabilityContext->FloatConstant()))
            {
                auto probabilityValue = ContextHelper::getFloat(probabilityContext->FloatConstant(), m_parseResult.errors);

                assert( probabilityValue );

                m_parseResult.rewriteRules.emplace(std::move(assignedModule), std::move(replacementModules), probabilityValue.value());
            }
            else
            {
                m_parseResult.rewriteRules.emplace(std::move(assignedModule), std::move(replacementModules));
            }
        }
    }
}