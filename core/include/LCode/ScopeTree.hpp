//
// Created by Io on 2019-03-08.
//

#ifndef LINDENMAYER_SCOPETREE_HPP
#define LINDENMAYER_SCOPETREE_HPP
#include "SymbolTable.hpp"
#include "ScopeTreeAlgorithms.hpp"
#include <functional>
#include <vector>
#include <map>

/*!
 * \brief Generic data structure to simulate scopes
 * \tparam T Type of the instance stored in each scope
 */
template <typename T>
class ScopeTreeNode
{
    static_assert( std::is_default_constructible<T>::value, "T must be default constructible");
    using ThisType = ScopeTreeNode<T>;
public:
    explicit ScopeTreeNode(antlr4::tree::ParseTree* parseTreeNode = nullptr, ThisType* parent = nullptr)
        : _parent(parent)
        , _parseTreeNode(parseTreeNode)
    {
    }

    antlr4::tree::ParseTree* parseTreeNode() const
    {
        return _parseTreeNode;
    }

    ThisType* parent() const
    {
        return _parent;
    }

    ThisType* makeChild(antlr4::tree::ParseTree* parseTreeNode)
    {
        _children.emplace_back(std::make_unique<ThisType>(parseTreeNode, this));
        return _children.back().get();
    }

    std::size_t childrenCount() const
    {
        return _children.size();
    }

    ThisType* getChild(std::size_t index) const
    {
        assert( index < childrenCount() );
        return _children[index].get();
    }

    T& value() { return _value; }
    T const& value() const { return _value; }
private:
    ThisType* const _parent;
    antlr4::tree::ParseTree* const _parseTreeNode;
    std::vector<std::unique_ptr<ThisType>> _children;
    T _value;
};

template <typename T>
class ScopeTree
{
public:
    using NodeType = ScopeTreeNode<T>;

    void clear()
    {
        _root.reset();
        _nodesByParseTree.clear();
    }

    NodeType* root() const
    {
        return _root.get();
    }

    NodeType* findNode(antlr4::tree::ParseTree* parseTreeNode) const
    {
        auto it = _nodesByParseTree.find(parseTreeNode);

        return it != _nodesByParseTree.end() ? it->second : nullptr;
    }

    NodeType* addNode(antlr4::tree::ParseTree* parseTreeNode, NodeType* parent = nullptr)
    {
        NodeType* newNode = nullptr;
        if (parent == nullptr)
        {
            _root = std::make_unique<NodeType>(parseTreeNode);
            newNode = _root.get();
        }
        else
        {
            newNode = parent->makeChild(parseTreeNode);
        }
        _nodesByParseTree.emplace(parseTreeNode, newNode);

        return newNode;
    }
private:
    std::unique_ptr<NodeType> _root;
    std::map<antlr4::tree::ParseTree*, NodeType*> _nodesByParseTree;
};

#endif //LINDENMAYER_SCOPETREE_HPP
