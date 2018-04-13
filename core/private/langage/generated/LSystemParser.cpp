
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

std::vector<LSystemParser::IterationsContext *> LSystemParser::ProgramContext::iterations() {
  return getRuleContexts<LSystemParser::IterationsContext>();
}

LSystemParser::IterationsContext* LSystemParser::ProgramContext::iterations(size_t i) {
  return getRuleContext<LSystemParser::IterationsContext>(i);
}

std::vector<LSystemParser::DistanceContext *> LSystemParser::ProgramContext::distance() {
  return getRuleContexts<LSystemParser::DistanceContext>();
}

LSystemParser::DistanceContext* LSystemParser::ProgramContext::distance(size_t i) {
  return getRuleContext<LSystemParser::DistanceContext>(i);
}

std::vector<LSystemParser::AngleContext *> LSystemParser::ProgramContext::angle() {
  return getRuleContexts<LSystemParser::AngleContext>();
}

LSystemParser::AngleContext* LSystemParser::ProgramContext::angle(size_t i) {
  return getRuleContext<LSystemParser::AngleContext>(i);
}

std::vector<LSystemParser::Initial_angleContext *> LSystemParser::ProgramContext::initial_angle() {
  return getRuleContexts<LSystemParser::Initial_angleContext>();
}

LSystemParser::Initial_angleContext* LSystemParser::ProgramContext::initial_angle(size_t i) {
  return getRuleContext<LSystemParser::Initial_angleContext>(i);
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
    setState(14);
    axiom();
    setState(21);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << LSystemParser::T__1)
      | (1ULL << LSystemParser::T__2)
      | (1ULL << LSystemParser::T__3)
      | (1ULL << LSystemParser::T__4))) != 0)) {
      setState(19);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case LSystemParser::T__1: {
          setState(15);
          iterations();
          break;
        }

        case LSystemParser::T__2: {
          setState(16);
          distance();
          break;
        }

        case LSystemParser::T__3: {
          setState(17);
          angle();
          break;
        }

        case LSystemParser::T__4: {
          setState(18);
          initial_angle();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(23);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(27);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == LSystemParser::Identifier) {
      setState(24);
      transformation();
      setState(29);
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

tree::TerminalNode* LSystemParser::AxiomContext::EndOfLine() {
  return getToken(LSystemParser::EndOfLine, 0);
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
    setState(30);
    match(LSystemParser::T__0);
    setState(32); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(31);
      match(LSystemParser::Identifier);
      setState(34); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == LSystemParser::Identifier);
    setState(36);
    match(LSystemParser::EndOfLine);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IterationsContext ------------------------------------------------------------------

LSystemParser::IterationsContext::IterationsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LSystemParser::IterationsContext::Integer() {
  return getToken(LSystemParser::Integer, 0);
}

tree::TerminalNode* LSystemParser::IterationsContext::EndOfLine() {
  return getToken(LSystemParser::EndOfLine, 0);
}


size_t LSystemParser::IterationsContext::getRuleIndex() const {
  return LSystemParser::RuleIterations;
}

void LSystemParser::IterationsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIterations(this);
}

void LSystemParser::IterationsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIterations(this);
}

LSystemParser::IterationsContext* LSystemParser::iterations() {
  IterationsContext *_localctx = _tracker.createInstance<IterationsContext>(_ctx, getState());
  enterRule(_localctx, 4, LSystemParser::RuleIterations);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(38);
    match(LSystemParser::T__1);
    setState(39);
    match(LSystemParser::Integer);
    setState(40);
    match(LSystemParser::EndOfLine);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DistanceContext ------------------------------------------------------------------

LSystemParser::DistanceContext::DistanceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LSystemParser::DistanceContext::Float() {
  return getToken(LSystemParser::Float, 0);
}

tree::TerminalNode* LSystemParser::DistanceContext::EndOfLine() {
  return getToken(LSystemParser::EndOfLine, 0);
}


size_t LSystemParser::DistanceContext::getRuleIndex() const {
  return LSystemParser::RuleDistance;
}

void LSystemParser::DistanceContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDistance(this);
}

void LSystemParser::DistanceContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDistance(this);
}

LSystemParser::DistanceContext* LSystemParser::distance() {
  DistanceContext *_localctx = _tracker.createInstance<DistanceContext>(_ctx, getState());
  enterRule(_localctx, 6, LSystemParser::RuleDistance);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(42);
    match(LSystemParser::T__2);
    setState(43);
    match(LSystemParser::Float);
    setState(44);
    match(LSystemParser::EndOfLine);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AngleContext ------------------------------------------------------------------

LSystemParser::AngleContext::AngleContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LSystemParser::AngleContext::Float() {
  return getToken(LSystemParser::Float, 0);
}

tree::TerminalNode* LSystemParser::AngleContext::EndOfLine() {
  return getToken(LSystemParser::EndOfLine, 0);
}


size_t LSystemParser::AngleContext::getRuleIndex() const {
  return LSystemParser::RuleAngle;
}

void LSystemParser::AngleContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAngle(this);
}

void LSystemParser::AngleContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAngle(this);
}

LSystemParser::AngleContext* LSystemParser::angle() {
  AngleContext *_localctx = _tracker.createInstance<AngleContext>(_ctx, getState());
  enterRule(_localctx, 8, LSystemParser::RuleAngle);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(46);
    match(LSystemParser::T__3);
    setState(47);
    match(LSystemParser::Float);
    setState(48);
    match(LSystemParser::EndOfLine);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Initial_angleContext ------------------------------------------------------------------

LSystemParser::Initial_angleContext::Initial_angleContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LSystemParser::Initial_angleContext::Float() {
  return getToken(LSystemParser::Float, 0);
}

tree::TerminalNode* LSystemParser::Initial_angleContext::EndOfLine() {
  return getToken(LSystemParser::EndOfLine, 0);
}


size_t LSystemParser::Initial_angleContext::getRuleIndex() const {
  return LSystemParser::RuleInitial_angle;
}

void LSystemParser::Initial_angleContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterInitial_angle(this);
}

void LSystemParser::Initial_angleContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitInitial_angle(this);
}

LSystemParser::Initial_angleContext* LSystemParser::initial_angle() {
  Initial_angleContext *_localctx = _tracker.createInstance<Initial_angleContext>(_ctx, getState());
  enterRule(_localctx, 10, LSystemParser::RuleInitial_angle);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(50);
    match(LSystemParser::T__4);
    setState(51);
    match(LSystemParser::Float);
    setState(52);
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

tree::TerminalNode* LSystemParser::TransformationContext::TransformOperator() {
  return getToken(LSystemParser::TransformOperator, 0);
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
  enterRule(_localctx, 12, LSystemParser::RuleTransformation);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(54);
    match(LSystemParser::Identifier);
    setState(55);
    match(LSystemParser::TransformOperator);
    setState(57); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(56);
      match(LSystemParser::Identifier);
      setState(59); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == LSystemParser::Identifier);
    setState(61);
    match(LSystemParser::EndOfLine);
   
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
  "program", "axiom", "iterations", "distance", "angle", "initial_angle", 
  "transformation"
};

std::vector<std::string> LSystemParser::_literalNames = {
  "", "'axiom:'", "'iterations:'", "'distance:'", "'angle:'", "'initialAngle:'", 
  "", "", "", "'='", "", "';'"
};

std::vector<std::string> LSystemParser::_symbolicNames = {
  "", "", "", "", "", "", "Integer", "Float", "Boolean", "TransformOperator", 
  "Identifier", "EndOfLine", "Newline", "Whitespace"
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
    0x3, 0xf, 0x42, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 0x9, 
    0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 0x7, 0x4, 
    0x8, 0x9, 0x8, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x7, 
    0x2, 0x16, 0xa, 0x2, 0xc, 0x2, 0xe, 0x2, 0x19, 0xb, 0x2, 0x3, 0x2, 0x7, 
    0x2, 0x1c, 0xa, 0x2, 0xc, 0x2, 0xe, 0x2, 0x1f, 0xb, 0x2, 0x3, 0x3, 0x3, 
    0x3, 0x6, 0x3, 0x23, 0xa, 0x3, 0xd, 0x3, 0xe, 0x3, 0x24, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 
    0x5, 0x3, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x7, 0x3, 
    0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x6, 0x8, 0x3c, 
    0xa, 0x8, 0xd, 0x8, 0xe, 0x8, 0x3d, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x2, 
    0x2, 0x9, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x2, 0x2, 0x2, 0x41, 0x2, 
    0x10, 0x3, 0x2, 0x2, 0x2, 0x4, 0x20, 0x3, 0x2, 0x2, 0x2, 0x6, 0x28, 
    0x3, 0x2, 0x2, 0x2, 0x8, 0x2c, 0x3, 0x2, 0x2, 0x2, 0xa, 0x30, 0x3, 0x2, 
    0x2, 0x2, 0xc, 0x34, 0x3, 0x2, 0x2, 0x2, 0xe, 0x38, 0x3, 0x2, 0x2, 0x2, 
    0x10, 0x17, 0x5, 0x4, 0x3, 0x2, 0x11, 0x16, 0x5, 0x6, 0x4, 0x2, 0x12, 
    0x16, 0x5, 0x8, 0x5, 0x2, 0x13, 0x16, 0x5, 0xa, 0x6, 0x2, 0x14, 0x16, 
    0x5, 0xc, 0x7, 0x2, 0x15, 0x11, 0x3, 0x2, 0x2, 0x2, 0x15, 0x12, 0x3, 
    0x2, 0x2, 0x2, 0x15, 0x13, 0x3, 0x2, 0x2, 0x2, 0x15, 0x14, 0x3, 0x2, 
    0x2, 0x2, 0x16, 0x19, 0x3, 0x2, 0x2, 0x2, 0x17, 0x15, 0x3, 0x2, 0x2, 
    0x2, 0x17, 0x18, 0x3, 0x2, 0x2, 0x2, 0x18, 0x1d, 0x3, 0x2, 0x2, 0x2, 
    0x19, 0x17, 0x3, 0x2, 0x2, 0x2, 0x1a, 0x1c, 0x5, 0xe, 0x8, 0x2, 0x1b, 
    0x1a, 0x3, 0x2, 0x2, 0x2, 0x1c, 0x1f, 0x3, 0x2, 0x2, 0x2, 0x1d, 0x1b, 
    0x3, 0x2, 0x2, 0x2, 0x1d, 0x1e, 0x3, 0x2, 0x2, 0x2, 0x1e, 0x3, 0x3, 
    0x2, 0x2, 0x2, 0x1f, 0x1d, 0x3, 0x2, 0x2, 0x2, 0x20, 0x22, 0x7, 0x3, 
    0x2, 0x2, 0x21, 0x23, 0x7, 0xc, 0x2, 0x2, 0x22, 0x21, 0x3, 0x2, 0x2, 
    0x2, 0x23, 0x24, 0x3, 0x2, 0x2, 0x2, 0x24, 0x22, 0x3, 0x2, 0x2, 0x2, 
    0x24, 0x25, 0x3, 0x2, 0x2, 0x2, 0x25, 0x26, 0x3, 0x2, 0x2, 0x2, 0x26, 
    0x27, 0x7, 0xd, 0x2, 0x2, 0x27, 0x5, 0x3, 0x2, 0x2, 0x2, 0x28, 0x29, 
    0x7, 0x4, 0x2, 0x2, 0x29, 0x2a, 0x7, 0x8, 0x2, 0x2, 0x2a, 0x2b, 0x7, 
    0xd, 0x2, 0x2, 0x2b, 0x7, 0x3, 0x2, 0x2, 0x2, 0x2c, 0x2d, 0x7, 0x5, 
    0x2, 0x2, 0x2d, 0x2e, 0x7, 0x9, 0x2, 0x2, 0x2e, 0x2f, 0x7, 0xd, 0x2, 
    0x2, 0x2f, 0x9, 0x3, 0x2, 0x2, 0x2, 0x30, 0x31, 0x7, 0x6, 0x2, 0x2, 
    0x31, 0x32, 0x7, 0x9, 0x2, 0x2, 0x32, 0x33, 0x7, 0xd, 0x2, 0x2, 0x33, 
    0xb, 0x3, 0x2, 0x2, 0x2, 0x34, 0x35, 0x7, 0x7, 0x2, 0x2, 0x35, 0x36, 
    0x7, 0x9, 0x2, 0x2, 0x36, 0x37, 0x7, 0xd, 0x2, 0x2, 0x37, 0xd, 0x3, 
    0x2, 0x2, 0x2, 0x38, 0x39, 0x7, 0xc, 0x2, 0x2, 0x39, 0x3b, 0x7, 0xb, 
    0x2, 0x2, 0x3a, 0x3c, 0x7, 0xc, 0x2, 0x2, 0x3b, 0x3a, 0x3, 0x2, 0x2, 
    0x2, 0x3c, 0x3d, 0x3, 0x2, 0x2, 0x2, 0x3d, 0x3b, 0x3, 0x2, 0x2, 0x2, 
    0x3d, 0x3e, 0x3, 0x2, 0x2, 0x2, 0x3e, 0x3f, 0x3, 0x2, 0x2, 0x2, 0x3f, 
    0x40, 0x7, 0xd, 0x2, 0x2, 0x40, 0xf, 0x3, 0x2, 0x2, 0x2, 0x7, 0x15, 
    0x17, 0x1d, 0x24, 0x3d, 
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
