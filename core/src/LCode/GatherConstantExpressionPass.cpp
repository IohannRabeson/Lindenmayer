//
// Created by Io on 2019-03-03.
//

#include "LCode/CompilationContext.hpp"
#include "LCode/GatherConstantExpressionPass.hpp"
#include "LCode/StorageType.hpp"
#include "LCode/AbstractSyntaxTreeNode.hpp"

#include <iostream>


template <typename ... A>
struct Print;

template <typename N, typename ... A>
void GatherConstantExpressionPass::addChildOrSetCurrent(LCodeParser::Const_expressionContext* parseTree, A&& ... args)
{
    auto node = std::make_unique<N>(parseTree, std::forward<A>(args)...);
    if (_stack.empty())
    {
        _stack.push(node.get());
        _root = std::move(node);

    }
    else
    {
        _stack.top()->addChild(std::move(node));
    }
}

template <typename N, typename ... A>
void GatherConstantExpressionPass::addChildAndSetCurrent(LCodeParser::Const_expressionContext* parseTree, A&& ... args)
{
    auto node = std::make_unique<N>(parseTree, std::forward<A>(args)...);
    auto nodePtr = node.get();
    if (_stack.empty())
    {
        _root = std::move(node);
    }
    else
    {
        assert( _stack.top() != nullptr );
        _stack.top()->addChild(std::move(node));
    }
    _stack.push(nodePtr);
}

GatherConstantExpressionPass::GatherConstantExpressionPass(CompilationContext& context)
: _context(context)
{
}

GatherConstantExpressionPass::~GatherConstantExpressionPass() = default;

void GatherConstantExpressionPass::enterConstantDecl(LCodeParser::ConstantDeclContext*)
{
    _stack = std::stack<ExpressionNode*>();
    _root = nullptr;
}

#include <iostream>

void GatherConstantExpressionPass::exitConstantDecl(LCodeParser::ConstantDeclContext*)
{
    std::cout << "expresion count: " << _context.constExpressions.size() << std::endl;
}

void GatherConstantExpressionPass::enterConstEnclosedExpression(LCodeParser::ConstEnclosedExpressionContext*)
{
}

void GatherConstantExpressionPass::enterConstInteger(LCodeParser::ConstIntegerContext* context)
{
    auto const newValue = StorageTypeTrait<StorageType::Integer>::fromText(context->getText());
    addChildOrSetCurrent<LitteralNode<StorageType::Integer, AbstractSyntaxTreeNode::NodeType::LiteralInteger>>(context, newValue);
}

void GatherConstantExpressionPass::enterConstFloat(LCodeParser::ConstFloatContext* context)
{
    auto const newValue = StorageTypeTrait<StorageType::Float>::fromText(context->getText());
    addChildOrSetCurrent<LitteralNode<StorageType::Float, AbstractSyntaxTreeNode::NodeType::LiteralFloat>>(context, newValue);
}

void GatherConstantExpressionPass::enterConstNegativeExpression(LCodeParser::ConstNegativeExpressionContext* context)
{
    addChildAndSetCurrent<NegativeNode>(context);
}

void GatherConstantExpressionPass::enterConstMultiplication(LCodeParser::ConstMultiplicationContext* context)
{
    addChildAndSetCurrent<MultiplicationNode>(context);
}

void GatherConstantExpressionPass::enterConstAddition(LCodeParser::ConstAdditionContext* context)
{
    addChildAndSetCurrent<AdditionNode>(context);
}

void GatherConstantExpressionPass::enterConstDivision(LCodeParser::ConstDivisionContext* context)
{
    addChildAndSetCurrent<DivisionNode>(context);
}

void GatherConstantExpressionPass::enterConstSubstraction(LCodeParser::ConstSubstractionContext* context)
{
    addChildAndSetCurrent<SubstractionNode>(context);
}

void GatherConstantExpressionPass::exitConstEnclosedExpression(LCodeParser::ConstEnclosedExpressionContext* context)
{
    assert( _stack.top()->parseTreeNode() == context );
    _stack.pop();
    static_cast<void>(context);
}

void GatherConstantExpressionPass::exitConstNegativeExpression(LCodeParser::ConstNegativeExpressionContext* context)
{
    assert( _stack.top()->parseTreeNode() == context );
    _stack.pop();
    static_cast<void>(context);
}

void GatherConstantExpressionPass::exitConstMultiplication(LCodeParser::ConstMultiplicationContext* context)
{
    assert( _stack.top()->parseTreeNode() == context );
    _stack.pop();
    static_cast<void>(context);
}

void GatherConstantExpressionPass::exitConstAddition(LCodeParser::ConstAdditionContext* context)
{
    assert( _stack.top()->parseTreeNode() == context );
    _stack.pop();
    static_cast<void>(context);
}

void GatherConstantExpressionPass::exitConstDivision(LCodeParser::ConstDivisionContext* context)
{
    assert( _stack.top()->parseTreeNode() == context );
    _stack.pop();
    static_cast<void>(context);
}

void GatherConstantExpressionPass::exitConstSubstraction(LCodeParser::ConstSubstractionContext* context)
{
    assert( _stack.top()->parseTreeNode() == context );
    _stack.pop();
    static_cast<void>(context);
}

void GatherConstantExpressionPass::exitInteger(LCodeParser::IntegerContext* context)
{
    assert( _stack.top()->parseTreeNode() == context );
    _stack.pop();
    static_cast<void>(context);
}

void GatherConstantExpressionPass::exitFloat(LCodeParser::FloatContext* context)
{
    assert( _stack.top()->parseTreeNode() == context );
    _stack.pop();
    static_cast<void>(context);
}

