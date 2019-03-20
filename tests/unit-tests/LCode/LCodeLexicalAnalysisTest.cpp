//
// Created by Io on 2019-03-17.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <LCode/ParseErrorListener.hpp>
#include <generated/LCodeLexer.h>
#include <generated/LCodeParser.h>

class LCodeLexicalAnalysisTest : public ::testing::Test
{
public:
    ParseErrors errors;

    bool parse(std::string const& text)
    {
        SCOPED_TRACE("Parsing '" + text + "'");
        antlr4::ANTLRInputStream inputStream(text);
        LCodeLexer lexer(&inputStream);
        antlr4::CommonTokenStream tokenStream(&lexer);
        LCodeParser parser(&tokenStream);
        antlr4::tree::ParseTreeWalker treeWalker;

        errors.clear();

        ParseErrorListener errorListener(errors);
        parser.removeErrorListeners();
        parser.addErrorListener(&errorListener);

        parser.program();

        return errors.empty();
    }
};

TEST_F(LCodeLexicalAnalysisTest, empty_program)
{
    EXPECT_FALSE( parse("") );
    EXPECT_FALSE( parse(" ") );
    EXPECT_FALSE( parse("\n") );
}

TEST_F(LCodeLexicalAnalysisTest, whitespace_skip)
{
    EXPECT_TRUE( parse("\nnumber\na\n=\n123.0\n;\n") );
    EXPECT_FALSE( parse("num\nber a = 123;") );
}

TEST_F(LCodeLexicalAnalysisTest, number_declarations)
{
    EXPECT_FALSE( parse("number") );
    EXPECT_FALSE( parse("number a") );
    EXPECT_FALSE( parse("number a =") );
    EXPECT_FALSE( parse("number a =;") );
    EXPECT_FALSE( parse("number a =;123") );
    EXPECT_FALSE( parse("a = 123;") );
    EXPECT_FALSE( parse("number number = 123;") );
}