#ifndef RTMIDIHELPERS_HPP
#define RTMIDIHELPERS_HPP

#include <memory>
#include <RtMidi.h>
#include <QtGlobal>
#include "QMidiMessage.hpp"

class QAbstractMidiIn;
class QAbstractMidiOut;

namespace imp
{
    void setMidiPortEnabled(std::unique_ptr<RtMidiIn> const& midiIn, RtMidiIn::RtMidiCallback callback,
                            void* const userData, bool const enable);

    template <class P>
    inline bool tryToOpenPort(std::shared_ptr<P> const& port, int const portID)
    {
        Q_ASSERT( port != nullptr );
        Q_ASSERT( portID > -1 );

        static_assert( std::is_base_of<QAbstractMidiIn, P>::value || std::is_base_of<QAbstractMidiOut, P>::value, "P should be derived from QAbstractMidiIn or QAbstractMidiOut" );

        return port->isPortOpen() || port->openPort(portID);
    }

    template <class P>
    static void midiInCallback(double /*timestamp*/, std::vector<unsigned char>* bytes, void* userData)
    {
        if (bytes->empty())
        {
            return;
        }

        P* const midiIn = static_cast<P*>(userData);

        if (midiIn->isEnabled())
        {
            midiIn->broadcastMessage(QMidiMessage(*bytes, midiIn->portOpened()));
        }
    }
}

#endif // RTMIDIHELPERS_HPP
