//
// Created by Io on 2019-03-09.
//

#include "LCode/ScopeTreeBuilder.hpp"
#include "LCode/ParsingContext.hpp"

ScopeTreeBuilder::ScopeTreeBuilder(ParsingContext& context, ParseErrors& errors)
: _context(context)
, _errors(errors)
{
    static_cast<void>(_errors);
}

void ScopeTreeBuilder::enterProgram(LCodeParser::ProgramContext* context)
{
    _context._scope = std::make_unique<ParsingContext::ScopeNode>(context);
    _currentScope = _context._scope.get();

    auto& rootSymbolTable = _currentScope->value();
    rootSymbolTable.defineType("number", StorageType::Number);
}

void ScopeTreeBuilder::exitProgram(LCodeParser::ProgramContext*)
{
    std::function<bool(ParsingContext::ScopeNode*)> f([this](ParsingContext::ScopeNode* node) -> bool
    {
        _context._scopeByParseTree.emplace(node->parseTreeNode(), node);
        return true;
    });
    visitDepthFirstSearch(_context._scope.get(), std::move(f));
}

void ScopeTreeBuilder::enterRewriteRuleDecl(LCodeParser::RewriteRuleDeclContext* context)
{
    _currentScope = _currentScope->makeChild(context);
}

void ScopeTreeBuilder::exitRewriteRuleDecl(LCodeParser::RewriteRuleDeclContext*)
{
    _currentScope = _currentScope->parent();
}

void ScopeTreeBuilder::enterAliasDecl(LCodeParser::AliasDeclContext* context)
{
    _currentScope = _currentScope->makeChild(context);
}

void ScopeTreeBuilder::exitAliasDecl(LCodeParser::AliasDeclContext*)
{
    _currentScope = _currentScope->parent();
}
