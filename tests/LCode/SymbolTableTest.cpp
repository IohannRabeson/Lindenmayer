//
// Created by Io on 2019-02-23.
//
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <LCode/SymbolTable.hpp>

TEST(LCodeSymbolTable, add)
{
    SymbolTable table;
    table.define("a", std::make_unique<SymbolTable::FloatConstant>(1.f));
    table.define("b", std::make_unique<SymbolTable::IntegerConstant>(1));
    table.define("c", std::make_unique<SymbolTable::BooleanConstant>(true));
    table.define("d", std::make_unique<SymbolTable::StringConstant>("abc"));
    table.define("e", std::make_unique<SymbolTable::Function>(2u));
    EXPECT_TRUE( table.contains("a") );
    EXPECT_TRUE( table.contains("b") );
    EXPECT_TRUE( table.contains("c") );
    EXPECT_TRUE( table.contains("d") );
    EXPECT_TRUE( table.contains("e") );
    EXPECT_EQ( table.symbolType("a"), SymbolTable::SymbolType::Constant );
    EXPECT_EQ( table.symbolType("b"), SymbolTable::SymbolType::Constant );
    EXPECT_EQ( table.symbolType("c"), SymbolTable::SymbolType::Constant );
    EXPECT_EQ( table.symbolType("d"), SymbolTable::SymbolType::Constant );
    EXPECT_EQ( table.symbolType("e"), SymbolTable::SymbolType::Function );
    EXPECT_EQ( table.symbolType("no"), SymbolTable::SymbolType::Null );
    EXPECT_FALSE( table.contains("no") );
    EXPECT_EQ( table.symbolType("a"), SymbolTable::SymbolType::Constant );
    EXPECT_EQ( table.symbolType("b"), SymbolTable::SymbolType::Constant );
    EXPECT_EQ( table.symbolType("c"), SymbolTable::SymbolType::Constant );
    EXPECT_EQ( table.symbolType("d"), SymbolTable::SymbolType::Constant );
    EXPECT_EQ( table.symbolType("e"), SymbolTable::SymbolType::Function );
}
