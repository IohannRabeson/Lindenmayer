
// Generated from LSystem.g4 by ANTLR 4.7.1


#include "LSystemLexer.h"


using namespace antlr4;


LSystemLexer::LSystemLexer(CharStream *input) : Lexer(input) {
  _interpreter = new atn::LexerATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

LSystemLexer::~LSystemLexer() {
  delete _interpreter;
}

std::string LSystemLexer::getGrammarFileName() const {
  return "LSystem.g4";
}

const std::vector<std::string>& LSystemLexer::getRuleNames() const {
  return _ruleNames;
}

const std::vector<std::string>& LSystemLexer::getChannelNames() const {
  return _channelNames;
}

const std::vector<std::string>& LSystemLexer::getModeNames() const {
  return _modeNames;
}

const std::vector<std::string>& LSystemLexer::getTokenNames() const {
  return _tokenNames;
}

dfa::Vocabulary& LSystemLexer::getVocabulary() const {
  return _vocabulary;
}

const std::vector<uint16_t> LSystemLexer::getSerializedATN() const {
  return _serializedATN;
}

const atn::ATN& LSystemLexer::getATN() const {
  return _atn;
}




// Static vars and initialization.
std::vector<dfa::DFA> LSystemLexer::_decisionToDFA;
atn::PredictionContextCache LSystemLexer::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN LSystemLexer::_atn;
std::vector<uint16_t> LSystemLexer::_serializedATN;

std::vector<std::string> LSystemLexer::_ruleNames = {
  u8"T__0", u8"T__1", u8"T__2", u8"T__3", u8"T__4", u8"T__5", u8"DIGIT09", 
  u8"DIGIT19", u8"TRUE", u8"FALSE", u8"NEWLINE", u8"Integer", u8"Float", 
  u8"Boolean", u8"TransformOperator", u8"Identifier", u8"EndOfLine", u8"Newline", 
  u8"Whitespace"
};

std::vector<std::string> LSystemLexer::_channelNames = {
  "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
};

std::vector<std::string> LSystemLexer::_modeNames = {
  u8"DEFAULT_MODE"
};

std::vector<std::string> LSystemLexer::_literalNames = {
  "", u8"'axiom:'", u8"'iterations:'", u8"'distance:'", u8"'angle:'", u8"'initialAngle:'", 
  u8"'alias'", "", "", "", u8"'='", "", u8"';'"
};

std::vector<std::string> LSystemLexer::_symbolicNames = {
  "", "", "", "", "", "", "", u8"Integer", u8"Float", u8"Boolean", u8"TransformOperator", 
  u8"Identifier", u8"EndOfLine", u8"Newline", u8"Whitespace"
};

dfa::Vocabulary LSystemLexer::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> LSystemLexer::_tokenNames;

LSystemLexer::Initializer::Initializer() {
  // This code could be in a static initializer lambda, but VS doesn't allow access to private class members from there.
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
    0x2, 0x10, 0xab, 0x8, 0x1, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 
    0x4, 0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 
    0x7, 0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 0x4, 
    0xb, 0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 0xe, 0x9, 
    0xe, 0x4, 0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 0x9, 0x11, 
    0x4, 0x12, 0x9, 0x12, 0x4, 0x13, 0x9, 0x13, 0x4, 0x14, 0x9, 0x14, 0x3, 
    0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 
    0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 
    0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 
    0x6, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 
    0x8, 0x3, 0x8, 0x3, 0x9, 0x3, 0x9, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 
    0xa, 0x3, 0xa, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 
    0xb, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x5, 0xc, 0x74, 0xa, 0xc, 0x3, 0xd, 
    0x5, 0xd, 0x77, 0xa, 0xd, 0x3, 0xd, 0x6, 0xd, 0x7a, 0xa, 0xd, 0xd, 0xd, 
    0xe, 0xd, 0x7b, 0x3, 0xe, 0x5, 0xe, 0x7f, 0xa, 0xe, 0x3, 0xe, 0x3, 0xe, 
    0x7, 0xe, 0x83, 0xa, 0xe, 0xc, 0xe, 0xe, 0xe, 0x86, 0xb, 0xe, 0x3, 0xe, 
    0x5, 0xe, 0x89, 0xa, 0xe, 0x3, 0xe, 0x7, 0xe, 0x8c, 0xa, 0xe, 0xc, 0xe, 
    0xe, 0xe, 0x8f, 0xb, 0xe, 0x3, 0xf, 0x3, 0xf, 0x5, 0xf, 0x93, 0xa, 0xf, 
    0x3, 0x10, 0x3, 0x10, 0x3, 0x11, 0x5, 0x11, 0x98, 0xa, 0x11, 0x3, 0x12, 
    0x3, 0x12, 0x3, 0x13, 0x3, 0x13, 0x5, 0x13, 0x9e, 0xa, 0x13, 0x3, 0x13, 
    0x5, 0x13, 0xa1, 0xa, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x14, 0x6, 0x14, 
    0xa6, 0xa, 0x14, 0xd, 0x14, 0xe, 0x14, 0xa7, 0x3, 0x14, 0x3, 0x14, 0x2, 
    0x2, 0x15, 0x3, 0x3, 0x5, 0x4, 0x7, 0x5, 0x9, 0x6, 0xb, 0x7, 0xd, 0x8, 
    0xf, 0x2, 0x11, 0x2, 0x13, 0x2, 0x15, 0x2, 0x17, 0x2, 0x19, 0x9, 0x1b, 
    0xa, 0x1d, 0xb, 0x1f, 0xc, 0x21, 0xd, 0x23, 0xe, 0x25, 0xf, 0x27, 0x10, 
    0x3, 0x2, 0x8, 0x3, 0x2, 0x32, 0x3b, 0x3, 0x2, 0x33, 0x3b, 0x4, 0x2, 
    0xc, 0xc, 0xf, 0xf, 0x4, 0x2, 0x2d, 0x2d, 0x2f, 0x2f, 0x8, 0x2, 0x2d, 
    0x2d, 0x2f, 0x2f, 0x43, 0x5d, 0x5f, 0x5f, 0x61, 0x61, 0x63, 0x7c, 0x4, 
    0x2, 0xb, 0xb, 0x22, 0x22, 0x2, 0xb0, 0x2, 0x3, 0x3, 0x2, 0x2, 0x2, 
    0x2, 0x5, 0x3, 0x2, 0x2, 0x2, 0x2, 0x7, 0x3, 0x2, 0x2, 0x2, 0x2, 0x9, 
    0x3, 0x2, 0x2, 0x2, 0x2, 0xb, 0x3, 0x2, 0x2, 0x2, 0x2, 0xd, 0x3, 0x2, 
    0x2, 0x2, 0x2, 0x19, 0x3, 0x2, 0x2, 0x2, 0x2, 0x1b, 0x3, 0x2, 0x2, 0x2, 
    0x2, 0x1d, 0x3, 0x2, 0x2, 0x2, 0x2, 0x1f, 0x3, 0x2, 0x2, 0x2, 0x2, 0x21, 
    0x3, 0x2, 0x2, 0x2, 0x2, 0x23, 0x3, 0x2, 0x2, 0x2, 0x2, 0x25, 0x3, 0x2, 
    0x2, 0x2, 0x2, 0x27, 0x3, 0x2, 0x2, 0x2, 0x3, 0x29, 0x3, 0x2, 0x2, 0x2, 
    0x5, 0x30, 0x3, 0x2, 0x2, 0x2, 0x7, 0x3c, 0x3, 0x2, 0x2, 0x2, 0x9, 0x46, 
    0x3, 0x2, 0x2, 0x2, 0xb, 0x4d, 0x3, 0x2, 0x2, 0x2, 0xd, 0x5b, 0x3, 0x2, 
    0x2, 0x2, 0xf, 0x61, 0x3, 0x2, 0x2, 0x2, 0x11, 0x63, 0x3, 0x2, 0x2, 
    0x2, 0x13, 0x65, 0x3, 0x2, 0x2, 0x2, 0x15, 0x6a, 0x3, 0x2, 0x2, 0x2, 
    0x17, 0x73, 0x3, 0x2, 0x2, 0x2, 0x19, 0x76, 0x3, 0x2, 0x2, 0x2, 0x1b, 
    0x7e, 0x3, 0x2, 0x2, 0x2, 0x1d, 0x92, 0x3, 0x2, 0x2, 0x2, 0x1f, 0x94, 
    0x3, 0x2, 0x2, 0x2, 0x21, 0x97, 0x3, 0x2, 0x2, 0x2, 0x23, 0x99, 0x3, 
    0x2, 0x2, 0x2, 0x25, 0xa0, 0x3, 0x2, 0x2, 0x2, 0x27, 0xa5, 0x3, 0x2, 
    0x2, 0x2, 0x29, 0x2a, 0x7, 0x63, 0x2, 0x2, 0x2a, 0x2b, 0x7, 0x7a, 0x2, 
    0x2, 0x2b, 0x2c, 0x7, 0x6b, 0x2, 0x2, 0x2c, 0x2d, 0x7, 0x71, 0x2, 0x2, 
    0x2d, 0x2e, 0x7, 0x6f, 0x2, 0x2, 0x2e, 0x2f, 0x7, 0x3c, 0x2, 0x2, 0x2f, 
    0x4, 0x3, 0x2, 0x2, 0x2, 0x30, 0x31, 0x7, 0x6b, 0x2, 0x2, 0x31, 0x32, 
    0x7, 0x76, 0x2, 0x2, 0x32, 0x33, 0x7, 0x67, 0x2, 0x2, 0x33, 0x34, 0x7, 
    0x74, 0x2, 0x2, 0x34, 0x35, 0x7, 0x63, 0x2, 0x2, 0x35, 0x36, 0x7, 0x76, 
    0x2, 0x2, 0x36, 0x37, 0x7, 0x6b, 0x2, 0x2, 0x37, 0x38, 0x7, 0x71, 0x2, 
    0x2, 0x38, 0x39, 0x7, 0x70, 0x2, 0x2, 0x39, 0x3a, 0x7, 0x75, 0x2, 0x2, 
    0x3a, 0x3b, 0x7, 0x3c, 0x2, 0x2, 0x3b, 0x6, 0x3, 0x2, 0x2, 0x2, 0x3c, 
    0x3d, 0x7, 0x66, 0x2, 0x2, 0x3d, 0x3e, 0x7, 0x6b, 0x2, 0x2, 0x3e, 0x3f, 
    0x7, 0x75, 0x2, 0x2, 0x3f, 0x40, 0x7, 0x76, 0x2, 0x2, 0x40, 0x41, 0x7, 
    0x63, 0x2, 0x2, 0x41, 0x42, 0x7, 0x70, 0x2, 0x2, 0x42, 0x43, 0x7, 0x65, 
    0x2, 0x2, 0x43, 0x44, 0x7, 0x67, 0x2, 0x2, 0x44, 0x45, 0x7, 0x3c, 0x2, 
    0x2, 0x45, 0x8, 0x3, 0x2, 0x2, 0x2, 0x46, 0x47, 0x7, 0x63, 0x2, 0x2, 
    0x47, 0x48, 0x7, 0x70, 0x2, 0x2, 0x48, 0x49, 0x7, 0x69, 0x2, 0x2, 0x49, 
    0x4a, 0x7, 0x6e, 0x2, 0x2, 0x4a, 0x4b, 0x7, 0x67, 0x2, 0x2, 0x4b, 0x4c, 
    0x7, 0x3c, 0x2, 0x2, 0x4c, 0xa, 0x3, 0x2, 0x2, 0x2, 0x4d, 0x4e, 0x7, 
    0x6b, 0x2, 0x2, 0x4e, 0x4f, 0x7, 0x70, 0x2, 0x2, 0x4f, 0x50, 0x7, 0x6b, 
    0x2, 0x2, 0x50, 0x51, 0x7, 0x76, 0x2, 0x2, 0x51, 0x52, 0x7, 0x6b, 0x2, 
    0x2, 0x52, 0x53, 0x7, 0x63, 0x2, 0x2, 0x53, 0x54, 0x7, 0x6e, 0x2, 0x2, 
    0x54, 0x55, 0x7, 0x43, 0x2, 0x2, 0x55, 0x56, 0x7, 0x70, 0x2, 0x2, 0x56, 
    0x57, 0x7, 0x69, 0x2, 0x2, 0x57, 0x58, 0x7, 0x6e, 0x2, 0x2, 0x58, 0x59, 
    0x7, 0x67, 0x2, 0x2, 0x59, 0x5a, 0x7, 0x3c, 0x2, 0x2, 0x5a, 0xc, 0x3, 
    0x2, 0x2, 0x2, 0x5b, 0x5c, 0x7, 0x63, 0x2, 0x2, 0x5c, 0x5d, 0x7, 0x6e, 
    0x2, 0x2, 0x5d, 0x5e, 0x7, 0x6b, 0x2, 0x2, 0x5e, 0x5f, 0x7, 0x63, 0x2, 
    0x2, 0x5f, 0x60, 0x7, 0x75, 0x2, 0x2, 0x60, 0xe, 0x3, 0x2, 0x2, 0x2, 
    0x61, 0x62, 0x9, 0x2, 0x2, 0x2, 0x62, 0x10, 0x3, 0x2, 0x2, 0x2, 0x63, 
    0x64, 0x9, 0x3, 0x2, 0x2, 0x64, 0x12, 0x3, 0x2, 0x2, 0x2, 0x65, 0x66, 
    0x7, 0x76, 0x2, 0x2, 0x66, 0x67, 0x7, 0x74, 0x2, 0x2, 0x67, 0x68, 0x7, 
    0x77, 0x2, 0x2, 0x68, 0x69, 0x7, 0x67, 0x2, 0x2, 0x69, 0x14, 0x3, 0x2, 
    0x2, 0x2, 0x6a, 0x6b, 0x7, 0x68, 0x2, 0x2, 0x6b, 0x6c, 0x7, 0x63, 0x2, 
    0x2, 0x6c, 0x6d, 0x7, 0x6e, 0x2, 0x2, 0x6d, 0x6e, 0x7, 0x75, 0x2, 0x2, 
    0x6e, 0x6f, 0x7, 0x67, 0x2, 0x2, 0x6f, 0x16, 0x3, 0x2, 0x2, 0x2, 0x70, 
    0x71, 0x7, 0xf, 0x2, 0x2, 0x71, 0x74, 0x7, 0xc, 0x2, 0x2, 0x72, 0x74, 
    0x9, 0x4, 0x2, 0x2, 0x73, 0x70, 0x3, 0x2, 0x2, 0x2, 0x73, 0x72, 0x3, 
    0x2, 0x2, 0x2, 0x74, 0x18, 0x3, 0x2, 0x2, 0x2, 0x75, 0x77, 0x9, 0x5, 
    0x2, 0x2, 0x76, 0x75, 0x3, 0x2, 0x2, 0x2, 0x76, 0x77, 0x3, 0x2, 0x2, 
    0x2, 0x77, 0x79, 0x3, 0x2, 0x2, 0x2, 0x78, 0x7a, 0x5, 0xf, 0x8, 0x2, 
    0x79, 0x78, 0x3, 0x2, 0x2, 0x2, 0x7a, 0x7b, 0x3, 0x2, 0x2, 0x2, 0x7b, 
    0x79, 0x3, 0x2, 0x2, 0x2, 0x7b, 0x7c, 0x3, 0x2, 0x2, 0x2, 0x7c, 0x1a, 
    0x3, 0x2, 0x2, 0x2, 0x7d, 0x7f, 0x9, 0x5, 0x2, 0x2, 0x7e, 0x7d, 0x3, 
    0x2, 0x2, 0x2, 0x7e, 0x7f, 0x3, 0x2, 0x2, 0x2, 0x7f, 0x80, 0x3, 0x2, 
    0x2, 0x2, 0x80, 0x84, 0x5, 0x11, 0x9, 0x2, 0x81, 0x83, 0x5, 0xf, 0x8, 
    0x2, 0x82, 0x81, 0x3, 0x2, 0x2, 0x2, 0x83, 0x86, 0x3, 0x2, 0x2, 0x2, 
    0x84, 0x82, 0x3, 0x2, 0x2, 0x2, 0x84, 0x85, 0x3, 0x2, 0x2, 0x2, 0x85, 
    0x88, 0x3, 0x2, 0x2, 0x2, 0x86, 0x84, 0x3, 0x2, 0x2, 0x2, 0x87, 0x89, 
    0x7, 0x30, 0x2, 0x2, 0x88, 0x87, 0x3, 0x2, 0x2, 0x2, 0x88, 0x89, 0x3, 
    0x2, 0x2, 0x2, 0x89, 0x8d, 0x3, 0x2, 0x2, 0x2, 0x8a, 0x8c, 0x5, 0xf, 
    0x8, 0x2, 0x8b, 0x8a, 0x3, 0x2, 0x2, 0x2, 0x8c, 0x8f, 0x3, 0x2, 0x2, 
    0x2, 0x8d, 0x8b, 0x3, 0x2, 0x2, 0x2, 0x8d, 0x8e, 0x3, 0x2, 0x2, 0x2, 
    0x8e, 0x1c, 0x3, 0x2, 0x2, 0x2, 0x8f, 0x8d, 0x3, 0x2, 0x2, 0x2, 0x90, 
    0x93, 0x5, 0x13, 0xa, 0x2, 0x91, 0x93, 0x5, 0x15, 0xb, 0x2, 0x92, 0x90, 
    0x3, 0x2, 0x2, 0x2, 0x92, 0x91, 0x3, 0x2, 0x2, 0x2, 0x93, 0x1e, 0x3, 
    0x2, 0x2, 0x2, 0x94, 0x95, 0x7, 0x3f, 0x2, 0x2, 0x95, 0x20, 0x3, 0x2, 
    0x2, 0x2, 0x96, 0x98, 0x9, 0x6, 0x2, 0x2, 0x97, 0x96, 0x3, 0x2, 0x2, 
    0x2, 0x98, 0x22, 0x3, 0x2, 0x2, 0x2, 0x99, 0x9a, 0x7, 0x3d, 0x2, 0x2, 
    0x9a, 0x24, 0x3, 0x2, 0x2, 0x2, 0x9b, 0x9d, 0x7, 0xf, 0x2, 0x2, 0x9c, 
    0x9e, 0x7, 0xc, 0x2, 0x2, 0x9d, 0x9c, 0x3, 0x2, 0x2, 0x2, 0x9d, 0x9e, 
    0x3, 0x2, 0x2, 0x2, 0x9e, 0xa1, 0x3, 0x2, 0x2, 0x2, 0x9f, 0xa1, 0x7, 
    0xc, 0x2, 0x2, 0xa0, 0x9b, 0x3, 0x2, 0x2, 0x2, 0xa0, 0x9f, 0x3, 0x2, 
    0x2, 0x2, 0xa1, 0xa2, 0x3, 0x2, 0x2, 0x2, 0xa2, 0xa3, 0x8, 0x13, 0x2, 
    0x2, 0xa3, 0x26, 0x3, 0x2, 0x2, 0x2, 0xa4, 0xa6, 0x9, 0x7, 0x2, 0x2, 
    0xa5, 0xa4, 0x3, 0x2, 0x2, 0x2, 0xa6, 0xa7, 0x3, 0x2, 0x2, 0x2, 0xa7, 
    0xa5, 0x3, 0x2, 0x2, 0x2, 0xa7, 0xa8, 0x3, 0x2, 0x2, 0x2, 0xa8, 0xa9, 
    0x3, 0x2, 0x2, 0x2, 0xa9, 0xaa, 0x8, 0x14, 0x2, 0x2, 0xaa, 0x28, 0x3, 
    0x2, 0x2, 0x2, 0xf, 0x2, 0x73, 0x76, 0x7b, 0x7e, 0x84, 0x88, 0x8d, 0x92, 
    0x97, 0x9d, 0xa0, 0xa7, 0x3, 0x8, 0x2, 0x2, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

LSystemLexer::Initializer LSystemLexer::_init;
