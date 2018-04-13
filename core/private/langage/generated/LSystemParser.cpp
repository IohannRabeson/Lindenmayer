
// Generated from LSystem.g4 by ANTLR 4.7.1


#include "LSystemListener.h"

#include "LSystemParser.h"


using namespace antlrcpp;
using namespace antlr4;

LSystemParser::LSystemParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

LSystemParser::~LSystemParser() {
  delete _interpreter;
}

std::string LSystemParser::getGrammarFileName() const {
  return "LSystem.g4";
}

const std::vector<std::string>& LSystemParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& LSystemParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- ProgramContext ------------------------------------------------------------------

LSystemParser::ProgramContext::ProgramContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LSystemParser::AxiomContext* LSystemParser::ProgramContext::axiom() {
  return getRuleContext<LSystemParser::AxiomContext>(0);
}

std::vector<LSystemParser::TransformationContext *> LSystemParser::ProgramContext::transformation() {
  return getRuleContexts<LSystemParser::TransformationContext>();
}

LSystemParser::TransformationContext* LSystemParser::ProgramContext::transformation(size_t i) {
  return getRuleContext<LSystemParser::TransformationContext>(i);
}


size_t LSystemParser::ProgramContext::getRuleIndex() const {
  return LSystemParser::RuleProgram;
}

void LSystemParser::ProgramContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterProgram(this);
}

void LSystemParser::ProgramContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitProgram(this);
}

LSystemParser::ProgramContext* LSystemParser::program() {
  ProgramContext *_localctx = _tracker.createInstance<ProgramContext>(_ctx, getState());
  enterRule(_localctx, 0, LSystemParser::RuleProgram);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(10);
    axiom();
    setState(14);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == LSystemParser::Identifier) {
      setState(11);
      transformation();
      setState(16);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AxiomContext ------------------------------------------------------------------

LSystemParser::AxiomContext::AxiomContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> LSystemParser::AxiomContext::Identifier() {
  return getTokens(LSystemParser::Identifier);
}

tree::TerminalNode* LSystemParser::AxiomContext::Identifier(size_t i) {
  return getToken(LSystemParser::Identifier, i);
}


size_t LSystemParser::AxiomContext::getRuleIndex() const {
  return LSystemParser::RuleAxiom;
}

void LSystemParser::AxiomContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAxiom(this);
}

void LSystemParser::AxiomContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAxiom(this);
}

LSystemParser::AxiomContext* LSystemParser::axiom() {
  AxiomContext *_localctx = _tracker.createInstance<AxiomContext>(_ctx, getState());
  enterRule(_localctx, 2, LSystemParser::RuleAxiom);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(18); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(17);
      match(LSystemParser::Identifier);
      setState(20); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == LSystemParser::Identifier);
    setState(22);
    match(LSystemParser::EndOfLine);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TransformationContext ------------------------------------------------------------------

LSystemParser::TransformationContext::TransformationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> LSystemParser::TransformationContext::Identifier() {
  return getTokens(LSystemParser::Identifier);
}

tree::TerminalNode* LSystemParser::TransformationContext::Identifier(size_t i) {
  return getToken(LSystemParser::Identifier, i);
}

tree::TerminalNode* LSystemParser::TransformationContext::Assign() {
  return getToken(LSystemParser::Assign, 0);
}

tree::TerminalNode* LSystemParser::TransformationContext::EndOfLine() {
  return getToken(LSystemParser::EndOfLine, 0);
}


size_t LSystemParser::TransformationContext::getRuleIndex() const {
  return LSystemParser::RuleTransformation;
}

void LSystemParser::TransformationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTransformation(this);
}

void LSystemParser::TransformationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTransformation(this);
}

LSystemParser::TransformationContext* LSystemParser::transformation() {
  TransformationContext *_localctx = _tracker.createInstance<TransformationContext>(_ctx, getState());
  enterRule(_localctx, 4, LSystemParser::RuleTransformation);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(24);
    match(LSystemParser::Identifier);
    setState(25);
    match(LSystemParser::Assign);
    setState(27); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(26);
      match(LSystemParser::Identifier);
      setState(29); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == LSystemParser::Identifier);
    setState(31);
    match(LSystemParser::EndOfLine);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Constant_defContext ------------------------------------------------------------------

LSystemParser::Constant_defContext::Constant_defContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LSystemParser::Constant_defContext::Identifier() {
  return getToken(LSystemParser::Identifier, 0);
}

LSystemParser::ExpressionContext* LSystemParser::Constant_defContext::expression() {
  return getRuleContext<LSystemParser::ExpressionContext>(0);
}

tree::TerminalNode* LSystemParser::Constant_defContext::EndOfLine() {
  return getToken(LSystemParser::EndOfLine, 0);
}


size_t LSystemParser::Constant_defContext::getRuleIndex() const {
  return LSystemParser::RuleConstant_def;
}

void LSystemParser::Constant_defContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConstant_def(this);
}

void LSystemParser::Constant_defContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConstant_def(this);
}

LSystemParser::Constant_defContext* LSystemParser::constant_def() {
  Constant_defContext *_localctx = _tracker.createInstance<Constant_defContext>(_ctx, getState());
  enterRule(_localctx, 6, LSystemParser::RuleConstant_def);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(33);
    match(LSystemParser::T__0);
    setState(34);
    match(LSystemParser::Identifier);
    setState(35);
    match(LSystemParser::Assign);
    setState(36);
    expression();
    setState(37);
    match(LSystemParser::EndOfLine);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionContext ------------------------------------------------------------------

LSystemParser::ExpressionContext::ExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LSystemParser::ExpressionContext::Integer() {
  return getToken(LSystemParser::Integer, 0);
}

tree::TerminalNode* LSystemParser::ExpressionContext::Float() {
  return getToken(LSystemParser::Float, 0);
}

tree::TerminalNode* LSystemParser::ExpressionContext::Boolean() {
  return getToken(LSystemParser::Boolean, 0);
}


size_t LSystemParser::ExpressionContext::getRuleIndex() const {
  return LSystemParser::RuleExpression;
}

void LSystemParser::ExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExpression(this);
}

void LSystemParser::ExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExpression(this);
}

LSystemParser::ExpressionContext* LSystemParser::expression() {
  ExpressionContext *_localctx = _tracker.createInstance<ExpressionContext>(_ctx, getState());
  enterRule(_localctx, 8, LSystemParser::RuleExpression);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(39);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << LSystemParser::Integer)
      | (1ULL << LSystemParser::Float)
      | (1ULL << LSystemParser::Boolean))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

// Static vars and initialization.
std::vector<dfa::DFA> LSystemParser::_decisionToDFA;
atn::PredictionContextCache LSystemParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN LSystemParser::_atn;
std::vector<uint16_t> LSystemParser::_serializedATN;

std::vector<std::string> LSystemParser::_ruleNames = {
  "program", "axiom", "transformation", "constant_def", "expression"
};

std::vector<std::string> LSystemParser::_literalNames = {
  "", "'const'", "", "", "", "'='", "", "';'"
};

std::vector<std::string> LSystemParser::_symbolicNames = {
  "", "", "Integer", "Float", "Boolean", "Assign", "Identifier", "EndOfLine", 
  "Newline", "Whitespace"
};

dfa::Vocabulary LSystemParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> LSystemParser::_tokenNames;

LSystemParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  _serializedATN = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
    0x3, 0xb, 0x2c, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 0x9, 
    0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x3, 0x2, 0x3, 0x2, 0x7, 
    0x2, 0xf, 0xa, 0x2, 0xc, 0x2, 0xe, 0x2, 0x12, 0xb, 0x2, 0x3, 0x3, 0x6, 
    0x3, 0x15, 0xa, 0x3, 0xd, 0x3, 0xe, 0x3, 0x16, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x4, 0x3, 0x4, 0x3, 0x4, 0x6, 0x4, 0x1e, 0xa, 0x4, 0xd, 0x4, 0xe, 0x4, 
    0x1f, 0x3, 0x4, 0x3, 0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 
    0x5, 0x3, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x2, 0x2, 0x7, 0x2, 0x4, 
    0x6, 0x8, 0xa, 0x2, 0x3, 0x3, 0x2, 0x4, 0x6, 0x2, 0x29, 0x2, 0xc, 0x3, 
    0x2, 0x2, 0x2, 0x4, 0x14, 0x3, 0x2, 0x2, 0x2, 0x6, 0x1a, 0x3, 0x2, 0x2, 
    0x2, 0x8, 0x23, 0x3, 0x2, 0x2, 0x2, 0xa, 0x29, 0x3, 0x2, 0x2, 0x2, 0xc, 
    0x10, 0x5, 0x4, 0x3, 0x2, 0xd, 0xf, 0x5, 0x6, 0x4, 0x2, 0xe, 0xd, 0x3, 
    0x2, 0x2, 0x2, 0xf, 0x12, 0x3, 0x2, 0x2, 0x2, 0x10, 0xe, 0x3, 0x2, 0x2, 
    0x2, 0x10, 0x11, 0x3, 0x2, 0x2, 0x2, 0x11, 0x3, 0x3, 0x2, 0x2, 0x2, 
    0x12, 0x10, 0x3, 0x2, 0x2, 0x2, 0x13, 0x15, 0x7, 0x8, 0x2, 0x2, 0x14, 
    0x13, 0x3, 0x2, 0x2, 0x2, 0x15, 0x16, 0x3, 0x2, 0x2, 0x2, 0x16, 0x14, 
    0x3, 0x2, 0x2, 0x2, 0x16, 0x17, 0x3, 0x2, 0x2, 0x2, 0x17, 0x18, 0x3, 
    0x2, 0x2, 0x2, 0x18, 0x19, 0x7, 0x9, 0x2, 0x2, 0x19, 0x5, 0x3, 0x2, 
    0x2, 0x2, 0x1a, 0x1b, 0x7, 0x8, 0x2, 0x2, 0x1b, 0x1d, 0x7, 0x7, 0x2, 
    0x2, 0x1c, 0x1e, 0x7, 0x8, 0x2, 0x2, 0x1d, 0x1c, 0x3, 0x2, 0x2, 0x2, 
    0x1e, 0x1f, 0x3, 0x2, 0x2, 0x2, 0x1f, 0x1d, 0x3, 0x2, 0x2, 0x2, 0x1f, 
    0x20, 0x3, 0x2, 0x2, 0x2, 0x20, 0x21, 0x3, 0x2, 0x2, 0x2, 0x21, 0x22, 
    0x7, 0x9, 0x2, 0x2, 0x22, 0x7, 0x3, 0x2, 0x2, 0x2, 0x23, 0x24, 0x7, 
    0x3, 0x2, 0x2, 0x24, 0x25, 0x7, 0x8, 0x2, 0x2, 0x25, 0x26, 0x7, 0x7, 
    0x2, 0x2, 0x26, 0x27, 0x5, 0xa, 0x6, 0x2, 0x27, 0x28, 0x7, 0x9, 0x2, 
    0x2, 0x28, 0x9, 0x3, 0x2, 0x2, 0x2, 0x29, 0x2a, 0x9, 0x2, 0x2, 0x2, 
    0x2a, 0xb, 0x3, 0x2, 0x2, 0x2, 0x5, 0x10, 0x16, 0x1f, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

LSystemParser::Initializer LSystemParser::_init;
