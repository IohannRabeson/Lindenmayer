//
// Created by Io on 2019-03-02.
//

#ifndef LINDENMAYER_ABSTRACTSYNTAXTREE_HPP
#define LINDENMAYER_ABSTRACTSYNTAXTREE_HPP
#include <antlr4-runtime.h>
#include "StorageType.hpp"

class AbstractSyntaxTreeNode
{
    using AbstractSyntaxTreePtr = std::unique_ptr<AbstractSyntaxTreeNode>;
public:
    enum class NodeType
    {
        Abstract,
        LiteralBoolean,
        LiteralInteger,
        LiteralFloat,
        LiteralString,
        Identifier,
        Assignation,
        Addition,
        Substraction,
        Multiplication,
        Division,
        Negative,
        Program,
        ConstantDeclaration,
        AliasDeclaration,
        AxiomDeclaration,
        RewriteRuleDeclaration
    };

    static std::string const& nodeTypeName(NodeType nodeType);
    static constexpr NodeType const Type = NodeType::Abstract;

    explicit AbstractSyntaxTreeNode(antlr4::tree::ParseTree* parseTreeNode = nullptr);
    virtual ~AbstractSyntaxTreeNode() = default;
    virtual NodeType nodeType() const = 0;
    virtual bool areEqual(AbstractSyntaxTreeNode const* other) const
    {
        return nodeType() == other->nodeType();
    }

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
    auto begin() const
    {
        return _children.begin();
    }
    auto end() const
    {
        return _children.begin();
    }
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

class ExpressionNode : public AbstractSyntaxTreeNode
{
protected:
    ExpressionNode const* getExpressionChild(std::size_t index) const;
    StorageType getEvaluatedTypeChild(std::size_t index) const;
public:
    using AbstractSyntaxTreeNode::AbstractSyntaxTreeNode;
    virtual StorageType evaluatedType() const = 0;
};

using ExpressionNodePtr = std::unique_ptr<ExpressionNode>;

template <StorageType StorageTypeId, AbstractSyntaxTreeNode::NodeType NodeTypeId>
class LitteralNode : public ExpressionNode
{
    using ThisType = LitteralNode<StorageTypeId, NodeTypeId>;
public:
    using ValueType = CppType<StorageTypeId>;

    explicit LitteralNode(ValueType value)
    : ExpressionNode(nullptr)
    , _value(value)
    {
    }

    LitteralNode(antlr4::tree::ParseTree* parserTreeNode, ValueType value)
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

using IntegerNode = LitteralNode<StorageType::Integer, AbstractSyntaxTreeNode::NodeType::LiteralInteger>;
using FloatNode = LitteralNode<StorageType::Float, AbstractSyntaxTreeNode::NodeType::LiteralFloat>;
using StringNode = LitteralNode<StorageType::String, AbstractSyntaxTreeNode::NodeType::LiteralString>;
using BooleanNode = LitteralNode<StorageType::Boolean, AbstractSyntaxTreeNode::NodeType::LiteralBoolean>;

class IdentifierNode : public ExpressionNode
{
    // TODO: storage an iterator on an element in the symbol table
    std::string const _identifier;
public:
    explicit IdentifierNode(std::string const& identifier);
    IdentifierNode(antlr4::tree::ParseTree* parserTreeNode, std::string const& identifier);
    StorageType evaluatedType() const override
    {
        // TODO: I need to have a symbol table which stores informations about
        // every constants. This method should query the symbol table to be able to
        // return the type.
        return StorageType::Null;
    }

    NodeType nodeType() const override;
};

class BinaryOperatorNode : public ExpressionNode
{
public:
    explicit BinaryOperatorNode(antlr4::tree::ParseTree* parseTreeNode = nullptr);

    StorageType evaluatedType() const override;
};

class AdditionNode : public BinaryOperatorNode
{
public:
    using BinaryOperatorNode::BinaryOperatorNode;
    NodeType nodeType() const override;
};

class SubstractionNode : public BinaryOperatorNode
{
public:
    using BinaryOperatorNode::BinaryOperatorNode;
    NodeType nodeType() const override;
};

class MultiplicationNode : public BinaryOperatorNode
{
public:
    using BinaryOperatorNode::BinaryOperatorNode;
    NodeType nodeType() const override;
};

class DivisionNode : public BinaryOperatorNode
{
public:
    using BinaryOperatorNode::BinaryOperatorNode;
    NodeType nodeType() const override;
};

class AssignationNode : public BinaryOperatorNode
{
    using BinaryOperatorNode::BinaryOperatorNode;
    NodeType nodeType() const override;
};

class NegativeNode : public ExpressionNode
{
public:
    using ExpressionNode::ExpressionNode;
    StorageType evaluatedType() const override;
    NodeType nodeType() const override;
    bool areEqual(AbstractSyntaxTreeNode const* other) const override;
};

#endif //LINDENMAYER_ABSTRACTSYNTAXTREE_HPP
