//
// Created by Io on 2019-03-09.
//

#ifndef LINDENMAYER_SCOPETREEBUILDER_HPP
#define LINDENMAYER_SCOPETREEBUILDER_HPP
#include <generated/LCodeBaseListener.h>
#include "LCode/LCodeScopeTree.hpp"
#include "LCode/ParseError.hpp"

/*!
 * \brief Build the scope tree.
 * This pass doesn't add symbols to scope tree nodes.
 */
class LCodeScopeTreeBuilder : public LCodeBaseListener
{
    LCodeScopeTree& _scopeTree;
    LCodeScopeTree::NodeType* _currentScope;
public:
    explicit LCodeScopeTreeBuilder(LCodeScopeTree& scopeTree);
    void enterProgram(LCodeParser::ProgramContext* context) override;
    void exitProgram(LCodeParser::ProgramContext* context) override;
    void enterRewriteRuleDecl(LCodeParser::RewriteRuleDeclContext* context) override;
    void exitRewriteRuleDecl(LCodeParser::RewriteRuleDeclContext* context) override;
    void enterAliasDecl(LCodeParser::AliasDeclContext* context) override;
    void exitAliasDecl(LCodeParser::AliasDeclContext* context) override;
};

#endif //LINDENMAYER_SCOPETREEBUILDER_HPP
