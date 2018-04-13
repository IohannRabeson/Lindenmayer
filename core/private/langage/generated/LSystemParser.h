
// Generated from LSystem.g4 by ANTLR 4.7.1

#pragma once


#include "antlr4-runtime.h"




class  LSystemParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, Integer = 2, Float = 3, Boolean = 4, Assign = 5, Identifier = 6, 
    EndOfLine = 7, Newline = 8, Whitespace = 9
  };

  enum {
    RuleProgram = 0, RuleAxiom = 1, RuleTransformation = 2, RuleConstant_def = 3, 
    RuleExpression = 4
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
  class TransformationContext;
  class Constant_defContext;
  class ExpressionContext; 

  class  ProgramContext : public antlr4::ParserRuleContext {
  public:
    ProgramContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AxiomContext *axiom();
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
    std::vector<antlr4::tree::TerminalNode *> Identifier();
    antlr4::tree::TerminalNode* Identifier(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AxiomContext* axiom();

  class  TransformationContext : public antlr4::ParserRuleContext {
  public:
    TransformationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> Identifier();
    antlr4::tree::TerminalNode* Identifier(size_t i);
    antlr4::tree::TerminalNode *Assign();
    antlr4::tree::TerminalNode *EndOfLine();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  TransformationContext* transformation();

  class  Constant_defContext : public antlr4::ParserRuleContext {
  public:
    Constant_defContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Identifier();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *EndOfLine();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Constant_defContext* constant_def();

  class  ExpressionContext : public antlr4::ParserRuleContext {
  public:
    ExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Integer();
    antlr4::tree::TerminalNode *Float();
    antlr4::tree::TerminalNode *Boolean();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ExpressionContext* expression();


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

