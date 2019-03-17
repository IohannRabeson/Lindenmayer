//
// Created by Io on 2019-03-16.
//

#include "LCode/LCodeProgram.hpp"
#include "LCode/LCodeScopeTreeBuilder.hpp"
#include "LCode/AbstractSyntaxTreeBuilder.hpp"
#include <generated/LCodeParser.h>
#include <generated/LCodeLexer.h>
#include <antlr4-runtime.h>

ProgramContent parseLCode(std::string const& text)
{
    // Results
    ParseErrors parseErrors;
    LCodeScopeTree scopeTree;
    std::unique_ptr<ProgramNode> ast;
    // Antlr stuff
    antlr4::ANTLRInputStream inputStream(text);
    LCodeLexer lexer(&inputStream);
    antlr4::CommonTokenStream tokenStream(&lexer);
    LCodeParser parser(&tokenStream);
    antlr4::tree::ParseTreeWalker treeWalker;
    // Passes
    LCodeScopeTreeBuilder scopeTreeBuilder(scopeTree);
    AbstractSyntaxTreeBuilder astBuilder(scopeTree, parseErrors);

    // !! program() instanciate a new node each time so it's really really important to call it
    // only once and store the pointer returned !!
    // This pointer will be deleted when parser will be deleted.
    auto* programNode = parser.program();
    // We have to build the scope tree before building the AST
    treeWalker.walk(&scopeTreeBuilder, programNode);
    // Then with a scope tree we can build the AST
    treeWalker.walk(&astBuilder, programNode);
    // And store it in a context
    astBuilder.releaseAst(ast);
    return {std::move(parseErrors), std::move(scopeTree), std::move(ast)};
}