//
// Created by Io on 2019-03-02.
//

#ifndef LINDENMAYER_SCOPETREE_HPP
#define LINDENMAYER_SCOPETREE_HPP
#include <memory>
#include <vector>
#include "SymbolTable.hpp"

class ScopeTree
{
public:
    class Scope;
    using ScopePtr = std::unique_ptr<Scope>;

    Scope* root() const;
    Scope* reset();
    Scope* addChild(Scope* parent);

    bool define(Scope* scope, std::string const& identifier, SymbolTable::Symbol&& symbol);
    bool contains(Scope const* scope, std::string const& identifier, bool const includeParents) const;
    SymbolTable::SymbolType symbolType(Scope const* scope, std::string const& identifier) const;
private:
    std::unique_ptr<Scope> _root;
};

class ScopeTree::Scope
{
public:
    explicit Scope(Scope* parent);
    SymbolTable const& storage() const;
    SymbolTable& storage();
    Scope* parent() const;
    ScopePtr const& addChild();
    auto const& children() const;
private:
    std::vector<ScopePtr> _children;
    SymbolTable _storage;
    Scope* const _parent;
};

#endif //LINDENMAYER_SCOPETREE_HPP
