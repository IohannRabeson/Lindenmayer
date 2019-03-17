//
// Created by Io on 2019-03-05.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <LCode/AbstractSyntaxTreeNode.hpp>
#include <LCode/AbstractSyntaxTreeAlgorithms.hpp>

TEST(AbstractSyntaxTreeTest, CompareTree_01)
{
    std::unique_ptr<AbstractSyntaxTreeNode> rootLeft = std::make_unique<AdditionNode>(nullptr);
    rootLeft->addChild(std::make_unique<LiteralNumberNode>(nullptr, 1));
    rootLeft->addChild(std::make_unique<LiteralNumberNode>(nullptr, 2));

    std::unique_ptr<AbstractSyntaxTreeNode> rootRight = std::make_unique<AdditionNode>(nullptr);
    rootRight->addChild(std::make_unique<LiteralNumberNode>(nullptr, 1));
    rootRight->addChild(std::make_unique<LiteralNumberNode>(nullptr, 2));

    ASSERT_TRUE( compareTrees(rootLeft.get(), rootRight.get()) );
}

TEST(AbstractSyntaxTreeTest, CompareTree_01b)
{
    std::unique_ptr<AbstractSyntaxTreeNode> rootLeft = std::make_unique<AdditionNode>(nullptr);
    rootLeft->addChild(std::make_unique<LiteralNumberNode>(nullptr, 1));
    rootLeft->addChild(std::make_unique<LiteralNumberNode>(nullptr, 2));

    std::unique_ptr<AbstractSyntaxTreeNode> rootRight = std::make_unique<AdditionNode>(nullptr);
    rootRight->addChild(std::make_unique<LiteralNumberNode>(nullptr, 1));
    rootRight->addChild(std::make_unique<LiteralNumberNode>(nullptr, 56789));

    ASSERT_FALSE( compareTrees(rootLeft.get(), rootRight.get()) );
}

TEST(AbstractSyntaxTreeTest, CompareTree_02)
{
    std::unique_ptr<AbstractSyntaxTreeNode> rootLeft = std::make_unique<AdditionNode>(nullptr);
    rootLeft->addChild(std::make_unique<LiteralNumberNode>(nullptr, 1));
    auto* leftLeftAdd = rootLeft->addChild(std::make_unique<AdditionNode>(nullptr));
    leftLeftAdd->addChild(std::make_unique<LiteralNumberNode>(nullptr, 3));
    leftLeftAdd->addChild(std::make_unique<LiteralNumberNode>(nullptr, 4));

    std::unique_ptr<AbstractSyntaxTreeNode> rootRight = std::make_unique<AdditionNode>(nullptr);
    rootRight->addChild(std::make_unique<LiteralNumberNode>(nullptr, 1));
    auto* leftRightAdd = rootRight->addChild(std::make_unique<AdditionNode>(nullptr));
    leftRightAdd->addChild(std::make_unique<LiteralNumberNode>(nullptr, 3));
    leftRightAdd->addChild(std::make_unique<LiteralNumberNode>(nullptr, 4));

    ASSERT_TRUE( compareTrees(rootLeft.get(), rootRight.get()) );
}

TEST(AbstractSyntaxTreeTest, CompareTree_02b)
{
    std::unique_ptr<AbstractSyntaxTreeNode> rootLeft = std::make_unique<AdditionNode>(nullptr);
    rootLeft->addChild(std::make_unique<LiteralNumberNode>(nullptr, 1));
    auto* leftLeftAdd = rootLeft->addChild(std::make_unique<AdditionNode>(nullptr));
    leftLeftAdd->addChild(std::make_unique<LiteralNumberNode>(nullptr, 3));
    leftLeftAdd->addChild(std::make_unique<LiteralNumberNode>(nullptr, 4));

    std::unique_ptr<AbstractSyntaxTreeNode> rootRight = std::make_unique<AdditionNode>(nullptr);
    rootRight->addChild(std::make_unique<LiteralNumberNode>(nullptr, 1));
    auto* leftRightAdd = rootRight->addChild(std::make_unique<AdditionNode>(nullptr));
    leftRightAdd->addChild(std::make_unique<LiteralNumberNode>(nullptr, 3));
    leftRightAdd->addChild(std::make_unique<LiteralNumberNode>(nullptr, 56789));

    ASSERT_FALSE( compareTrees(rootLeft.get(), rootRight.get()) );
}