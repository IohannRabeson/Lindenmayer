//
// Created by Io on 28/04/2018.
//

#include "ContextHelper.hpp"

namespace lcode::ContextHelper
{
    Program::Error errorFromToken(antlr4::Token* token, std::string const& message)
    {
        Program::Error error;

        error.charIndex = token->getCharPositionInLine();
        error.line = token->getLine();
        error.message = message;
        return error;
    }

    Module getModule(LSystemParser::ModuleContext* const context,
                     ModuleTable const& table,
                     std::vector<Program::Error>& errors)
    {
        assert(context != nullptr);
        assert(context->Identifier() != nullptr);

        auto const identifierText = context->Identifier()->getText();

        // TODO: use context->parameter_pack()!

        auto const module = table.createModule(identifierText);

        if (module.isNull())
        {
            auto error = errorFromToken(context->Identifier()->getSymbol(), "Invalid identifier '" + context->Identifier()->getText() + "'");

            errors.emplace_back(std::move(error));
        }

        return module;
    }

    Modules getModules(std::vector<LSystemParser::ModuleContext*>::const_iterator begin, std::vector<LSystemParser::ModuleContext*>::const_iterator end, ModuleTable const& table, std::vector<Program::Error>& errors)
    {
        Modules modules;

        modules.reserve(std::distance(begin, end));

        for (auto it = begin; it != end; ++it)
        {
            modules.emplace_back(getModule(*it, table, errors));
        }

        return modules;
    }

    bool isError(antlr4::tree::TerminalNode* const node)
    {
        return node == nullptr || dynamic_cast<antlr4::tree::ErrorNode*>(node) != nullptr;
    }

    Optional<long> getInteger(antlr4::tree::TerminalNode* const terminalNode, std::vector<Program::Error>& errors)
    {
        assert(terminalNode != nullptr);

        Optional<long> result;

        if (!isError(terminalNode))
        {
            try
            {
                auto const text = terminalNode->getText();
                // TODO: use strtol instead of stol...
                auto const number = std::stol(text);

                result.emplace(number);
            }
            catch (std::out_of_range const& e)
            {
                auto error = errorFromToken(terminalNode->getSymbol(), "number '" + terminalNode->getText() + "' to large for an unsigned int");

                errors.emplace_back(std::move(error));
            }
        }

        return result;
    }

    Optional<float> getFloat(antlr4::tree::TerminalNode* const terminalNode, std::vector<Program::Error>& errors)
    {
        assert(terminalNode != nullptr);

        Optional<float> result;

        if (!isError(terminalNode))
        {
            try
            {
                auto const text = terminalNode->getText();

                // TODO: use std::strtof instead of std::stoul to avoid usage of exceptions.
                result.emplace(std::stof(text));
            }
            catch (std::out_of_range const& e)
            {
                auto error = errorFromToken(terminalNode->getSymbol(), "number '" + terminalNode->getText() + "' to large for an unsigned int");

                errors.emplace_back(std::move(error));
            }
        }
        return result;
    }
}