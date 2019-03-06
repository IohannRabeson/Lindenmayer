//
// Created by Io on 2019-03-03.
//

#ifndef LINDENMAYER_COMPILATIONCONTEXT_HPP
#define LINDENMAYER_COMPILATIONCONTEXT_HPP
#include <antlr4-runtime.h>
#include <map>
#include <memory>
#include "LCode/AbstractSyntaxTreeNode.hpp"
#include <generated/LCodeParser.h>

class CompilationContext
{
public:
    CompilationContext() = default;
    ~CompilationContext();

    /*!
     * \brief Lists root constants expressions
     * Theses are the expressions which should be reduced or are already reduced.
     */
    std::map<LCodeParser::Const_expressionContext*, ExpressionNodePtr> constExpressions;
};

#endif //LINDENMAYER_COMPILATIONCONTEXT_HPP
