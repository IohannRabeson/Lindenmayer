//
// Created by Io on 21/04/2018.
//

#ifndef LINDENMAYER_PRINTERS_HPP
#define LINDENMAYER_PRINTERS_HPP
#include <Module.hpp>
#include <ostream>

namespace lcode
{
    ::std::ostream& operator << (::std::ostream& os, const Module& module)
    {
        return os << toString(module);
    }
}

#endif //LINDENMAYER_PRINTERS_HPP
