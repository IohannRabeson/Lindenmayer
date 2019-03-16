//
// Created by Io on 2019-03-16.
//

#ifndef LINDENMAYER_LCODEPROGRAM_HPP
#define LINDENMAYER_LCODEPROGRAM_HPP
#include "LCode/ParsingContext.hpp"
#include "LCode/ParseError.hpp"

#include <memory>
#include <vector>
#include <string>

struct ProgramContent
{
    ParsingContext parsingContext;
    ParseErrors parseErrors;
};

ProgramContent parseLCode(std::string const& text);

class LCodeProgramImp;
class LCodeProgram
{
    std::unique_ptr<LCodeProgramImp> _imp;
public:
    struct Content
    {
        ParsingContext parsingContext;
        ParseErrors parseErrors;
    };

    LCodeProgram();
    ~LCodeProgram();

    Content const& parse(std::string const& text);
};

#endif //LINDENMAYER_LCODEPROGRAM_HPP
