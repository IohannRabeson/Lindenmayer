//
// Created by Io on 2019-03-09.
//

#ifndef LINDENMAYER_SCOPETREEBUILDER_HPP
#define LINDENMAYER_SCOPETREEBUILDER_HPP
#include <generated/LCodeBaseListener.h>
#include "LCode/Context.hpp"

/*!
 * \brief Build the scope tree.
 * This pass doesn't add symbols to scope tree nodes.
 */
class ScopeTreeBuilder : public LCodeBaseListener
{
    Context& _context;
    Context::ScopeNode* _currentScope;
public:
    explicit ScopeTreeBuilder(Context& context);
    void enterProgram(LCodeParser::ProgramContext* context) override;
    void exitProgram(LCodeParser::ProgramContext* context) override;
    void enterRewriteRuleDecl(LCodeParser::RewriteRuleDeclContext* context) override;
    void enterAliasDecl(LCodeParser::AliasDeclContext* context) override;
};

#endif //LINDENMAYER_SCOPETREEBUILDER_HPP
