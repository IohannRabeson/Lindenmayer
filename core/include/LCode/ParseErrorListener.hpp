//
// Created by Io on 2019-03-17.
//

#ifndef LINDENMAYER_LCODELEXICALERRORPASS_HPP
#define LINDENMAYER_LCODELEXICALERRORPASS_HPP
#include <antlr4-runtime.h>
#include "LCode/ParseError.hpp"

class ParseErrorListener : public antlr4::BaseErrorListener
{
public:
    explicit ParseErrorListener(ParseErrors& errors);

private:
    void syntaxError(antlr4::Recognizer* recognizer, antlr4::Token* offendingSymbol, size_t line, size_t charPositionInLine, const std::string& msg, std::exception_ptr e) override;

private:
    ParseErrors& _errors;
};

#endif //LINDENMAYER_LCODELEXICALERRORPASS_HPP
