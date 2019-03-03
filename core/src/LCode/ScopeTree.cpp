//
// Created by Io on 2019-03-02.
//

#include "LCode/ScopeTree.hpp"

ScopeTree::Scope::Scope(Scope* parent)
: _parent(parent)
{
}

SymbolTable const& ScopeTree::Scope::storage() const
{
    return _storage;
}

SymbolTable& ScopeTree::Scope::storage()
{
    return _storage;
}

ScopeTree::Scope* ScopeTree::Scope::parent() const
{
    return _parent;
}

ScopeTree::ScopePtr const& ScopeTree::Scope::addChild()
{
    _children.emplace_back(std::make_unique<Scope>(this));
    return _children.back();
}

auto const& ScopeTree::Scope::children() const
{
    return _children;
}

//
// class ScopeTree
//

ScopeTree::Scope* ScopeTree::addChild(ScopeTree::Scope* parent)
{
    return parent->addChild().get();
}

ScopeTree::Scope* ScopeTree::root() const
{
    return _root.get();
}

ScopeTree::Scope* ScopeTree::reset()
{
    _root = std::make_unique<Scope>(nullptr);
    return _root.get();
}

bool ScopeTree::contains(ScopeTree::Scope const* scope, std::string const& identifier, bool const includeParents) const
{
    assert( scope != nullptr );
    if (includeParents)
    {
        auto const* current = scope;
        while (current != nullptr)
        {
            if (current->storage().contains(identifier))
            {
                return true;
            }
            current = current->parent();
        }
        return false;
    }
    else
    {
        return scope->storage().contains(identifier);
    }
}

SymbolTable::SymbolType ScopeTree::symbolType(ScopeTree::Scope const* scope, std::string const& identifier) const
{
    assert( scope != nullptr );

    return scope->storage().symbolType(identifier);
}

bool ScopeTree::define(ScopeTree::Scope* scope, std::string const& identifier, SymbolTable::Symbol&& symbol)
{
    assert( scope != nullptr );

    return scope->storage().define(identifier, std::move(symbol));
}
