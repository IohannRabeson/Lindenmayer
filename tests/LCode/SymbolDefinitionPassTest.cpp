//
// Created by Io on 2019-03-02.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <LCode/SymbolDefinitionPass.hpp>
#include <generated/LCodeListener.h>

#include "ParserUtility.hpp"

// TODO: clean this shit: move this test to a dedicated file GatherConstantExpressionPassTest.cpp
#include "LCode/GatherConstantExpressionPass.hpp"
#include "LCode/CompilationContext.hpp"
TEST(Temp, temp)
{
    CompilationContext context;
    GatherConstantExpressionPass listener(context);
    std::string const code =
        "const integer integer_value = 123;"
        "const float float_value = 0.123;"
        "const float float_value = 1 + 2 * 3 - 4 / 5;"
        "const boolean boolean_value_true = true;"
        "const boolean boolean_value_false = false;"
        "const string string_value = \"123\";";
    parseLCode(code, listener);
}
// ENDTODO: clean this shit

TEST(SymbolDefinitionPassTest, constants)
{
    SymbolDefinitionPass listener;
    std::string const code =
        "const integer integer_value = 123;"
        "const float float_value = 0.123;"
        "const boolean boolean_value_true = true;"
        "const boolean boolean_value_false = false;"
        "const string string_value = \"123\";";
    parseLCode(code, listener);

    auto* const root = listener.scopeTree().root();
    ASSERT_EQ( listener.scopeTree().symbolType(root, "integer"), SymbolTable::SymbolType::Type );
    ASSERT_EQ( listener.scopeTree().symbolType(root, "float"), SymbolTable::SymbolType::Type );
    ASSERT_EQ( listener.scopeTree().symbolType(root, "boolean"), SymbolTable::SymbolType::Type );
    ASSERT_EQ( listener.scopeTree().symbolType(root, "string"), SymbolTable::SymbolType::Type );
    ASSERT_EQ( listener.scopeTree().symbolType(root, "void"), SymbolTable::SymbolType::Type );
    EXPECT_EQ( listener.scopeTree().symbolType(root, "integer_value"), SymbolTable::SymbolType::Constant );
    EXPECT_EQ( listener.scopeTree().symbolType(root, "float_value"), SymbolTable::SymbolType::Constant );
    EXPECT_EQ( listener.scopeTree().symbolType(root, "boolean_value_true"), SymbolTable::SymbolType::Constant );
    EXPECT_EQ( listener.scopeTree().symbolType(root, "boolean_value_false"), SymbolTable::SymbolType::Constant );
    EXPECT_EQ( listener.scopeTree().symbolType(root, "string_value"), SymbolTable::SymbolType::Constant );
}