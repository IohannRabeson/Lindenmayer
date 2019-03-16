//
// Created by Io on 2019-03-16.
//

#ifndef LINDENMAYER_STRINGFORMAT_HPP
#define LINDENMAYER_STRINGFORMAT_HPP
#include <string>
#include <array>

template <typename E>
struct StdStringTraits;

template <>
struct StdStringTraits<char>
{
    using CharType = char;
    using StringType = std::basic_string<CharType>;
    static constexpr char const BeginPlaceholder = '{';
    static constexpr char const EndPlaceholder = '}';

    template <typename T>
    static std::string toString(T value)
    {
        return std::to_string(value);
    }
};

template <>
struct StdStringTraits<wchar_t>
{
    using CharType = wchar_t;
    using StringType = std::basic_string<CharType>;
    static constexpr char const BeginPlaceholder = L'{';
    static constexpr char const EndPlaceholder = L'}';

    template <typename T>
    static std::wstring toString(T value)
    {
        return std::to_wstring(value);
    }
};

template <typename StringTraitsType, typename ... TS>
using StringificationArray = std::array<typename StringTraitsType::StringType, sizeof...(TS)>;

template <typename StringTraitsType, typename ... TS>
void stringify(StringificationArray<StringTraitsType, TS...>& results, TS&&... args)
{
    std::size_t index = 0u;
    auto f = [&index, &results](auto&& arg)
    {
        using ArgType = typename std::remove_all_extents<decltype(arg)>::type;
        using StringType = typename StringTraitsType::StringType;
        constexpr bool const IsString = std::is_convertible<ArgType, StringType>::value;

        if constexpr (IsString)
        {
            // Type of arg is convertible to a string
            results[index] = arg;
        }
        else
        {
            // Type of arg is not convertible to string, try to use StringTraitsType::toString
            results[index] = StringTraitsType::toString(arg);
        }
        ++index;
    };
    // Fold expression: call f for each variadic arguments args
    (f(args), ...);
}

/*!
 * \brief Simple text format function which replaces `{}` by \param args
 * \tparam StringTraitsType
 * \tparam TS Infered by the compiler. Type of each arguments `args`.
 * \param format Specify how to interpret the arguments.
 * \param args Arguments specifying data to print.
 * \return Formated text.
 * \code
 *   std::cout << formatText<StdStringTraits<char>>("hello {}!\n", "Bob");
 * \endcode
 */
template <typename StringTraitsType, typename ... TS>
typename StringTraitsType::StringType formatText(typename StringTraitsType::StringType const& format, TS&&...args)
{
    auto isPlaceholder = [](auto const begin, auto const end) -> bool
    {
        auto const currentChar = *begin;
        auto const nextIt = begin + 1;

        return (currentChar == StringTraitsType::BeginPlaceholder
                && nextIt != end
                && *nextIt == StringTraitsType::EndPlaceholder);
    };

    std::string result;
    // This array contains strings. The number of elements is defined by the number of arguments in TS.
    StringificationArray<StringTraitsType, TS...> placeholderValues;
    // Fill placeholderValues by arguments converted to strings
    stringify<StringTraitsType>(placeholderValues, std::forward<TS>(args)...);
    auto placeholderIt = placeholderValues.cbegin();
    auto it = format.cbegin();

    // TODO: maybe reserve memory using a heurisic to define
    // how many bytes to reserve?
    while (it != format.cend())
    {
        auto const currentChar = *it;

        if (placeholderIt != placeholderValues.end() && isPlaceholder(it, format.cend()))
        {
            result += *placeholderIt;
            ++placeholderIt;
            ++it; // advance twice to skip the entire placeholder
        }
        else
        {
            result.push_back(currentChar);
        }
        ++it;
    }

    return result;
}

template <typename ... TS>
std::string formatText(std::string const& format, TS&&... args)
{
    return formatText<StdStringTraits<char>>(format, std::forward<TS>(args)...);
}

template <typename ... TS>
std::wstring formatText(std::wstring const& format, TS&&... args)
{
    return formatText<StdStringTraits<wchar_t >>(format, std::forward<TS>(args)...);
}

#endif //LINDENMAYER_STRINGFORMAT_HPP
