//
// Created by Io on 2019-03-05.
//

#ifndef LINDENMAYER_ABSTRACTSYNTAXTREEALGORITHMS_HPP
#define LINDENMAYER_ABSTRACTSYNTAXTREEALGORITHMS_HPP
#include <functional>
#include <ostream>

class AbstractSyntaxTreeNode;

bool compareTrees(AbstractSyntaxTreeNode const* left, AbstractSyntaxTreeNode const* right);
void exportDot(AbstractSyntaxTreeNode const* tree, std::string const& name, std::ostream& os);

#endif //LINDENMAYER_ABSTRACTSYNTAXTREEALGORITHMS_HPP
