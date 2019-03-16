//
// Created by Io on 2019-03-16.
//

#include "LCode/LCodeProgram.hpp"
#include "LCode/ScopeTreeBuilder.hpp"
#include "LCode/AbstractSyntaxTreeBuilder.hpp"
#include <generated/LCodeParser.h>
#include <generated/LCodeLexer.h>
#include <antlr4-runtime.h>

class LCodeProgramImp
{
public:
    LCodeProgram::Content _content;
};

ProgramContent parseLCode(std::string const& text)
{
    ParsingContext parsingContext;
    ParseErrors parseErrors;
    antlr4::ANTLRInputStream inputStream(text);
    LCodeLexer lexer(&inputStream);
    antlr4::CommonTokenStream tokenStream(&lexer);
    LCodeParser parser(&tokenStream);
    antlr4::tree::ParseTreeWalker treeWalker;
    ScopeTreeBuilder scopeTreeBuilder(parsingContext, parseErrors);
    AbstractSyntaxTreeBuilder astBuilder(parsingContext._scopeByParseTree, parseErrors);

    // /!\ program() instanciate a new node each time so it's really really important to call it
    // only once and store the pointer returned /!\
    // This pointer will be deleted when parser will be deleted.
    auto* programNode = parser.program();
    // We have to build the scope tree before building the AST
    treeWalker.walk(&scopeTreeBuilder, programNode);
    // Then with a scope tree we can build the AST
    treeWalker.walk(&astBuilder, programNode);
    // And store it in a context
    astBuilder.releaseAst(parsingContext._ast);
    return {std::move(parsingContext), std::move(parseErrors)};
}

LCodeProgram::LCodeProgram()
: _imp(std::make_unique<LCodeProgramImp>())
{
}

LCodeProgram::~LCodeProgram()
{
}

LCodeProgram::Content const& LCodeProgram::parse(std::string const& text)
{
    ParsingContext parsingContext;
    ParseErrors parseErrors;
    antlr4::ANTLRInputStream inputStream(text);
    LCodeLexer lexer(&inputStream);
    antlr4::CommonTokenStream tokenStream(&lexer);
    LCodeParser parser(&tokenStream);
    antlr4::tree::ParseTreeWalker treeWalker;
    ScopeTreeBuilder scopeTreeBuilder(parsingContext, parseErrors);
    AbstractSyntaxTreeBuilder astBuilder(parsingContext._scopeByParseTree, parseErrors);

    // /!\ program() instanciate a new node each time so it's really really important to call it
    // only once and store the pointer returned /!\
    // This pointer will be deleted when parser will be deleted.
    auto* programNode = parser.program();
    // We have to build the scope tree before building the AST
    treeWalker.walk(&scopeTreeBuilder, programNode);
    // Then with a scope tree we can build the AST
    treeWalker.walk(&astBuilder, programNode);
    // And store it in a context
    astBuilder.releaseAst(parsingContext._ast);
    _imp->_content.parsingContext = std::move(parsingContext);
    _imp->_content.parseErrors = std::move(parseErrors);
    return _imp->_content;
}
