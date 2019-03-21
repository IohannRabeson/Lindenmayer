//
// Created by Io on 2019-03-05.
//

#ifndef LINDENMAYER_PARSERUTILITY_HPP
#define LINDENMAYER_PARSERUTILITY_HPP
#include <antlr4-runtime.h>
#include <string>

void parseLCode(std::string const& text, antlr4::tree::ParseTreeListener& listener);

#endif //LINDENMAYER_PARSERUTILITY_HPP
