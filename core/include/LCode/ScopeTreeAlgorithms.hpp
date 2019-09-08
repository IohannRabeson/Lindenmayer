//
// Created by Io on 2019-03-10.
//

#ifndef LINDENMAYER_SCOPETREEALGORITHMS_HPP
#define LINDENMAYER_SCOPETREEALGORITHMS_HPP
#include <functional>

template <typename T>
class ScopeTreeNode;

template <typename T>
using ConstantScopeTreeValueVisitor = std::function<bool(T const&)>;

template <typename T>
using ScopeTreeValueVisitor = std::function<bool(T&)>&;

template <typename T>
using ConstScopeTreeValueVisitor = std::function<bool(T const&)>;

template <typename T>
using ScopeTreeVisitor = std::function<bool(ScopeTreeNode<T>*)>;

template <typename T>
using ConstantScopeTreeVisitor = std::function<bool(ScopeTreeNode<T> const*)>;

/*!
 * \brief Visit a constant scope tree node then visit his parent and so on recursively.
 * \tparam T Type deduced by the compiler. This is the type stored by the node passed as parameter.
 * \param node The node where visit starts.
 * \param visitor If the visitor return false, then the visit will be stopped, otherwise visit continue.
 */
template <typename T>
void visitBottomUp(ScopeTreeNode<T> const* node, ConstantScopeTreeValueVisitor<T>&& visitor)
{
    while (node != nullptr)
    {
        if (!visitor(node->value()))
        {
            return;
        }
        node = node->parent();
    }
}

/*!
 * \brief Visit a scope tree with DFS order.
 * \tparam T Type deduced by the compiler. This is the type stored by the node passed as parameter.
 * \param node The node where visit starts.
 * \param visitor If the visitor return false, then the visit will be stopped, otherwise visit continue.
 */
template <typename T>
void visitDepthFirstSearch(ScopeTreeNode<T>* node, ScopeTreeVisitor<T>&& visitor)
{
    using Node = ScopeTreeNode<T>;
    std::stack<Node*> stack;
    stack.push(node);
    while (!stack.empty())
    {
        auto* currentNode = stack.top();
        stack.pop();
        if (!visitor(currentNode))
        {
            return;
        }
        for (auto i = 0u; i < currentNode->childrenCount(); ++i)
        {
            stack.push(currentNode->getChild(i));
        }
    }
}

/*!
 * \brief Visit a constant scope tree with DFS order.
 * \tparam T Type deduced by the compiler. This is the type stored by the node passed as parameter.
 * \param node The node where visit starts.
 * \param visitor If the visitor return false, then the visit will be stopped, otherwise visit continue.
 */
template <typename T>
void visitDepthFirstSearch(ScopeTreeNode<T> const* node, ConstantScopeTreeVisitor<T>&& visitor)
{
    using Node = ScopeTreeNode<T>;
    std::stack<Node const*> stack;
    stack.push(node);
    while (!stack.empty())
    {
        auto* currentNode = stack.top();
        stack.pop();
        if (!visitor(currentNode))
        {
            return;
        }
        for (auto i = 0u; i < currentNode->childrenCount(); ++i)
        {
            stack.push(currentNode->getChild(i));
        }
    }
}

/*!
 * \brief Find a specific node using a custom predicate
 * \tparam T Type deduced by the compiler. This is the type stored by the node passed as parameter.
 * \param root The node where visit starts.
 * \param predicate Used to test each node. Return true if the node passed as parameter is the searched node.
 * \return The node if found otherwise null.
 */
template <typename T>
ScopeTreeNode<T> const* findIf(ScopeTreeNode<T> const* root, ConstScopeTreeValueVisitor<T>&& predicate)
{
	ScopeTreeNode<T> const* result = nullptr;
	ConstantScopeTreeVisitor<T> visitor = [&result, predicate](ScopeTreeNode<T> const* node) -> bool
	{
		if (predicate(node->value()))
		{
			result = node;
		}
		return false;
	};
	visitDepthFirstSearch(root, std::move(visitor));
	return result;
}

#endif //LINDENMAYER_SCOPETREEALGORITHMS_HPP
