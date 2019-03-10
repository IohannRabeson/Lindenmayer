//
// Created by Io on 2019-03-05.
//

#include "LCode/AbstractSyntaxTreeBuilder.hpp"
#include "LCode/AbstractSyntaxTreeNode.hpp"

void AbstractSyntaxTreeBuilder::enterProgram(LCodeParser::ProgramContext* context)
{
    _astRoot = std::make_unique<ProgramNode>(context);
    _currentScopeNode = _context._scope.get();
    pushAstNode(_astRoot.get());
}

void AbstractSyntaxTreeBuilder::exitProgram(LCodeParser::ProgramContext*)
{
    popAstNode();
    _context._ast = std::move(_astRoot);
}

void AbstractSyntaxTreeBuilder::enterFloat(LCodeParser::FloatContext* context)
{
    currentAstNode()->makeChild<NumericNode>(context, StorageTypeTrait<StorageType::Number>::fromText(context->getText()));
}

void AbstractSyntaxTreeBuilder::enterConstFloat(LCodeParser::ConstFloatContext* context)
{
    currentAstNode()->makeChild<NumericNode>(context, StorageTypeTrait<StorageType::Number>::fromText(context->getText()));
}



void AbstractSyntaxTreeBuilder::enterIdentifier(LCodeParser::IdentifierContext* context)
{
    currentAstNode()->makeChild<IdentifierNode>(context, context->getText(), StorageType::Null);
}

void AbstractSyntaxTreeBuilder::enterConstIdentifier(LCodeParser::ConstIdentifierContext* context)
{
    currentAstNode()->makeChild<IdentifierNode>(context, context->getText());
}

void AbstractSyntaxTreeBuilder::enterConstantDecl(LCodeParser::ConstantDeclContext* context)
{
    pushAstNode(currentAstNode()->makeChild<ConstantDeclarationNode>(context));
}

void AbstractSyntaxTreeBuilder::exitConstantDecl(LCodeParser::ConstantDeclContext*)
{
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


void AbstractSyntaxTreeBuilder::enterConstAddition(LCodeParser::ConstAdditionContext* context)
{
    pushAstNode(currentAstNode()->makeChild<AdditionNode>(context));
}

void AbstractSyntaxTreeBuilder::exitConstAddition(LCodeParser::ConstAdditionContext*)
{
    popAstNode();
}

void AbstractSyntaxTreeBuilder::enterConstSubstraction(LCodeParser::ConstSubstractionContext* context)
{
    pushAstNode(currentAstNode()->makeChild<SubstractionNode>(context));
}

void AbstractSyntaxTreeBuilder::exitConstSubstraction(LCodeParser::ConstSubstractionContext*)
{
    popAstNode();
}

void AbstractSyntaxTreeBuilder::enterConstMultiplication(LCodeParser::ConstMultiplicationContext* context)
{
    pushAstNode(currentAstNode()->makeChild<MultiplicationNode>(context));
}

void AbstractSyntaxTreeBuilder::exitConstMultiplication(LCodeParser::ConstMultiplicationContext*)
{
    popAstNode();
}

void AbstractSyntaxTreeBuilder::enterConstDivision(LCodeParser::ConstDivisionContext* context)
{
    pushAstNode(currentAstNode()->makeChild<DivisionNode>(context));
}

void AbstractSyntaxTreeBuilder::exitConstDivision(LCodeParser::ConstDivisionContext*)
{
    popAstNode();
}

void AbstractSyntaxTreeBuilder::enterConstNegativeExpression(LCodeParser::ConstNegativeExpressionContext* context)
{
    pushAstNode(currentAstNode()->makeChild<NegativeNode>(context));
}

void AbstractSyntaxTreeBuilder::exitConstNegativeExpression(LCodeParser::ConstNegativeExpressionContext*)
{
    popAstNode();
}

void AbstractSyntaxTreeBuilder::enterFunctionCall(LCodeParser::FunctionCallContext* context)
{
    if (auto* scopeNode = currentScopeNode(); scopeNode != nullptr)
    {
        auto const& symbolTable = scopeNode->value();
        auto* const functionCall = context->function_call();
        auto const functionIdentifier = functionCall->IDENTIFIER()->getText();
        if (!symbolTable.isFunctionDefined(functionIdentifier))
        {
            // TODO: signal error with an exception? or an error stack stored by the context?
            std::cerr << "Undefined function '" << functionIdentifier << "' called\n";
            return;
        }
        auto const& functionSymbol = symbolTable.getFunction(functionIdentifier);
        pushAstNode(currentAstNode()->makeChild<FunctionCallNode>(context, functionIdentifier, functionSymbol));
    }
}

void AbstractSyntaxTreeBuilder::exitFunctionCall(LCodeParser::FunctionCallContext*)
{
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

Context::ScopeNode* AbstractSyntaxTreeBuilder::currentScopeNode() const
{
    auto* const parseTreeNode = currentAstNode()->parseTreeNode();
    auto scopeIt = _context._scopeByParseTree.find(parseTreeNode);
    return scopeIt != _context._scopeByParseTree.end() ? scopeIt->second : nullptr;
}

AbstractSyntaxTreeBuilder::AbstractSyntaxTreeBuilder(Context& context)
: _context(context)
{
}

void AbstractSyntaxTreeBuilder::updateCurrentScope(antlr4::tree::ParseTree* parseTreeNode)
{
    if (auto it = _context._scopeByParseTree.find(parseTreeNode); it != _context._scopeByParseTree.end())
    {
        _currentScopeNode = it->second;
    }
}
