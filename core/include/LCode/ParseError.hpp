//
// Created by Io on 2019-03-16.
//

#ifndef LINDENMAYER_ERROR_HPP
#define LINDENMAYER_ERROR_HPP
#include <string>
#include <vector>
#include <antlr4-runtime.h>
#include <LCode/StringFormat.hpp>

struct ParseError
{
    static constexpr std::size_t const Null = std::numeric_limits<std::size_t>::max();

    enum class Type
    {
        Error,
        Warning,
        Info
    };

    explicit ParseError(std::string const& message, Type type);
    explicit ParseError(std::string const& message, std::size_t line, std::size_t absolutePosition, Type type);

    std::string message;
    std::size_t line;
    std::size_t absolutePosition;
    Type type;
};

using ParseErrors = std::vector<ParseError>;

bool containsErrors(ParseErrors const& errors, ParseError::Type type);

template <typename ... TS>
void pushError(ParseErrors& errors, ParseError::Type type, std::size_t line, std::size_t absolutePosition, std::string const& format, TS&&... args)
{
    auto message = formatText(format, std::forward<TS>(args)...);
    errors.emplace_back(message, line, absolutePosition, type);
}

template <typename ... TS>
void pushError(ParseErrors& errors, ParseError::Type type, antlr4::ParserRuleContext* ruleContext, std::string const& format, TS&&... args)
{
    auto const absolutePosition = ruleContext->start->getCharPositionInLine();
    auto const line = ruleContext->start->getLine();
    pushError(errors, type, line, absolutePosition, format, std::forward<TS>(args)...);
}

#endif //LINDENMAYER_ERROR_HPP
