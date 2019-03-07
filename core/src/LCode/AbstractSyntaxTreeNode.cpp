//
// Created by Io on 2019-03-02.
//

#include "LCode/AbstractSyntaxTreeNode.hpp"

AbstractSyntaxTreeNode::AbstractSyntaxTreeNode(antlr4::tree::ParseTree* parseTreeNode)
: _parseTreeNode(parseTreeNode)
{
}

AbstractSyntaxTreeNode* AbstractSyntaxTreeNode::addChild(AbstractSyntaxTreePtr&& tree)
{
    _children.push_back(std::move(tree));
    return _children.back().get();
}

AbstractSyntaxTreeNode* AbstractSyntaxTreeNode::getChild(std::size_t index) const
{
    return _children.at(index).get();
}

void AbstractSyntaxTreeNode::replaceChild(std::size_t index, AbstractSyntaxTreePtr&& tree)
{
    _children.at(index) = std::move(tree);
}

antlr4::tree::ParseTree* AbstractSyntaxTreeNode::parseTreeNode() const
{
    return _parseTreeNode;
}

std::size_t AbstractSyntaxTreeNode::getChildrenCount() const
{
    return _children.size();
}

std::string const& AbstractSyntaxTreeNode::nodeTypeName(AbstractSyntaxTreeNode::NodeType nodeType)
{
    // To generate this shit use my script (and put enums in enum.txt:
    // python3 MakeEnumDictionary.py NodeTypeNames enum.txt AbstractSyntaxTreeNode::NodeType
    static std::map<AbstractSyntaxTreeNode::NodeType, std::string> const NodeTypeNames =
    {
        { AbstractSyntaxTreeNode::NodeType::Abstract, "Abstract" },
        { AbstractSyntaxTreeNode::NodeType::LiteralBoolean, "LiteralBoolean" },
        { AbstractSyntaxTreeNode::NodeType::LiteralInteger, "LiteralInteger" },
        { AbstractSyntaxTreeNode::NodeType::LiteralFloat, "LiteralFloat" },
        { AbstractSyntaxTreeNode::NodeType::LiteralString, "LiteralString" },
        { AbstractSyntaxTreeNode::NodeType::Assignation, "Assignation" },
        { AbstractSyntaxTreeNode::NodeType::Addition, "Addition" },
        { AbstractSyntaxTreeNode::NodeType::Substraction, "Substraction" },
        { AbstractSyntaxTreeNode::NodeType::Multiplication, "Multiplication" },
        { AbstractSyntaxTreeNode::NodeType::Division, "Division" },
        { AbstractSyntaxTreeNode::NodeType::Negative, "Negative" },
        { AbstractSyntaxTreeNode::NodeType::Program, "Program" },
        { AbstractSyntaxTreeNode::NodeType::ConstantDeclaration, "ConstantDeclaration" },
        { AbstractSyntaxTreeNode::NodeType::AliasDeclaration, "AliasDeclaration" },
        { AbstractSyntaxTreeNode::NodeType::AxiomDeclaration, "AxiomDeclaration" },
        { AbstractSyntaxTreeNode::NodeType::RewriteRuleDeclaration, "RewriteRuleDeclaration" },
    };
    return NodeTypeNames.at(nodeType);
}

ExpressionNode const* ExpressionNode::getExpressionChild(std::size_t index) const
{
    return dynamic_cast<ExpressionNode const*>(getChild(index));
}

StorageType ExpressionNode::getEvaluatedTypeChild(std::size_t index) const
{
    auto* node = getExpressionChild(index);
    return node == nullptr ? StorageType::Null : node->evaluatedType();
}

StorageType NegativeNode::evaluatedType() const
{
    return getEvaluatedTypeChild(0u);
}

bool NegativeNode::areEqual(AbstractSyntaxTreeNode const* other) const
{
    return other->nodeType() == nodeType();
}

AbstractSyntaxTreeNode::NodeType NegativeNode::nodeType() const
{
    return AbstractSyntaxTreeNode::NodeType::Negative;
}

BinaryOperatorNode::BinaryOperatorNode(antlr4::tree::ParseTree* parseTreeNode)
    : ExpressionNode(parseTreeNode)
{
}

StorageType BinaryOperatorNode::evaluatedType() const
{
    assert( getChildrenCount() == 2u );
    auto const leftTypeId = static_cast<std::size_t>(getEvaluatedTypeChild(0u));
    auto const rightTypeId = static_cast<std::size_t>(getEvaluatedTypeChild(1u));
    auto const maxTypeId = std::max(leftTypeId, rightTypeId);
    return static_cast<StorageType>(maxTypeId);
}

AbstractSyntaxTreeNode::NodeType AdditionNode::nodeType() const
{
    return AbstractSyntaxTreeNode::NodeType::Addition;
}

AbstractSyntaxTreeNode::NodeType SubstractionNode::nodeType() const
{
    return AbstractSyntaxTreeNode::NodeType::Substraction;
}

AbstractSyntaxTreeNode::NodeType MultiplicationNode::nodeType() const
{
    return AbstractSyntaxTreeNode::NodeType::Multiplication;
}

AbstractSyntaxTreeNode::NodeType DivisionNode::nodeType() const
{
    return AbstractSyntaxTreeNode::NodeType::Division;
}

AbstractSyntaxTreeNode::NodeType ProgramNode::nodeType() const
{
    return NodeType::Program;
}

AbstractSyntaxTreeNode::NodeType ConstantDeclarationNode::nodeType() const
{
    return NodeType::ConstantDeclaration;
}

AbstractSyntaxTreeNode::NodeType AliasDeclarationNode::nodeType() const
{
    return NodeType::AliasDeclaration;
}

AbstractSyntaxTreeNode::NodeType AxiomDeclarationNode::nodeType() const
{
    return NodeType::AxiomDeclaration;
}

AbstractSyntaxTreeNode::NodeType RewriteRuleDeclaratinoNode::nodeType() const
{
    return NodeType::RewriteRuleDeclaration;
}

AbstractSyntaxTreeNode::NodeType AssignationNode::nodeType() const
{
    return NodeType::Assignation;
}


IdentifierNode::IdentifierNode(std::string const& identifier)
    : IdentifierNode(nullptr, identifier)
{
}

IdentifierNode::IdentifierNode(antlr4::tree::ParseTree* parserTreeNode, std::string const& identifier)
: ExpressionNode(parserTreeNode)
, _identifier(identifier)
{
}

AbstractSyntaxTreeNode::NodeType IdentifierNode::nodeType() const
{
    return NodeType::Identifier;
}
