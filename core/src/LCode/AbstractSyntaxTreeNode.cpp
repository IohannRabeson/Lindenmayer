//
// Created by Io on 2019-03-02.
//

#include "LCode/AbstractSyntaxTreeNode.hpp"
#include <cmath>

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
        { AbstractSyntaxTreeNode::NodeType::Number, "Number" },
        { AbstractSyntaxTreeNode::NodeType::Constant, "Constant" },
        { AbstractSyntaxTreeNode::NodeType::Assignation, "Assignation" },
        { AbstractSyntaxTreeNode::NodeType::Program, "Program" },
        { AbstractSyntaxTreeNode::NodeType::ConstantDeclaration, "ConstantDeclaration" },
        { AbstractSyntaxTreeNode::NodeType::AliasDeclaration, "AliasDeclaration" },
        { AbstractSyntaxTreeNode::NodeType::AxiomDeclaration, "AxiomDeclaration" },
        { AbstractSyntaxTreeNode::NodeType::RewriteRuleDeclaration, "RewriteRuleDeclaration" },
        { AbstractSyntaxTreeNode::NodeType::FunctionCall, "FunctionCall" },
        { AbstractSyntaxTreeNode::NodeType::Addition, "Addition" },
        { AbstractSyntaxTreeNode::NodeType::Substraction, "Substraction" },
        { AbstractSyntaxTreeNode::NodeType::Multiplication, "Multiplication" },
        { AbstractSyntaxTreeNode::NodeType::Division, "Division" },
        { AbstractSyntaxTreeNode::NodeType::Negative, "Negative" }
    };
    return NodeTypeNames.at(nodeType);
}

bool AbstractSyntaxTreeNode::isBinaryOperator(NodeType nodeType)
{
    switch (nodeType)
    {
        case AbstractSyntaxTreeNode::NodeType::Addition:
        case AbstractSyntaxTreeNode::NodeType::Substraction:
        case AbstractSyntaxTreeNode::NodeType::Multiplication:
        case AbstractSyntaxTreeNode::NodeType::Division:
            return true;
        default:
            return false;
    }
}

bool AbstractSyntaxTreeNode::areEqual(AbstractSyntaxTreeNode const* other) const
{
    return nodeType() == other->nodeType();
}

auto AbstractSyntaxTreeNode::begin() const
{
    return _children.begin();
}

auto AbstractSyntaxTreeNode::end() const
{
    return _children.end();
}

ExpressionNode const* ExpressionNode::getExpressionChild(std::size_t index) const
{
    return dynamic_cast<ExpressionNode const*>(getChild(index));
}

StorageType ExpressionNode::getEvaluatedTypeChild(std::size_t index) const
{
    auto* const node = getExpressionChild(index);
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

auto NegativeNode::evaluateUnaryOperation(ExpressionNode::NumberType value) const -> NumberType
{
    return -value;
}

StorageType BinaryOperatorNode::evaluatedType() const
{
    assert( getChildrenCount() == 2u );
    auto const leftTypeId = static_cast<std::size_t>(getEvaluatedTypeChild(0u));
    auto const rightTypeId = static_cast<std::size_t>(getEvaluatedTypeChild(1u));
    auto const maxTypeId = std::max(leftTypeId, rightTypeId);
    return static_cast<StorageType>(maxTypeId);
}

auto BinaryOperatorNode::evaluateNumber() const -> NumberType
{
    auto const* leftNode = getExpressionChild(0);
    auto const* rightNode = getExpressionChild(1);

    return evaluateBinaryOperation(leftNode->evaluateNumber(), rightNode->evaluateNumber());
}

auto AdditionNode::nodeType() const -> NodeType
{
    return AbstractSyntaxTreeNode::NodeType::Addition;
}

ExpressionNode::NumberType AdditionNode::evaluateBinaryOperation(ExpressionNode::NumberType left, ExpressionNode::NumberType right) const
{
    return left + right;
}

AbstractSyntaxTreeNode::NodeType SubstractionNode::nodeType() const
{
    return AbstractSyntaxTreeNode::NodeType::Substraction;
}

ExpressionNode::NumberType SubstractionNode::evaluateBinaryOperation(ExpressionNode::NumberType left, ExpressionNode::NumberType right) const
{
    return left - right;
}

AbstractSyntaxTreeNode::NodeType MultiplicationNode::nodeType() const
{
    return AbstractSyntaxTreeNode::NodeType::Multiplication;
}

ExpressionNode::NumberType MultiplicationNode::evaluateBinaryOperation(ExpressionNode::NumberType left, ExpressionNode::NumberType right) const
{
    return left * right;
}

AbstractSyntaxTreeNode::NodeType DivisionNode::nodeType() const
{
    return AbstractSyntaxTreeNode::NodeType::Division;
}

ExpressionNode::NumberType DivisionNode::evaluateBinaryOperation(ExpressionNode::NumberType left, ExpressionNode::NumberType right) const
{
    return left / right;
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


ConstantNumberNode::ConstantNumberNode(std::string const& identifier, SymbolTable const& symbolTable)
: ConstantNumberNode(nullptr, identifier, symbolTable)
{
}

ConstantNumberNode::ConstantNumberNode(antlr4::tree::ParseTree* parserTreeNode, std::string const& identifier, SymbolTable const& symbolTable)
: ExpressionNode(parserTreeNode)
, _identifier(identifier)
, _symbolTable(symbolTable)
{
}

AbstractSyntaxTreeNode::NodeType ConstantNumberNode::nodeType() const
{
    return NodeType::Constant;
}

StorageType ConstantNumberNode::evaluatedType() const
{
    return StorageType::Number;
}

ExpressionNode::NumberType ConstantNumberNode::evaluateNumber() const
{
    assert( _symbolTable.isConstantDefined(_identifier) );
    return _symbolTable.getConstant(_identifier)._value;
}

FunctionCallNode::FunctionCallNode(antlr4::tree::ParseTree* const parseTree, std::string const& identifier, SymbolTable::FunctionSymbol const& symbol)
    : ExpressionNode(parseTree)
    , _identifier(identifier)
    , _symbol(symbol)
{
}

FunctionCallNode::FunctionCallNode(std::string const& identifier, SymbolTable::FunctionSymbol const& symbol)
    : _identifier(identifier)
    , _symbol(symbol)
{
}

StorageType FunctionCallNode::evaluatedType() const
{
    return _symbol._returnType;
}

AbstractSyntaxTreeNode::NodeType FunctionCallNode::nodeType() const
{
    return NodeType::FunctionCall;
}

bool FunctionCallNode::areEqual(AbstractSyntaxTreeNode const* other) const
{
    if (nodeType() == other->nodeType())
    {
        auto* const otherNode = static_cast<FunctionCallNode const*>(other);

        return _identifier == otherNode->_identifier;
    }
    return false;
}

ExpressionNode::NumberType FunctionCallNode::evaluateNumber() const
{
    auto result = 0;


    return result;
}

auto LiteralNumberNode::evaluateNumber() const -> NumberType
{

    return value();
}

bool LiteralNumberNode::areEqual(AbstractSyntaxTreeNode const* other) const
{
    if (other == nullptr || other->nodeType() != nodeType())
    {
        return false;
    }
    auto const otherValue = static_cast<LiteralNumberNode const*>(other)->value();
    return std::abs(otherValue - value()) < std::numeric_limits<ValueType>::epsilon();
}

StorageType UnaryOperatorNode::evaluatedType() const
{
    return getEvaluatedTypeChild(0);
}

ExpressionNode::NumberType UnaryOperatorNode::evaluateNumber() const
{
    auto const* childNode = getExpressionChild(0);

    return evaluateUnaryOperation(childNode->evaluateNumber());
}
