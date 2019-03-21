//
// Created by Io on 2019-03-05.
//

#include "ParserUtility.hpp"
#include <generated/LCodeLexer.h>
#include <generated/LCodeParser.h>

void parseLCode(std::string const& text, antlr4::tree::ParseTreeListener& listener)
{
    antlr4::ANTLRInputStream inputStream(text);
    LCodeLexer lexer(&inputStream);
    antlr4::CommonTokenStream tokenStream(&lexer);
    LCodeParser parser(&tokenStream);
    antlr4::tree::ParseTreeWalker treeWalker;
    treeWalker.walk(&listener, parser.program());
}