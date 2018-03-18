//
// Created by Io on 08/03/2018.
//

#ifndef MIDIMONITOR_VECTORHELPERS_HPP
#define MIDIMONITOR_VECTORHELPERS_HPP
#include <vector>
#include <algorithm>

namespace imp
{
    template <class T>
    inline int pushBack(std::vector<T>& vector, T const& value)
    {
        int const index = vector.size();

        vector.emplace_back(value);
        return index;
    }

    template <class T>
    inline int emplaceBack(std::vector<T>& vector, T&& value)
    {
        int const index = vector.size();

        vector.emplace_back(std::move(value));
        return index;
    }

    template <class T>
    inline void removeAt(std::vector<T>& vector, int const index)
    {
        if (index < vector.size())
        {
            vector.erase(cbegin(vector) + index);
        }
    }

    template <class T>
    inline void removeOne(std::vector<T>& vector, T const& value)
    {
        auto const it = std::find(vector.begin(), vector.end(), value);

        vector.erase(it);
    }

    template <class T>
    inline int uniquePushBack(std::vector<T>& vector, T const& value)
    {
        int index = -1;

        if (std::find(std::begin(vector), std::end(vector), value) == std::end(vector))
        {
            index = static_cast<int>(vector.size());
            vector.emplace_back(value);
        }
        return index;
    }

    template <class T, class ... A>
    inline int uniqueEmplace(std::vector<T>& vector, A&&... args)
    {
        int index = -1;
        T value(std::forward<A>(args)...);

        if (std::find(std::begin(vector), std::end(vector), value) == std::end(vector))
        {
            index = static_cast<int>(vector.size());
            vector.emplace_back(std::move(value));
        }
        return index;
    }
}

#endif //MIDIMONITOR_VECTORHELPERS_HPP
