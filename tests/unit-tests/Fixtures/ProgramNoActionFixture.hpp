//
// Created by Io on 17/04/2018.
//

#ifndef LINDENMAYER_PROGRAMNOACTIONFIXTURE_HPP
#define LINDENMAYER_PROGRAMNOACTIONFIXTURE_HPP
#include <gtest/gtest.h>

#include <ModuleTable.hpp>

struct ProgramNoActionFixture : public ::testing::Test
{
    ProgramNoActionFixture()
    {
        moduleTable.registerModule("F");
        moduleTable.registerModule("f");
        moduleTable.registerModule("L");
        moduleTable.registerModule("R");
        moduleTable.registerModule("[");
        moduleTable.registerModule("]");
    }

    lcode::ModuleTable moduleTable;
};

#endif //LINDENMAYER_PROGRAMNOACTIONFIXTURE_HPP
