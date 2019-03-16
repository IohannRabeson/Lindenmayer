//
// Created by Io on 2019-03-09.
//

#ifndef LINDENMAYER_SCOPETREEBUILDER_HPP
#define LINDENMAYER_SCOPETREEBUILDER_HPP
#include <generated/LCodeBaseListener.h>
#include "LCode/ParsingContext.hpp"
#include "LCode/ParseError.hpp"

/*!
 * \brief Build the scope tree.
 * This pass doesn't add symbols to scope tree nodes.
 */
class ScopeTreeBuilder : public LCodeBaseListener
{
    ParsingContext& _context;
    ParseErrors& _errors;
    ParsingContext::ScopeNode* _currentScope;
public:
    explicit ScopeTreeBuilder(ParsingContext& context, ParseErrors& errors);
    void enterProgram(LCodeParser::ProgramContext* context) override;
    void exitProgram(LCodeParser::ProgramContext* context) override;
    void enterRewriteRuleDecl(LCodeParser::RewriteRuleDeclContext* context) override;
    void exitRewriteRuleDecl(LCodeParser::RewriteRuleDeclContext* context) override;
    void enterAliasDecl(LCodeParser::AliasDeclContext* context) override;
    void exitAliasDecl(LCodeParser::AliasDeclContext* context) override;
};

#endif //LINDENMAYER_SCOPETREEBUILDER_HPP
