//
// Created by Io on 08/03/2018.
//

#ifndef MIDIMONITOR_FILTERHELPERS_HPP
#define MIDIMONITOR_FILTERHELPERS_HPP
#include "QMidiMessage.hpp"
#include "QAbstractMidiMessageFilter.hpp"

#include <algorithm>

namespace imp
{
    template<class IT>
    inline bool acceptMessage(QMidiMessage const& message, IT begin, IT end)
    {
        return begin == end || std::all_of(begin, end,
                                           [&message](auto const& filter)
                                           {
                                               return filter->acceptMessage(message);
                                           });
    }
}

#endif //MIDIMONITOR_FILTERHELPERS_HPP
