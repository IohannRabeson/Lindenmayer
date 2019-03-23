//
// Created by Io on 2019-03-17.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <LCode/ParseError.hpp>

TEST(ParseErrorTest, ctors)
{
    auto a = ParseError("123", ParseError::Type::Error);
    auto b = ParseError("456", 5, 7, ParseError::Type::Warning);

    EXPECT_EQ( a.type, ParseError::Type::Error );
    EXPECT_EQ( a.message, "123" );
    EXPECT_EQ( a.line, ParseError::Null );
    EXPECT_EQ( a.absolutePosition, ParseError::Null );

    EXPECT_EQ( b.type, ParseError::Type::Warning );
    EXPECT_EQ( b.message, "456" );
    EXPECT_EQ( b.line, 5 );
    EXPECT_EQ( b.absolutePosition, 7 );
}

TEST(ParseErrorTest, containsErrors)
{
    std::vector<ParseError> errors;
    errors.emplace_back("error", ParseError::Type::Error);
    errors.emplace_back("warning", ParseError::Type::Warning);
    errors.emplace_back("info", ParseError::Type::Info);

    EXPECT_TRUE( containsErrors(errors, ParseError::Type::Error) );
    EXPECT_TRUE( containsErrors(errors, ParseError::Type::Warning) );
    EXPECT_TRUE( containsErrors(errors, ParseError::Type::Info) );

    std::vector<ParseError> errors_empty;
    EXPECT_FALSE( containsErrors(errors_empty, ParseError::Type::Error) );
    EXPECT_FALSE( containsErrors(errors_empty, ParseError::Type::Warning) );
    EXPECT_FALSE( containsErrors(errors_empty, ParseError::Type::Info) );

    std::vector<ParseError> errors_only_error;
    errors_only_error.emplace_back("error", ParseError::Type::Error);
    EXPECT_TRUE( containsErrors(errors_only_error, ParseError::Type::Error) );
    EXPECT_FALSE( containsErrors(errors_only_error, ParseError::Type::Warning) );
    EXPECT_FALSE( containsErrors(errors_only_error, ParseError::Type::Info) );
}