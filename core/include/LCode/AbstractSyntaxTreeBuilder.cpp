//
// Created by Io on 2019-03-05.
//

#include "AbstractSyntaxTreeBuilder.hpp"
#include "AbstractSyntaxTreeNode.hpp"

void AbstractSyntaxTreeBuilder::enterProgram(LCodeParser::ProgramContext* context)
{
    _root = std::make_unique<ProgramNode>(context);
    pushNode(_root.get());
}


void AbstractSyntaxTreeBuilder::enterInteger(LCodeParser::IntegerContext* context)
{
    currentNode()->makeChild<IntegerNode>(context, StorageTypeTrait<StorageType::Integer>::fromText(context->getText()));
}

void AbstractSyntaxTreeBuilder::enterFloat(LCodeParser::FloatContext* context)
{
    currentNode()->makeChild<FloatNode>(context, StorageTypeTrait<StorageType::Float>::fromText(context->getText()));
}

void AbstractSyntaxTreeBuilder::enterString(LCodeParser::StringContext* context)
{
    currentNode()->makeChild<StringNode>(context, StorageTypeTrait<StorageType::String>::fromText(context->getText()));
}

void AbstractSyntaxTreeBuilder::enterBoolean(LCodeParser::BooleanContext* context)
{
    currentNode()->makeChild<BooleanNode>(context, StorageTypeTrait<StorageType::Boolean>::fromText(context->getText()));
}



void AbstractSyntaxTreeBuilder::enterConstInteger(LCodeParser::ConstIntegerContext* context)
{
    currentNode()->makeChild<IntegerNode>(context, StorageTypeTrait<StorageType::Integer>::fromText(context->getText()));
}

void AbstractSyntaxTreeBuilder::enterConstString(LCodeParser::ConstStringContext* context)
{
    currentNode()->makeChild<StringNode>(context, StorageTypeTrait<StorageType::String>::fromText(context->getText()));
}

void AbstractSyntaxTreeBuilder::enterConstBoolean(LCodeParser::ConstBooleanContext* context)
{
    currentNode()->makeChild<BooleanNode>(context, StorageTypeTrait<StorageType::Boolean>::fromText(context->getText()));
}

void AbstractSyntaxTreeBuilder::enterConstFloat(LCodeParser::ConstFloatContext* context)
{
    currentNode()->makeChild<FloatNode>(context, StorageTypeTrait<StorageType::Float>::fromText(context->getText()));
}




void AbstractSyntaxTreeBuilder::enterIdentifier(LCodeParser::IdentifierContext* context)
{
    LCodeBaseListener::enterIdentifier(context);
}

void AbstractSyntaxTreeBuilder::enterConstIdentifier(LCodeParser::ConstIdentifierContext* context)
{
    LCodeBaseListener::enterConstIdentifier(context);
}


void AbstractSyntaxTreeBuilder::enterConstantDecl(LCodeParser::ConstantDeclContext* context)
{
    // TODO: store identifier and type
    pushNode(currentNode()->makeChild<ConstantDeclarationNode>(context));
}

void AbstractSyntaxTreeBuilder::exitConstantDecl(LCodeParser::ConstantDeclContext*)
{
    popNode();
}

void AbstractSyntaxTreeBuilder::enterAliasDecl(LCodeParser::AliasDeclContext* context)
{
    pushNode(currentNode()->makeChild<AliasDeclarationNode>(context));
}

void AbstractSyntaxTreeBuilder::exitAliasDecl(LCodeParser::AliasDeclContext*)
{
    popNode();
}

void AbstractSyntaxTreeBuilder::enterAxiomDecl(LCodeParser::AxiomDeclContext* context)
{
    pushNode(currentNode()->makeChild<AxiomDeclarationNode>(context));
}

void AbstractSyntaxTreeBuilder::exitAxiomDecl(LCodeParser::AxiomDeclContext*)
{
    popNode();
}

void AbstractSyntaxTreeBuilder::enterRewriteRuleDecl(LCodeParser::RewriteRuleDeclContext* context)
{
    pushNode(currentNode()->makeChild<RewriteRuleDeclaratinoNode>(context));
}

void AbstractSyntaxTreeBuilder::exitRewriteRuleDecl(LCodeParser::RewriteRuleDeclContext*)
{
    popNode();
}



void AbstractSyntaxTreeBuilder::enterAddition(LCodeParser::AdditionContext* context)
{
    pushNode(currentNode()->makeChild<AdditionNode>(context));
}

void AbstractSyntaxTreeBuilder::exitAddition(LCodeParser::AdditionContext*)
{
    popNode();
}

void AbstractSyntaxTreeBuilder::enterSubstraction(LCodeParser::SubstractionContext* context)
{
    pushNode(currentNode()->makeChild<SubstractionNode>(context));
}

void AbstractSyntaxTreeBuilder::exitSubstraction(LCodeParser::SubstractionContext*)
{
    popNode();
}

void AbstractSyntaxTreeBuilder::enterMultiplication(LCodeParser::MultiplicationContext* context)
{
    pushNode(currentNode()->makeChild<MultiplicationNode>(context));
}

void AbstractSyntaxTreeBuilder::exitMultiplication(LCodeParser::MultiplicationContext*)
{
    popNode();
}

void AbstractSyntaxTreeBuilder::enterDivision(LCodeParser::DivisionContext* context)
{
    pushNode(currentNode()->makeChild<DivisionNode>(context));
}

void AbstractSyntaxTreeBuilder::exitDivision(LCodeParser::DivisionContext*)
{
    popNode();
}

void AbstractSyntaxTreeBuilder::enterNegativeExpression(LCodeParser::NegativeExpressionContext* context)
{
    pushNode(currentNode()->makeChild<NegativeNode>(context));
}

void AbstractSyntaxTreeBuilder::exitNegativeExpression(LCodeParser::NegativeExpressionContext*)
{
    popNode();
}


void AbstractSyntaxTreeBuilder::enterConstAddition(LCodeParser::ConstAdditionContext* context)
{
    pushNode(currentNode()->makeChild<AdditionNode>(context));
}

void AbstractSyntaxTreeBuilder::exitConstAddition(LCodeParser::ConstAdditionContext*)
{
    popNode();
}

void AbstractSyntaxTreeBuilder::enterConstSubstraction(LCodeParser::ConstSubstractionContext* context)
{
    pushNode(currentNode()->makeChild<SubstractionNode>(context));
}

void AbstractSyntaxTreeBuilder::exitConstSubstraction(LCodeParser::ConstSubstractionContext*)
{
    popNode();
}

void AbstractSyntaxTreeBuilder::enterConstMultiplication(LCodeParser::ConstMultiplicationContext* context)
{
    pushNode(currentNode()->makeChild<MultiplicationNode>(context));
}

void AbstractSyntaxTreeBuilder::exitConstMultiplication(LCodeParser::ConstMultiplicationContext*)
{
    popNode();
}

void AbstractSyntaxTreeBuilder::enterConstDivision(LCodeParser::ConstDivisionContext* context)
{
    pushNode(currentNode()->makeChild<DivisionNode>(context));
}

void AbstractSyntaxTreeBuilder::exitConstDivision(LCodeParser::ConstDivisionContext*)
{
    popNode();
}

void AbstractSyntaxTreeBuilder::enterConstNegativeExpression(LCodeParser::ConstNegativeExpressionContext* context)
{
    pushNode(currentNode()->makeChild<NegativeNode>(context));
}

void AbstractSyntaxTreeBuilder::exitConstNegativeExpression(LCodeParser::ConstNegativeExpressionContext*)
{
    popNode();
}

void AbstractSyntaxTreeBuilder::pushNode(AbstractSyntaxTreeNode* node)
{
    _stack.push(node);
}

void AbstractSyntaxTreeBuilder::popNode()
{
    _stack.pop();
}

AbstractSyntaxTreeNode* AbstractSyntaxTreeBuilder::currentNode() const
{
    return _stack.top();
}

std::unique_ptr<ProgramNode> const& AbstractSyntaxTreeBuilder::programNode() const
{
    return _root;
}
