//
// Created by Io on 2019-03-17.
//

#include "ParseErrorListener.hpp"

ParseErrorListener::ParseErrorListener(ParseErrors& errors)
: _errors(errors)
{
}

void ParseErrorListener::syntaxError(antlr4::Recognizer*, antlr4::Token*, size_t line, size_t charPositionInLine, const std::string& msg, std::exception_ptr)
{
    pushError(_errors, ParseError::Type::Error, line, charPositionInLine, msg);
}


