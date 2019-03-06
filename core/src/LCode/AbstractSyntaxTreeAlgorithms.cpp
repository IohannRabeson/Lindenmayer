//
// Created by Io on 2019-03-05.
//

#include "LCode/AbstractSyntaxTreeAlgorithms.hpp"
#include "LCode/AbstractSyntaxTreeNode.hpp"

#include <stack>
#include <set>

namespace
{
    void visitDepthFirstSearch(AbstractSyntaxTreeNode const* root, std::function<void(AbstractSyntaxTreeNode const*)> const& f)
    {
        std::stack<AbstractSyntaxTreeNode const*> stack;
        std::set<AbstractSyntaxTreeNode const*> visited;
        stack.push(root);
        while (!stack.empty())
        {
            AbstractSyntaxTreeNode const* currentNode = stack.top();
            stack.pop();
            if (visited.insert(currentNode).second)
            {
                f(currentNode);
                for (auto i = 0u; i < currentNode->getChildrenCount(); ++i)
                {
                    stack.push(currentNode->getChild(i));
                }
            }
        }
    }
}

class GatherNode
{
    std::vector<AbstractSyntaxTreeNode const*>& _nodes;
public:
    explicit GatherNode(std::vector<AbstractSyntaxTreeNode const*>& nodes)
    : _nodes(nodes)
    {
    }

    void operator()(AbstractSyntaxTreeNode const* node)
    {
        _nodes.push_back(node);
    }
};

bool compareTrees(AbstractSyntaxTreeNode const* left, AbstractSyntaxTreeNode const* right)
{
    std::vector<AbstractSyntaxTreeNode const*> leftNodes;
    std::vector<AbstractSyntaxTreeNode const*> rightNodes;
    visitDepthFirstSearch(left, GatherNode(leftNodes));
    visitDepthFirstSearch(right, GatherNode(rightNodes));
    if (leftNodes.size() != rightNodes.size())
    {
        return false;
    }
    auto leftIt = leftNodes.begin();
    auto rightIt = rightNodes.begin();
    while (leftIt != leftNodes.end() && rightIt != rightNodes.end())
    {
        auto const* leftNode = *leftIt;
        auto const* rightNode = *rightIt;

        if (!leftNode->areEqual(rightNode))
        {
            return false;
        }
        ++leftIt;
        ++rightIt;
    }
    return true;
}

void exportDot(AbstractSyntaxTreeNode const* tree, std::string const& name, std::ostream& os)
{
    std::map<AbstractSyntaxTreeNode const*, std::size_t> nodeIdentifiers;
    visitDepthFirstSearch(tree, [&nodeIdentifiers](AbstractSyntaxTreeNode const* node)
    {
        nodeIdentifiers.emplace(node, nodeIdentifiers.size());
    });
    os << "digraph " << name << "{\n";
    for (auto const& nodeId : nodeIdentifiers)
    {
        os << nodeId.second << " [shape=circle, label='" << AbstractSyntaxTreeNode::nodeTypeName(nodeId.first->nodeType()) << "']\n";
    }
    for (auto const& nodeId : nodeIdentifiers)
    {
        for (auto i = 0u; i < nodeId.first->getChildrenCount(); ++i)
        {
            auto* const childNode = nodeId.first->getChild(i);
            os << nodeId.second << " -> " << nodeIdentifiers[childNode] << "\n";
        }
    }
    os << "}";
}
