//
// Created by Io on 2019-03-09.
//

#ifndef LINDENMAYER_CONTEXT_HPP
#define LINDENMAYER_CONTEXT_HPP
#include "LCode/SymbolTable.hpp"
#include "LCode/ScopeTree.hpp"
#include "LCode/AbstractSyntaxTreeNode.hpp"

class Context
{
public:
    using ScopeNode = ScopeTreeNode<SymbolTable>;

    std::unique_ptr<ProgramNode> _ast;
    std::unique_ptr<ScopeNode> _scope;
    std::map<antlr4::tree::ParseTree*, ScopeNode*> _scopeByParseTree;
};

bool getFunction(std::string const& identifier, Context::ScopeNode const* node, SymbolTable::FunctionSymbol& symbol);

#endif //LINDENMAYER_CONTEXT_HPP
