//
// Created by Io on 2019-03-02.
//

#ifndef LINDENMAYER_ABSTRACTSYNTAXTREE_HPP
#define LINDENMAYER_ABSTRACTSYNTAXTREE_HPP
#include <antlr4-runtime.h>
#include <limits>
#include "StorageType.hpp"
#include "SymbolTable.hpp"

class AbstractSyntaxTreeNode
{
    using AbstractSyntaxTreePtr = std::unique_ptr<AbstractSyntaxTreeNode>;
public:
    enum class NodeType
    {
        Abstract,
        Number,
        Constant,
        Assignation,
        Program,
        ConstantDeclaration,
        AliasDeclaration,
        AxiomDeclaration,
        RewriteRuleDeclaration,
        FunctionCall,
        // Binary operators
        Addition,
        Substraction,
        Multiplication,
        Division,
        // Unary operators
        Negative,
    };

    static std::string const& nodeTypeName(NodeType nodeType);
    static bool isBinaryOperator(NodeType nodeType);
    static constexpr NodeType const Type = NodeType::Abstract;

    explicit AbstractSyntaxTreeNode(antlr4::tree::ParseTree* parseTreeNode = nullptr);
    virtual ~AbstractSyntaxTreeNode() = default;
    virtual NodeType nodeType() const = 0;
    virtual bool areEqual(AbstractSyntaxTreeNode const* other) const;

    antlr4::tree::ParseTree* parseTreeNode() const;

    template <typename T, typename ... A>
    AbstractSyntaxTreeNode* makeChild(A&&... args)
    {
        return addChild(std::make_unique<T>(std::forward<A>(args)...));
    }

    AbstractSyntaxTreeNode* addChild(AbstractSyntaxTreePtr&& tree);
    AbstractSyntaxTreeNode* getChild(std::size_t index) const;
    void replaceChild(std::size_t index, AbstractSyntaxTreePtr&& tree);
    std::size_t getChildrenCount() const;
    auto begin() const;
    auto end() const;
private:
    std::vector<AbstractSyntaxTreePtr> _children;
    antlr4::tree::ParseTree* const _parseTreeNode;
};

class StatementNode : public AbstractSyntaxTreeNode
{
public:
    using AbstractSyntaxTreeNode::AbstractSyntaxTreeNode;
};

class ProgramNode : public StatementNode
{
public:
    using StatementNode::StatementNode;
    NodeType nodeType() const override;
};

class ConstantDeclarationNode : public StatementNode
{
public:
    using StatementNode::StatementNode;
    NodeType nodeType() const override;
};

class AliasDeclarationNode : public StatementNode
{
public:
    using StatementNode::StatementNode;
    NodeType nodeType() const override;
};

class AxiomDeclarationNode : public StatementNode
{
public:
    using StatementNode::StatementNode;
    NodeType nodeType() const override;
};

class RewriteRuleDeclaratinoNode : public StatementNode
{
public:
    using StatementNode::StatementNode;
    NodeType nodeType() const override;
};

class AssignationNode : public StatementNode
{
public:
    using StatementNode::StatementNode;
    NodeType nodeType() const override;
};

class ExpressionNode : public AbstractSyntaxTreeNode
{
protected:
    using NumberType = StorageTypeTrait<StorageType::Number>::Type;

    ExpressionNode const* getExpressionChild(std::size_t index) const;
    StorageType getEvaluatedTypeChild(std::size_t index) const;
public:
    using AbstractSyntaxTreeNode::AbstractSyntaxTreeNode;
    virtual StorageType evaluatedType() const = 0;
    virtual NumberType evaluateNumber() const = 0;
};

template <StorageType StorageTypeId, AbstractSyntaxTreeNode::NodeType NodeTypeId>
class LiteralNode : public ExpressionNode
{
    using ThisType = LiteralNode<StorageTypeId, NodeTypeId>;
public:
    using ValueType = CppType<StorageTypeId>;

    explicit LiteralNode(ValueType value)
    : ExpressionNode(nullptr)
    , _value(value)
    {
    }

    LiteralNode(antlr4::tree::ParseTree* parserTreeNode, ValueType value)
    : ExpressionNode(parserTreeNode)
    , _value(value)
    {
    }

    ValueType const& value() const
    {
        return _value;
    }

    StorageType evaluatedType() const override { return StorageTypeId; }
    NodeType nodeType() const override { return NodeTypeId; }
    bool areEqual(AbstractSyntaxTreeNode const* other) const override
    {
        if (other == nullptr || other->nodeType() != nodeType())
        {
            return false;
        }
        return static_cast<ThisType const*>(other)->_value == _value;
    }
private:
    ValueType const _value;
};

class LiteralNumberNode : public LiteralNode<StorageType::Number, AbstractSyntaxTreeNode::NodeType::Number>
{
public:
    using LiteralNode::LiteralNode;

    NumberType evaluateNumber() const override;
    bool areEqual(AbstractSyntaxTreeNode const* other) const override;
};

class ConstantNumberNode : public ExpressionNode
{
    std::string const _identifier;
    SymbolTable const& _symbolTable;
public:
    explicit ConstantNumberNode(std::string const& identifier, SymbolTable const& symbolTable);
    ConstantNumberNode(antlr4::tree::ParseTree* parserTreeNode, std::string const& identifier, SymbolTable const& symbolTable);
    StorageType evaluatedType() const override;
    NumberType evaluateNumber() const override;
    NodeType nodeType() const override;
};

class UnaryOperatorNode : public ExpressionNode
{
public:
    using ExpressionNode::ExpressionNode;

    StorageType evaluatedType() const override;
    NumberType evaluateNumber() const override;
private:
    virtual NumberType evaluateUnaryOperation(NumberType number) const = 0;
};

class BinaryOperatorNode : public ExpressionNode
{
public:
    using ExpressionNode::ExpressionNode;

    StorageType evaluatedType() const override;
    NumberType evaluateNumber() const override;
private:
    virtual NumberType evaluateBinaryOperation(NumberType left, NumberType right) const = 0;
};

class AdditionNode : public BinaryOperatorNode
{
public:
    using BinaryOperatorNode::BinaryOperatorNode;
    NodeType nodeType() const override;
    NumberType evaluateBinaryOperation(NumberType left, NumberType right) const override;
};

class SubstractionNode : public BinaryOperatorNode
{
public:
    using BinaryOperatorNode::BinaryOperatorNode;
    NodeType nodeType() const override;
    NumberType evaluateBinaryOperation(NumberType left, NumberType right) const override;
};

class MultiplicationNode : public BinaryOperatorNode
{
public:
    using BinaryOperatorNode::BinaryOperatorNode;
    NodeType nodeType() const override;
    NumberType evaluateBinaryOperation(NumberType left, NumberType right) const override;
};

class DivisionNode : public BinaryOperatorNode
{
public:
    using BinaryOperatorNode::BinaryOperatorNode;
    NodeType nodeType() const override;
    NumberType evaluateBinaryOperation(NumberType left, NumberType right) const override;
};

class NegativeNode : public UnaryOperatorNode
{
public:
    using UnaryOperatorNode::UnaryOperatorNode;
    StorageType evaluatedType() const override;
    NodeType nodeType() const override;
    bool areEqual(AbstractSyntaxTreeNode const* other) const override;
    NumberType evaluateUnaryOperation(NumberType value) const override;
};

class FunctionCallNode : public ExpressionNode
{
    std::string const _identifier;
    SymbolTable::FunctionSymbol const& _symbol;
public:
    FunctionCallNode(antlr4::tree::ParseTree* const parseTree, std::string const& identifier, SymbolTable::FunctionSymbol const& symbol);
    FunctionCallNode(std::string const& identifier, SymbolTable::FunctionSymbol const& symbol);

    StorageType evaluatedType() const override;
    NumberType evaluateNumber() const override;
    NodeType nodeType() const override;
    bool areEqual(AbstractSyntaxTreeNode const* other) const override;
};

#endif //LINDENMAYER_ABSTRACTSYNTAXTREE_HPP
