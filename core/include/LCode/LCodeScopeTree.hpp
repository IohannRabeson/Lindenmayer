//
// Created by Io on 2019-03-16.
//

#ifndef LINDENMAYER_LCODESCOPETREE_HPP
#define LINDENMAYER_LCODESCOPETREE_HPP
#include "LCode/ScopeTree.hpp"
#include "LCode/SymbolTable.hpp"

/*!
 * \brief A specialized scope tree for LCode language
 */
using LCodeScopeTree = ScopeTree<SymbolTable>;

#endif //LINDENMAYER_LCODESCOPETREE_HPP
