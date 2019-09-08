//
// Created by Io on 2019-03-16.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <LCode/StringFormat.hpp>

template <typename ... TS>
void expectFormat(std::string const& expected, std::string const& format, TS&&... args)
{
    auto const result = formatText<StdStringTraits<char>>(format, std::forward<TS>(args)...);

    EXPECT_EQ( expected, result );
}

TEST(StringFormatTest, no_args)
{
    expectFormat("", "");
    expectFormat("a", "a");

    expectFormat("{{a", "{{a");
    expectFormat("}}a", "}}a");
    expectFormat("a}}", "a}}");
    expectFormat("a{{", "a{{");
    expectFormat("{{a", "{{a");
    expectFormat("}}a", "}}a");
    expectFormat("a}}", "a}}");
    expectFormat("a{{", "a{{");

    expectFormat("{{a", "{{a", "b");
    expectFormat("}}a", "}}a", "b");
    expectFormat("a}}", "a}}", "b");
    expectFormat("a{{", "a{{", "b");
    expectFormat("{{a", "{{a", "b");
    expectFormat("}}a", "}}a", "b");
    expectFormat("a}}", "a}}", "b");
    expectFormat("a{{", "a{{", "b");
}

TEST(StringFormatTest, empty_arg)
{
    expectFormat("a", "a{}", "");
    expectFormat("a", "{}a", "");
    expectFormat("", "{}", "");
}

TEST(StringFormatTest, one_arg)
{
    expectFormat("123", "{}", 123);
    expectFormat("a123", "a{}", 123);
    expectFormat("123a", "{}a", 123);
    expectFormat("aba", "a{}", "ba");
    expectFormat("baa", "{}a", "ba");
}

TEST(StringFormatTest, two_arg)
{
    expectFormat("123{}", "{}{}", 123);
    expectFormat("123456", "{}{}", 123, 456);
    expectFormat("123", "{}{}", 123, "");
    expectFormat("456", "{}{}", "", 456);
}