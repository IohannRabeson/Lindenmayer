
// Generated from LSystem.g4 by ANTLR 4.7.1

#pragma once


#include "antlr4-runtime.h"
#include "LSystemParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by LSystemParser.
 */
class  LSystemListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterProgram(LSystemParser::ProgramContext *ctx) = 0;
  virtual void exitProgram(LSystemParser::ProgramContext *ctx) = 0;

  virtual void enterAxiom(LSystemParser::AxiomContext *ctx) = 0;
  virtual void exitAxiom(LSystemParser::AxiomContext *ctx) = 0;

  virtual void enterTransformation(LSystemParser::TransformationContext *ctx) = 0;
  virtual void exitTransformation(LSystemParser::TransformationContext *ctx) = 0;

  virtual void enterConstant_def(LSystemParser::Constant_defContext *ctx) = 0;
  virtual void exitConstant_def(LSystemParser::Constant_defContext *ctx) = 0;

  virtual void enterExpression(LSystemParser::ExpressionContext *ctx) = 0;
  virtual void exitExpression(LSystemParser::ExpressionContext *ctx) = 0;


};

