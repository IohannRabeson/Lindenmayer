//
// Created by Io on 2019-02-23.
//

#ifndef LINDENMAYER_LCODESYMBOLDEFINITIONPASS_HPP
#define LINDENMAYER_LCODESYMBOLDEFINITIONPASS_HPP
#include <generated/LCodeBaseListener.h>
#include <antlr4-runtime.h>
#include "SymbolTable.hpp"
#include "ScopeTree.hpp"
#include <memory>
#include <stack>

class SymbolDefinitionPass : public LCodeBaseListener
{
    using ScopeTree = ScopeTree;
    using Scope = ScopeTree::Scope;
    using ScopePtr = ScopeTree::ScopePtr;

    ScopeTree _scopeTree;
    antlr4::tree::ParseTreeProperty<Scope*> _annotations;
    Scope* _currentScope = nullptr;

    static bool isDefinedHereOrInParents(std::string const& symbolName, Scope const* scope);
    static bool checkTypeHereOrInParents(std::string const& symbolName, SymbolTable::SymbolType expectedType, Scope const* scope);
public:
    explicit SymbolDefinitionPass();

    void enterProgram(LCodeParser::ProgramContext* ctx) override;
    void exitProgram(LCodeParser::ProgramContext* ctx) override;
    void enterConstantDecl(LCodeParser::ConstantDeclContext* ctx) override;
    void exitConstantDecl(LCodeParser::ConstantDeclContext* ctx) override;
    void enterAliasDecl(LCodeParser::AliasDeclContext* ctx) override;
    void exitAliasDecl(LCodeParser::AliasDeclContext* ctx) override;

    ScopeTree const& scopeTree() const
    {
        return _scopeTree;
    }
private:
    void pushScope(antlr4::tree::ParseTree* node);
    void popScope();

    ScopePtr const& currentScope() const;
};

#endif //LINDENMAYER_LCODESYMBOLDEFINITIONPASS_HPP
