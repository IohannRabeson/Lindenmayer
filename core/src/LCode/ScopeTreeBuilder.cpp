//
// Created by Io on 2019-03-09.
//

#include "LCode/ScopeTreeBuilder.hpp"
#include "LCode/Context.hpp"

ScopeTreeBuilder::ScopeTreeBuilder(Context& context)
: _context(context)
{
}

void ScopeTreeBuilder::enterProgram(LCodeParser::ProgramContext* context)
{
    _context._scope = std::make_unique<Context::ScopeNode>(context);
    _currentScope = _context._scope.get();
    auto& rootSymbolTable = _currentScope->value();
    rootSymbolTable.defineType("number", StorageType::Number);
}

void ScopeTreeBuilder::exitProgram(LCodeParser::ProgramContext*)
{
    std::function<bool(Context::ScopeNode*)> f([this](Context::ScopeNode* node) -> bool
    {
        _context._scopeByParseTree[node->parseTreeNode()] = node;
        return true;
    });

    visitDepthFirstSearch(_context._scope.get(), std::move(f));
}

void ScopeTreeBuilder::enterRewriteRuleDecl(LCodeParser::RewriteRuleDeclContext* context)
{
    _currentScope = _currentScope->makeChild(context);
}

void ScopeTreeBuilder::enterAliasDecl(LCodeParser::AliasDeclContext* context)
{
    _currentScope = _currentScope->makeChild(context);
}
