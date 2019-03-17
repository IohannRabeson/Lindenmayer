//
// Created by Io on 2019-03-16.
//

#ifndef LINDENMAYER_LCODEPROGRAM_HPP
#define LINDENMAYER_LCODEPROGRAM_HPP
#include "LCode/ParseError.hpp"
#include "LCode/ScopeTree.hpp"
#include "LCode/SymbolTable.hpp"
#include "LCode/AbstractSyntaxTreeNode.hpp"

#include <memory>
#include <vector>
#include <string>

struct ProgramContent
{
    ParseErrors parseErrors;
    ScopeTree<SymbolTable> scopeTree;
    std::unique_ptr<ProgramNode> ast;
};

ProgramContent parseLCode(std::string const& text);

#endif //LINDENMAYER_LCODEPROGRAM_HPP
