//
// Created by Io on 2019-03-05.
//

#ifndef LINDENMAYER_ABSTRACTSYNTAXTREEALGORITHMS_HPP
#define LINDENMAYER_ABSTRACTSYNTAXTREEALGORITHMS_HPP
#include <functional>
#include <ostream>
#include "StorageType.hpp"

class AbstractSyntaxTreeNode;

/*!
 * \brief Compare two tree
 * \return Return true if the two tree are equals
 */
bool compareTrees(AbstractSyntaxTreeNode const* left, AbstractSyntaxTreeNode const* right);

/*!
 * \brief Export a tree using DOT.
 * \param tree Tree to export
 * \param name Name of the generated DOT graph
 * \param os Output stream
 */
void exportDot(AbstractSyntaxTreeNode const* tree, std::string const& name, std::ostream& os);

StorageTypeTrait<StorageType::Number>::Type reduceAst(AbstractSyntaxTreeNode const* tree);


#endif //LINDENMAYER_ABSTRACTSYNTAXTREEALGORITHMS_HPP
