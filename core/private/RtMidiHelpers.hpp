#ifndef RTMIDIHELPERS_HPP
#define RTMIDIHELPERS_HPP

#include <memory>
#include <RtMidi.h>

namespace imp
{
    void setMidiPortEnabled(std::unique_ptr<RtMidiIn> const& midiIn, RtMidiIn::RtMidiCallback callback, void* const userData,
                            bool const enable);

    template <class P>
    inline bool ifPortIsOpen(std::shared_ptr<P> const& port, int const portID)
    {
        return port->isPortOpen() || port->openPort(portID);
    }
}

#endif // RTMIDIHELPERS_HPP
