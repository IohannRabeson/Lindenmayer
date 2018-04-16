//
// Created by Io on 15/04/2018.
//

#ifndef LINDENMAYER_OPTIONAL_HPP
#define LINDENMAYER_OPTIONAL_HPP

#include <boost/optional.hpp>

namespace lcode
{
    template <class T>
    using Optional = boost::optional<T>;
}

#endif //LINDENMAYER_OPTIONAL_HPP
