//
// Created by Io on 19/01/2018.
//

#ifndef MIDIMONITOR_LISTENERHELPERS_HPP
#define MIDIMONITOR_LISTENERHELPERS_HPP

template <class ... A, class C>
static inline void callEachListener(C const& listeners, A&& ... args)
{
    for (auto const& listener : listeners)
    {
        listener(std::forward<A>(args)...);
    }
};

#endif //MIDIMONITOR_LISTENERHELPERS_HPP
