
// Generated from LSystem.g4 by ANTLR 4.7.1

#pragma once


#include "antlr4-runtime.h"




class  LSystemParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, Integer = 7, 
    Float = 8, Boolean = 9, TransformOperator = 10, Identifier = 11, EndOfLine = 12, 
    Newline = 13, Whitespace = 14
  };

  enum {
    RuleProgram = 0, RuleAxiom = 1, RuleIterations = 2, RuleDistance = 3, 
    RuleAngle = 4, RuleInitial_angle = 5, RuleAlias = 6, RuleTransformation = 7
  };

  LSystemParser(antlr4::TokenStream *input);
  ~LSystemParser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  class ProgramContext;
  class AxiomContext;
  class IterationsContext;
  class DistanceContext;
  class AngleContext;
  class Initial_angleContext;
  class AliasContext;
  class TransformationContext; 

  class  ProgramContext : public antlr4::ParserRuleContext {
  public:
    ProgramContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AxiomContext *axiom();
    std::vector<IterationsContext *> iterations();
    IterationsContext* iterations(size_t i);
    std::vector<DistanceContext *> distance();
    DistanceContext* distance(size_t i);
    std::vector<AngleContext *> angle();
    AngleContext* angle(size_t i);
    std::vector<Initial_angleContext *> initial_angle();
    Initial_angleContext* initial_angle(size_t i);
    std::vector<AliasContext *> alias();
    AliasContext* alias(size_t i);
    std::vector<TransformationContext *> transformation();
    TransformationContext* transformation(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ProgramContext* program();

  class  AxiomContext : public antlr4::ParserRuleContext {
  public:
    AxiomContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EndOfLine();
    std::vector<antlr4::tree::TerminalNode *> Identifier();
    antlr4::tree::TerminalNode* Identifier(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AxiomContext* axiom();

  class  IterationsContext : public antlr4::ParserRuleContext {
  public:
    IterationsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Integer();
    antlr4::tree::TerminalNode *EndOfLine();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  IterationsContext* iterations();

  class  DistanceContext : public antlr4::ParserRuleContext {
  public:
    DistanceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Float();
    antlr4::tree::TerminalNode *EndOfLine();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  DistanceContext* distance();

  class  AngleContext : public antlr4::ParserRuleContext {
  public:
    AngleContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Float();
    antlr4::tree::TerminalNode *EndOfLine();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AngleContext* angle();

  class  Initial_angleContext : public antlr4::ParserRuleContext {
  public:
    Initial_angleContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Float();
    antlr4::tree::TerminalNode *EndOfLine();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Initial_angleContext* initial_angle();

  class  AliasContext : public antlr4::ParserRuleContext {
  public:
    AliasContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> Identifier();
    antlr4::tree::TerminalNode* Identifier(size_t i);
    antlr4::tree::TerminalNode *EndOfLine();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AliasContext* alias();

  class  TransformationContext : public antlr4::ParserRuleContext {
  public:
    TransformationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> Identifier();
    antlr4::tree::TerminalNode* Identifier(size_t i);
    antlr4::tree::TerminalNode *TransformOperator();
    antlr4::tree::TerminalNode *EndOfLine();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  TransformationContext* transformation();


private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

