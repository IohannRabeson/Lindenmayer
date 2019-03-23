//
// Created by Io on 2019-03-09.
//

#include "LCode/LCodeScopeTreeBuilder.hpp"

LCodeScopeTreeBuilder::LCodeScopeTreeBuilder(LCodeScopeTree& scopeTree)
: _scopeTree(scopeTree)
, _currentScope(nullptr)
{
}

void LCodeScopeTreeBuilder::enterProgram(LCodeParser::ProgramContext* context)
{
    _currentScope = _scopeTree.addNode(context);

    auto& rootSymbolTable = _currentScope->value();
    rootSymbolTable.defineType("number", StorageType::Number);
}

void LCodeScopeTreeBuilder::exitProgram(LCodeParser::ProgramContext*)
{
}

void LCodeScopeTreeBuilder::enterRewriteRuleDecl(LCodeParser::RewriteRuleDeclContext* context)
{
    _currentScope = _currentScope->makeChild(context);
}

void LCodeScopeTreeBuilder::exitRewriteRuleDecl(LCodeParser::RewriteRuleDeclContext*)
{
    _currentScope = _currentScope->parent();
}

void LCodeScopeTreeBuilder::enterAliasDecl(LCodeParser::AliasDeclContext* context)
{
    _currentScope = _currentScope->makeChild(context);
}

void LCodeScopeTreeBuilder::exitAliasDecl(LCodeParser::AliasDeclContext*)
{
    _currentScope = _currentScope->parent();
}
