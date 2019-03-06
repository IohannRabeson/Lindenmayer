//
// Created by Io on 2019-03-03.
//

#ifndef LINDENMAYER_REDUCECONSTANTEXPRESSIONPASS_HPP
#define LINDENMAYER_REDUCECONSTANTEXPRESSIONPASS_HPP
#include <generated/LCodeBaseListener.h>
#include <stack>

class CompilationContext;
class ExpressionNode;

class GatherConstantExpressionPass : public LCodeBaseListener
{
    CompilationContext& _context;
    std::stack<ExpressionNode*> _stack;
    std::unique_ptr<ExpressionNode> _root;
private:
    template <typename N, typename ... A>
    void addChildOrSetCurrent(LCodeParser::Const_expressionContext* parseTree, A&&...args);

    template <typename N, typename ... A>
    void addChildAndSetCurrent(LCodeParser::Const_expressionContext* parseTree, A&&...args);
public:
    explicit GatherConstantExpressionPass(CompilationContext& context);
    ~GatherConstantExpressionPass();

    void enterConstantDecl(LCodeParser::ConstantDeclContext* context) override;
    void exitConstantDecl(LCodeParser::ConstantDeclContext* context) override;
    void enterConstEnclosedExpression(LCodeParser::ConstEnclosedExpressionContext* context) override;
    void enterConstInteger(LCodeParser::ConstIntegerContext* context) override;
    void enterConstMultiplication(LCodeParser::ConstMultiplicationContext* context) override;
    void enterConstFloat(LCodeParser::ConstFloatContext* context) override;
    void enterConstNegativeExpression(LCodeParser::ConstNegativeExpressionContext* context) override;
    void enterConstAddition(LCodeParser::ConstAdditionContext* context) override;
    void enterConstDivision(LCodeParser::ConstDivisionContext* context) override;
    void enterConstSubstraction(LCodeParser::ConstSubstractionContext* context) override;
    void exitConstEnclosedExpression(LCodeParser::ConstEnclosedExpressionContext* context) override;
    void exitConstMultiplication(LCodeParser::ConstMultiplicationContext* context) override;
    void exitConstNegativeExpression(LCodeParser::ConstNegativeExpressionContext* context) override;
    void exitConstAddition(LCodeParser::ConstAdditionContext* context) override;
    void exitConstDivision(LCodeParser::ConstDivisionContext* context) override;
    void exitConstSubstraction(LCodeParser::ConstSubstractionContext* context) override;
    void exitInteger(LCodeParser::IntegerContext* context) override;
    void exitFloat(LCodeParser::FloatContext* context) override;
};

#endif //LINDENMAYER_REDUCECONSTANTEXPRESSIONPASS_HPP
