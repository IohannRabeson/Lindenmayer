//
// Created by Io on 2019-02-23.
//

#include "LCode/SymbolDefinitionPass.hpp"

SymbolDefinitionPass::SymbolDefinitionPass()
{
}

void SymbolDefinitionPass::enterProgram(LCodeParser::ProgramContext* programContext)
{
    auto* const globalScope = _scopeTree.reset();
    _annotations.put(programContext, globalScope);
    globalScope->storage().defineType("void");
    globalScope->storage().defineType("integer");
    globalScope->storage().defineType("float");
    globalScope->storage().defineType("boolean");
    globalScope->storage().defineType("string");
    _currentScope = globalScope;
}

void SymbolDefinitionPass::exitProgram(LCodeParser::ProgramContext*)
{
}

//static SymbolTable::StorageType getConstantType()

void SymbolDefinitionPass::enterConstantDecl(LCodeParser::ConstantDeclContext* constantDeclarationContext)
{
    auto const constantName = constantDeclarationContext->IDENTIFIER()->getText();

    if (isDefinedHereOrInParents(constantName, _currentScope))
    {
        // error name already used by an already existing symbol
    }

    auto const constantTypeName = constantDeclarationContext->type()->getText();

    if (!isDefinedHereOrInParents(constantTypeName, _currentScope))
    {
        // error constant type undefined
    }

}

void SymbolDefinitionPass::exitConstantDecl(LCodeParser::ConstantDeclContext*)
{

}

void SymbolDefinitionPass::enterAliasDecl(LCodeParser::AliasDeclContext*)
{

}

void SymbolDefinitionPass::exitAliasDecl(LCodeParser::AliasDeclContext*)
{

}

void SymbolDefinitionPass::pushScope(antlr4::tree::ParseTree* node)
{
    auto* newScope = _scopeTree.addChild(_currentScope);
    _annotations.put(node, newScope);
    _currentScope = newScope;
}

void SymbolDefinitionPass::popScope()
{
    _currentScope = _currentScope->parent();
}

bool SymbolDefinitionPass::isDefinedHereOrInParents(std::string const& symbolName, ScopeTree::Scope const* scope)
{
    Scope const* current = scope;
    while (current != nullptr)
    {
        if (current->storage().contains(symbolName))
        {
            return true;
        }
        current = current->parent();
    }
    return false;
}

bool SymbolDefinitionPass::checkTypeHereOrInParents(std::string const& symbolName, SymbolTable::SymbolType expectedType, ScopeTree::Scope const* scope)
{
    Scope const* current = scope;
    while (current != nullptr)
    {
        if (auto const type = current->storage().symbolType(symbolName); type != SymbolTable::SymbolType::Null)
        {
            return type == expectedType;
        }
        current = current->parent();
    }
    return false;
}
