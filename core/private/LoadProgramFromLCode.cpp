//
// Created by Io on 13/04/2018.
//

#include "LoadProgramFromLCode.hpp"

#include <string>

namespace lcode
{
    namespace antlr
    {
        unsigned int stoui(std::string const& text, std::size_t* consumed)
        {
            return static_cast<unsigned int>(std::stoul(text, consumed));
        }

        std::string getTokenIdentifier(antlr4::Token* const token)
        {
            return token->getText();
        }

        template <class IT>
        std::vector<std::string> getTokenIdentifiers(IT begin, IT end)
        {
            std::vector<std::string> identifiers(std::distance(begin, end));

            std::generate(identifiers.begin(), identifiers.end(), [&begin] ()
            {
                auto text = (*begin)->getText();

                ++begin;
                return text;
            });

            return identifiers;
        }

        Program::Error errorFromToken(antlr4::Token* token, std::string const& message)
        {
            Program::Error error;

            error.charIndex = token->getCharPositionInLine();
            error.line = token->getLine();
            error.message = message;
            return error;
        }
    }

    namespace ContextHelper
    {
        std::string getIdentifier(LSystemParser::ModuleContext* const context)
        {
            assert(context != nullptr);
            assert(context->Identifier() != nullptr);

            return context->Identifier()->getText();
        }

        Module getModule(LSystemParser::ModuleContext* const context, ModuleTable const& table)
        {
            assert(context != nullptr);
            assert(context->Identifier() != nullptr);

            auto const identifierText = context->Identifier()->getText();

            // TODO: use context->parameter_pack()!

            return table.createModule(identifierText);
        }

        Modules
        getModules(std::vector<LSystemParser::ModuleContext*>::const_iterator begin, std::vector<LSystemParser::ModuleContext*>::const_iterator end, ModuleTable const& table)
        {
            Modules modules;

            modules.reserve(std::distance(begin, end));

            for (auto it = begin; it != end; ++it)
            {
                modules.emplace_back(getModule(*it, table));
            }

            return modules;
        }

        float getFloat(antlr4::tree::TerminalNode* const terminalNode, std::vector<Program::Error>& errors)
        {
            assert(terminalNode != nullptr);

            auto const text = terminalNode->getText();
            auto result = 0.f;

            if (!text.empty() && dynamic_cast<antlr4::tree::ErrorNode*>(terminalNode) == nullptr)
            {
                result = std::stof(text);
            }
            else
            {
                auto error = antlr::errorFromToken(terminalNode->getSymbol(), "invalid probability");

                errors.emplace_back(std::move(error));
            }
            return result;
        }
    }

    void Program::LoadFromLCode::enterTransformation(LSystemParser::TransformationContext* context)
    {
        auto const moduleContexts = context->module();
        auto const probabilityContext = context->probability();

        if (moduleContexts.size() > 1u)
        {
            Module assignedModule = ContextHelper::getModule(moduleContexts.front(), m_parseResult.moduleTable);
            auto replacementModules = ContextHelper::getModules(moduleContexts.begin() + 1, moduleContexts.end(), m_parseResult.moduleTable);

            if (probabilityContext && probabilityContext->Float())
            {
                auto probabilityValue = ContextHelper::getFloat(probabilityContext->Float(), m_parseResult.errors);

                m_parseResult.rewriteRules.emplace(std::move(assignedModule), std::move(replacementModules), probabilityValue);
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

    void
    Program::LoadFromLCode::syntaxError(antlr4::Recognizer*, antlr4::Token*, size_t line, size_t charPositionInLine, const std::string& msg, std::exception_ptr)
    {
        Error error;

        error.charIndex = charPositionInLine;
        error.line = line;
        error.message = msg;
        m_parseResult.errors.emplace_back(std::move(error));
    }

    void
    Program::LoadFromLCode::reportAmbiguity(antlr4::Parser*, antlr4::dfa::DFA const&, size_t, size_t, bool, const antlrcpp::BitSet&, antlr4::atn::ATNConfigSet*)
    {
    }

    void
    Program::LoadFromLCode::reportAttemptingFullContext(antlr4::Parser*, antlr4::dfa::DFA const&, size_t, size_t, const antlrcpp::BitSet&, antlr4::atn::ATNConfigSet*)
    {
    }

    void
    Program::LoadFromLCode::reportContextSensitivity(antlr4::Parser*, antlr4::dfa::DFA const&, size_t, size_t, size_t, antlr4::atn::ATNConfigSet*)
    {
    }

    template <class T, class F>
    Optional<T> Program::LoadFromLCode::toT(antlr4::Token* const token, std::vector<Error>& errors, F f)
    {
        std::size_t digitUsed = 0u;
        auto const text = token->getText();
        Optional<T> result;

        T tempResult = f(text, &digitUsed);

        if (digitUsed != text.size())
        {
            Program::Error error = antlr::errorFromToken(token, "Invalid input: '" + text + "'");

            errors.emplace_back(std::move(error));
        }
        else
        {
            result.emplace(std::move(tempResult));
        }

        return result;
    }

    void Program::LoadFromLCode::enterAxiom(LSystemParser::AxiomContext* axiomContext)
    {
        auto const& moduleContexts = axiomContext->module();

        m_parseResult.axiom = ContextHelper::getModules(moduleContexts.begin(), moduleContexts.end(), m_parseResult.moduleTable);
    }

    void Program::LoadFromLCode::enterIterations(LSystemParser::IterationsContext* context)
    {
        auto const tokens = context->getTokens(LSystemParser::Integer);

        // TODO: resolve expression here instead of just parsing a simple number
        if (tokens.size() == 1u)
        {
            m_parseResult.iterations = toT<unsigned int>(tokens.front()->getSymbol(), m_parseResult.errors, antlr::stoui);
        }
    }

    void Program::LoadFromLCode::enterDistance(LSystemParser::DistanceContext* context)
    {
        auto const tokens = context->getTokens(LSystemParser::Float);

        // TODO: resolve expression here instead of just parsing a simple number
        if (tokens.size() == 1u)
        {
            auto const value = ContextHelper::getFloat(context->Float(), m_parseResult.errors);

            if (value >= 0)
            {
                m_parseResult.distance = value;
            } else
            {
                auto const text = tokens.front()->getText();
                Program::Error error = antlr::errorFromToken(tokens.front()->getSymbol(),
                                                             "Invalid distance: '" + text + "'");

                m_parseResult.errors.emplace_back(std::move(error));
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
            Program::Error error = antlr::errorFromToken(context->getStart(), "Invalid alias");

            m_parseResult.errors.emplace_back(std::move(error));
            return;
        }

        auto const aliasIdentifier = tokens[0u]->getText();
        auto const aliasedIdentifier = tokens[1u]->getText();

        if (!m_parseResult.moduleTable.createAlias(aliasIdentifier, aliasedIdentifier))
        {
            Program::Error error = antlr::errorFromToken(context->getStart(), "Can't alias undefined identifier '" + aliasedIdentifier + "'");

            m_parseResult.errors.emplace_back(std::move(error));
        }
    }
}