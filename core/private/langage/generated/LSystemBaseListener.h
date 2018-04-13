
// Generated from LSystem.g4 by ANTLR 4.7.1

#pragma once


#include "antlr4-runtime.h"
#include "LSystemListener.h"


/**
 * This class provides an empty implementation of LSystemListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  LSystemBaseListener : public LSystemListener {
public:

  virtual void enterProgram(LSystemParser::ProgramContext * /*ctx*/) override { }
  virtual void exitProgram(LSystemParser::ProgramContext * /*ctx*/) override { }

  virtual void enterAxiom(LSystemParser::AxiomContext * /*ctx*/) override { }
  virtual void exitAxiom(LSystemParser::AxiomContext * /*ctx*/) override { }

  virtual void enterTransformation(LSystemParser::TransformationContext * /*ctx*/) override { }
  virtual void exitTransformation(LSystemParser::TransformationContext * /*ctx*/) override { }

  virtual void enterConstant_def(LSystemParser::Constant_defContext * /*ctx*/) override { }
  virtual void exitConstant_def(LSystemParser::Constant_defContext * /*ctx*/) override { }

  virtual void enterExpression(LSystemParser::ExpressionContext * /*ctx*/) override { }
  virtual void exitExpression(LSystemParser::ExpressionContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

