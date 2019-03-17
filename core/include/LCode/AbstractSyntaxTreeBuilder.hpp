//
// Created by Io on 2019-03-05.
//

#ifndef LINDENMAYER_ABSTRACTSYNTAXTREEBUILDER_HPP
#define LINDENMAYER_ABSTRACTSYNTAXTREEBUILDER_HPP
#include <generated/LCodeBaseListener.h>
#include "LCode/ParseError.hpp"
#include "LCode/LCodeScopeTree.hpp"

class ProgramNode;
class AbstractSyntaxTreeNode;

/*!
 * \brief This pass build the complete abstract syntax tree
 *
 * If everything goes well, context passed as parameter store an AST
 * after a complete visit of a parse tree.
 */
class AbstractSyntaxTreeBuilder : public LCodeBaseListener
{
    std::unique_ptr<ProgramNode> _astRoot;
    ScopeTree<SymbolTable> const& _scopeTree;
    std::stack<AbstractSyntaxTreeNode*> _stack;
    LCodeScopeTree::NodeType* _currentScopeNode = nullptr;
    ParseErrors& _errors;
private:
    void pushAstNode(AbstractSyntaxTreeNode* astNode);
    void popAstNode();
    void updateCurrentScope(antlr4::tree::ParseTree* parseTreeNode);

    AbstractSyntaxTreeNode* currentAstNode() const;
    LCodeScopeTree::NodeType* currentScopeNode() const;
public:
    explicit AbstractSyntaxTreeBuilder(ScopeTree<SymbolTable> const& scopeTree, ParseErrors& errors);

    void releaseAst(std::unique_ptr<ProgramNode>& ast);

    void enterProgram(LCodeParser::ProgramContext* context) override;
    void exitProgram(LCodeParser::ProgramContext* context) override;

    void enterAliasDecl(LCodeParser::AliasDeclContext* context) override;
    void exitAliasDecl(LCodeParser::AliasDeclContext* context) override;

    void enterAxiomDecl(LCodeParser::AxiomDeclContext* context) override;
    void exitAxiomDecl(LCodeParser::AxiomDeclContext* context) override;

    void enterConstantDecl(LCodeParser::ConstantDeclContext* context) override;
    void exitConstantDecl(LCodeParser::ConstantDeclContext* context) override;

    void enterRewriteRuleDecl(LCodeParser::RewriteRuleDeclContext* context) override;
    void exitRewriteRuleDecl(LCodeParser::RewriteRuleDeclContext* context) override;

    void enterFloat(LCodeParser::FloatContext* context) override;
    void enterIdentifier(LCodeParser::IdentifierContext* context) override;

    void enterAddition(LCodeParser::AdditionContext* context) override;
    void enterSubstraction(LCodeParser::SubstractionContext* context) override;
    void enterMultiplication(LCodeParser::MultiplicationContext* context) override;
    void enterDivision(LCodeParser::DivisionContext* context) override;
    void enterNegativeExpression(LCodeParser::NegativeExpressionContext* context) override;

    void exitAddition(LCodeParser::AdditionContext* context) override;
    void exitSubstraction(LCodeParser::SubstractionContext* context) override;
    void exitMultiplication(LCodeParser::MultiplicationContext* context) override;
    void exitDivision(LCodeParser::DivisionContext* context) override;
    void exitNegativeExpression(LCodeParser::NegativeExpressionContext* context) override;

    void enterFunctionCall(LCodeParser::FunctionCallContext* context) override;
    void exitFunctionCall(LCodeParser::FunctionCallContext* context) override;
};

#endif //LINDENMAYER_ABSTRACTSYNTAXTREEBUILDER_HPP
