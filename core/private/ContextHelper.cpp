//
// Created by Io on 28/04/2018.
//

#include "ContextHelper.hpp"

namespace lcode::ContextHelper
{
    Program::Error makeError(antlr4::tree::TerminalNode* node, std::string const& message)
    {
        return makeError(node->getSymbol(), message);
    }

    /*!
     * \brief Construct an error
     * \param token Token with something wrong
     * \param message Message to display. You can use the placeholder '%t' replaced by the token text.
     * \return
     */
    Program::Error makeError(antlr4::Token* token, std::string message)
    {
        Program::Error error;

        static std::string const TextTag = "%t";
        auto const textTagPosition = message.find(TextTag);

        if (textTagPosition != std::string::npos)
        {
            auto const tokenText = token->getText();

            message.replace(textTagPosition, TextTag.size(), tokenText);
        }

        error.charIndex = token->getCharPositionInLine();
        error.line = token->getLine();
        error.message = std::move(message);
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
            errors.emplace_back(makeError(context->Identifier(), "Invalid identifier '%t'"));
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
                errors.emplace_back(makeError(terminalNode, "number '%t' to large for an unsigned int"));
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
                auto error = makeError(terminalNode, "number '%t' to large for an unsigned int");

                errors.emplace_back(std::move(error));
            }
        }
        return result;
    }
}