//
// Created by Io on 02/05/2018.
//

#include <Program.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

struct ExpressionTest : public ::testing::Test
{
    ExpressionTest()
    {
        moduleTable.registerModule("F");
        moduleTable.registerModule("f");
        moduleTable.registerModule("L");
        moduleTable.registerModule("R");
        moduleTable.registerModule("[");
        moduleTable.registerModule("]");
        moduleTable.registerModule("+");
        moduleTable.registerModule("-");
    }

    lcode::ModuleTable moduleTable;
};