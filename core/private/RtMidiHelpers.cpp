#include "RtMidiHelpers.hpp"
#include <QtGlobal>

namespace imp
{
    void setMidiPortEnabled(std::unique_ptr<RtMidiIn> const& midiIn, RtMidiIn::RtMidiCallback callback, void* const userData,
                            bool const enable)
    {
        Q_ASSERT( callback != nullptr );
        Q_ASSERT( userData != nullptr );

        if (enable)
        {
            midiIn->setCallback(callback, userData);
        }
        else
        {
            midiIn->cancelCallback();
        }
    }
}
