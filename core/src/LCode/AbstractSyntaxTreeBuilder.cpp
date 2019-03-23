//
// Created by Io on 2019-03-05.
//

#include "LCode/AbstractSyntaxTreeBuilder.hpp"
#include "LCode/LCodeScopeTreeBuilder.hpp"
#include "LCode/AbstractSyntaxTreeAlgorithms.hpp"
#include "LCode/AbstractSyntaxTreeNode.hpp"

#include <cassert>

AbstractSyntaxTreeBuilder::AbstractSyntaxTreeBuilder(LCodeScopeTree const& scopeTree, ParseErrors& errors)
: _scopeTree(scopeTree)
, _errors(errors)
{
}

void AbstractSyntaxTreeBuilder::enterProgram(LCodeParser::ProgramContext* context)
{
    _astRoot = std::make_unique<ProgramNode>(context);
    _currentScopeNode = _scopeTree.findNode(context);
    assert( _currentScopeNode != nullptr );
    pushAstNode(_astRoot.get());
}

void AbstractSyntaxTreeBuilder::exitProgram(LCodeParser::ProgramContext*)
{
    popAstNode();
}

void AbstractSyntaxTreeBuilder::enterFloat(LCodeParser::FloatContext* context)
{
    currentAstNode()->makeChild<LiteralNumberNode>(context, StorageTypeTrait<StorageType::Number>::fromText(context->getText()));
}


void AbstractSyntaxTreeBuilder::enterIdentifier(LCodeParser::IdentifierContext* context)
{
    auto const identifier = context->getText();
    assert( currentScopeNode() != nullptr );
    auto const& symbolTable = currentScopeNode()->value();
    if (symbolTable.isConstantDefined(identifier))
    {
        currentAstNode()->makeChild<ConstantNumberNode>(context, identifier, symbolTable);
    }
    else
    {
        pushError(_errors, ParseError::Type::Error, context, "Undefined constant '{}'", identifier);
    }
}

void AbstractSyntaxTreeBuilder::enterConstantDecl(LCodeParser::ConstantDeclContext* context)
{
    pushAstNode(currentAstNode()->makeChild<ConstantDeclarationNode>(context));
}

void AbstractSyntaxTreeBuilder::exitConstantDecl(LCodeParser::ConstantDeclContext* context)
{
    // AST reduction can be done only if parsing is finished without errors.
    // Note: errors gathered here concerns only errors of parents or errors in the sub tree to reduce (the value assigned to
    // the constant).
    if (!containsErrors(_errors, ParseError::Type::Error))
    {
        auto* constantDeclarationNode = dynamic_cast<ConstantDeclarationNode*>(currentAstNode());
        assert(constantDeclarationNode != nullptr);
        auto* expressionNode = dynamic_cast<ExpressionNode*>(constantDeclarationNode->getChild(0));
        // If this assertion fails then something is broken in the parser itself
        assert(expressionNode != nullptr);
        auto const identifier = context->IDENTIFIER()->getText();
        auto const value = reduceAst(expressionNode);
        if (!currentScopeNode()->value().defineConstant(identifier, value))
        {
            pushError(_errors, ParseError::Type::Error, context, "Constant '{}' already defined", identifier);
        }
    }
    popAstNode();
}

void AbstractSyntaxTreeBuilder::enterAliasDecl(LCodeParser::AliasDeclContext* context)
{
    updateCurrentScope(context);
    pushAstNode(currentAstNode()->makeChild<AliasDeclarationNode>(context));
}

void AbstractSyntaxTreeBuilder::exitAliasDecl(LCodeParser::AliasDeclContext* context)
{
    updateCurrentScope(context);
    popAstNode();
}

void AbstractSyntaxTreeBuilder::enterAxiomDecl(LCodeParser::AxiomDeclContext* context)
{
    pushAstNode(currentAstNode()->makeChild<AxiomDeclarationNode>(context));
}

void AbstractSyntaxTreeBuilder::exitAxiomDecl(LCodeParser::AxiomDeclContext*)
{
    popAstNode();
}

void AbstractSyntaxTreeBuilder::enterRewriteRuleDecl(LCodeParser::RewriteRuleDeclContext* context)
{
    updateCurrentScope(context);
    pushAstNode(currentAstNode()->makeChild<RewriteRuleDeclaratinoNode>(context));
}

void AbstractSyntaxTreeBuilder::exitRewriteRuleDecl(LCodeParser::RewriteRuleDeclContext*)
{
    popAstNode();
}



void AbstractSyntaxTreeBuilder::enterAddition(LCodeParser::AdditionContext* context)
{
    pushAstNode(currentAstNode()->makeChild<AdditionNode>(context));
}

void AbstractSyntaxTreeBuilder::exitAddition(LCodeParser::AdditionContext*)
{
    popAstNode();
}

void AbstractSyntaxTreeBuilder::enterSubstraction(LCodeParser::SubstractionContext* context)
{
    pushAstNode(currentAstNode()->makeChild<SubstractionNode>(context));
}

void AbstractSyntaxTreeBuilder::exitSubstraction(LCodeParser::SubstractionContext*)
{
    popAstNode();
}

void AbstractSyntaxTreeBuilder::enterMultiplication(LCodeParser::MultiplicationContext* context)
{
    pushAstNode(currentAstNode()->makeChild<MultiplicationNode>(context));
}

void AbstractSyntaxTreeBuilder::exitMultiplication(LCodeParser::MultiplicationContext*)
{
    popAstNode();
}

void AbstractSyntaxTreeBuilder::enterDivision(LCodeParser::DivisionContext* context)
{
    pushAstNode(currentAstNode()->makeChild<DivisionNode>(context));
}

void AbstractSyntaxTreeBuilder::exitDivision(LCodeParser::DivisionContext*)
{
    popAstNode();
}

void AbstractSyntaxTreeBuilder::enterNegativeExpression(LCodeParser::NegativeExpressionContext* context)
{
    pushAstNode(currentAstNode()->makeChild<NegativeNode>(context));
}

void AbstractSyntaxTreeBuilder::exitNegativeExpression(LCodeParser::NegativeExpressionContext*)
{
    popAstNode();
}


void AbstractSyntaxTreeBuilder::enterFunctionCall(LCodeParser::FunctionCallContext* context)
{
    if (containsErrors(_errors, ParseError::Type::Error))
    {
        return;
    }

    if (auto* scopeNode = currentScopeNode(); scopeNode != nullptr)
    {
        auto const& symbolTable = scopeNode->value();
        auto* const functionCall = context->function_call();
        auto const functionIdentifier = functionCall->IDENTIFIER()->getText();
        if (!symbolTable.isFunctionDefined(functionIdentifier))
        {
            pushError(_errors, ParseError::Type::Error, context, "Undefined function '{}'", functionIdentifier);
            return;
        }
        auto const& functionSymbol = symbolTable.getFunction(functionIdentifier);
        pushAstNode(currentAstNode()->makeChild<FunctionCallNode>(context, functionIdentifier, functionSymbol));
    }
}

void AbstractSyntaxTreeBuilder::exitFunctionCall(LCodeParser::FunctionCallContext*)
{
    if (containsErrors(_errors, ParseError::Type::Error))
    {
        return;
    }
    popAstNode();
}


void AbstractSyntaxTreeBuilder::pushAstNode(AbstractSyntaxTreeNode* astNode)
{
    _stack.push(astNode);
}

void AbstractSyntaxTreeBuilder::popAstNode()
{
    assert( !_stack.empty() );
    _stack.pop();
}

AbstractSyntaxTreeNode* AbstractSyntaxTreeBuilder::currentAstNode() const
{
    assert( !_stack.empty() );
    return _stack.top();
}

LCodeScopeTree::NodeType* AbstractSyntaxTreeBuilder::currentScopeNode() const
{
    return _currentScopeNode;
}

void AbstractSyntaxTreeBuilder::updateCurrentScope(antlr4::tree::ParseTree* parseTreeNode)
{
    _currentScopeNode = _scopeTree.findNode(parseTreeNode);
    // Must have complete scope tree: parseTreeNode must have
    // his own scope tree.
    assert( _currentScopeNode != nullptr );
}

void AbstractSyntaxTreeBuilder::releaseAst(std::unique_ptr<ProgramNode>& ast)
{
    ast.reset(_astRoot.release());
}
