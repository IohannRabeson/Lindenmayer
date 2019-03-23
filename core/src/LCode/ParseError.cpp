//
// Created by Io on 2019-03-16.
//

#include "LCode/ParseError.hpp"

namespace
{
    class SearchErrorByType
    {
        ParseError::Type const _type;
    public:
        explicit SearchErrorByType(ParseError::Type type) : _type(type)
        {
        }

        bool operator()(ParseError const& error) const
        {
            return error.type == _type;
        }
    };

    template <typename P>
    bool any(ParseErrors const& errors, P&& predicate)
    {
        return std::find_if(errors.begin(), errors.end(), predicate) != errors.end();
    }
}

bool containsErrors(ParseErrors const& errors, ParseError::Type type)
{
    return any(errors, SearchErrorByType(type));
}

ParseError::ParseError(std::string const& message, ParseError::Type type)
    : ParseError(message, Null, Null, type)
{
}

ParseError::ParseError(std::string const& message, std::size_t line, std::size_t absolutePosition, ParseError::Type type)
    : message(message)
    , line(line)
    , absolutePosition(absolutePosition)
    , type(type)
{
}

std::size_t const ParseError::Null = std::numeric_limits<std::size_t>::max();
