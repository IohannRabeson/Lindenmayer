//
// Created by Io on 2019-03-05.
//

#ifndef LINDENMAYER_ABSTRACTSYNTAXTREEBUILDER_HPP
#define LINDENMAYER_ABSTRACTSYNTAXTREEBUILDER_HPP
#include <generated/LCodeBaseListener.h>
class ProgramNode;
class AbstractSyntaxTreeNode;

class AbstractSyntaxTreeBuilder : public LCodeBaseListener
{
    std::unique_ptr<ProgramNode> _root;
    std::stack<AbstractSyntaxTreeNode*> _stack;
private:
    void pushNode(AbstractSyntaxTreeNode* node);
    void popNode();

    AbstractSyntaxTreeNode* currentNode() const;
public:
    std::unique_ptr<ProgramNode> const& programNode() const
    {
        return _root;
    }

    void enterAddition(LCodeParser::AdditionContext* context) override;
    void enterAliasDecl(LCodeParser::AliasDeclContext* context) override;
    void enterAxiomDecl(LCodeParser::AxiomDeclContext* context) override;
    void enterBoolean(LCodeParser::BooleanContext* context) override;
    void enterConstAddition(LCodeParser::ConstAdditionContext* context) override;
    void enterConstantDecl(LCodeParser::ConstantDeclContext* context) override;
    void enterConstBoolean(LCodeParser::ConstBooleanContext* context) override;
    void enterConstDivision(LCodeParser::ConstDivisionContext* context) override;
    void enterConstEnclosedExpression(LCodeParser::ConstEnclosedExpressionContext* context) override;
    void enterConstFloat(LCodeParser::ConstFloatContext* context) override;
    void enterConstIdentifier(LCodeParser::ConstIdentifierContext* context) override;
    void enterConstInteger(LCodeParser::ConstIntegerContext* context) override;
    void enterConstMultiplication(LCodeParser::ConstMultiplicationContext* context) override;
    void enterConstNegativeExpression(LCodeParser::ConstNegativeExpressionContext* context) override;
    void enterConstString(LCodeParser::ConstStringContext* context) override;
    void enterConstSubstraction(LCodeParser::ConstSubstractionContext* context) override;
    void enterDivision(LCodeParser::DivisionContext* context) override;
    void enterEnclosedExpression(LCodeParser::EnclosedExpressionContext* context) override;
    void enterFloat(LCodeParser::FloatContext* context) override;
    void enterIdentifier(LCodeParser::IdentifierContext* context) override;
    void enterInteger(LCodeParser::IntegerContext* context) override;
    void enterMultiplication(LCodeParser::MultiplicationContext* context) override;
    void enterNegativeExpression(LCodeParser::NegativeExpressionContext* context) override;
    void enterProgram(LCodeParser::ProgramContext* context) override;
    void enterRewriteRuleDecl(LCodeParser::RewriteRuleDeclContext* context) override;
    void enterString(LCodeParser::StringContext* context) override;
    void enterSubstraction(LCodeParser::SubstractionContext* context) override;
    void exitAddition(LCodeParser::AdditionContext* context) override;
    void exitAliasDecl(LCodeParser::AliasDeclContext* context) override;
    void exitAxiomDecl(LCodeParser::AxiomDeclContext* context) override;
    void exitConstAddition(LCodeParser::ConstAdditionContext* context) override;
    void exitConstantDecl(LCodeParser::ConstantDeclContext* context) override;
    void exitConstDivision(LCodeParser::ConstDivisionContext* context) override;
    void exitConstEnclosedExpression(LCodeParser::ConstEnclosedExpressionContext* context) override;
    void exitConstMultiplication(LCodeParser::ConstMultiplicationContext* context) override;
    void exitConstNegativeExpression(LCodeParser::ConstNegativeExpressionContext* context) override;
    void exitConstSubstraction(LCodeParser::ConstSubstractionContext* context) override;
    void exitDivision(LCodeParser::DivisionContext* context) override;
    void exitEnclosedExpression(LCodeParser::EnclosedExpressionContext* context) override;
    void exitMultiplication(LCodeParser::MultiplicationContext* context) override;
    void exitNegativeExpression(LCodeParser::NegativeExpressionContext* context) override;
    void exitRewriteRuleDecl(LCodeParser::RewriteRuleDeclContext* context) override;
    void exitSubstraction(LCodeParser::SubstractionContext* context) override;
};

#endif //LINDENMAYER_ABSTRACTSYNTAXTREEBUILDER_HPP
