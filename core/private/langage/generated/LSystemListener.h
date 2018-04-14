
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

  virtual void enterIterations(LSystemParser::IterationsContext *ctx) = 0;
  virtual void exitIterations(LSystemParser::IterationsContext *ctx) = 0;

  virtual void enterDistance(LSystemParser::DistanceContext *ctx) = 0;
  virtual void exitDistance(LSystemParser::DistanceContext *ctx) = 0;

  virtual void enterAngle(LSystemParser::AngleContext *ctx) = 0;
  virtual void exitAngle(LSystemParser::AngleContext *ctx) = 0;

  virtual void enterInitial_angle(LSystemParser::Initial_angleContext *ctx) = 0;
  virtual void exitInitial_angle(LSystemParser::Initial_angleContext *ctx) = 0;

  virtual void enterAlias(LSystemParser::AliasContext *ctx) = 0;
  virtual void exitAlias(LSystemParser::AliasContext *ctx) = 0;

  virtual void enterTransformation(LSystemParser::TransformationContext *ctx) = 0;
  virtual void exitTransformation(LSystemParser::TransformationContext *ctx) = 0;


};

