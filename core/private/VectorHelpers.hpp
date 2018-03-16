//
// Created by Io on 08/03/2018.
//

#ifndef MIDIMONITOR_VECTORHELPERS_HPP
#define MIDIMONITOR_VECTORHELPERS_HPP
#include <vector>

namespace imp
{
    template <class T>
    inline int addToVector(std::vector<T>& vector, T const& value)
    {
        int const index = vector.size();

        vector.emplace_back(value);
        return index;
    }

    template <class T>
    inline int addToVector(std::vector<T>& vector, T&& value)
    {
        int const index = vector.size();

        vector.emplace_back(std::move(value));
        return index;
    }

    template <class T>
    inline void removeFromVector(std::vector<T>& vector, int const index)
    {
        if (index < vector.size())
        {
            vector.erase(cbegin(vector) + index);
        }
    }
}

#endif //MIDIMONITOR_VECTORHELPERS_HPP
