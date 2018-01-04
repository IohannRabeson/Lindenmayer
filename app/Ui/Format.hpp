//
// Created by Io on 02/01/2018.
//

#ifndef MIDIMONITOR_FORMAT_HPP
#define MIDIMONITOR_FORMAT_HPP
#include <QString>
#include <QTextStream>
#include <vector>

class Format
{
public:
    template <class I>
    static QString formatBytes(I begin, I end, int const base = 16, QChar const separator = ' ')
    {
        QString buffer;
        QTextStream stream(&buffer);
        auto it = begin;

        stream.setIntegerBase(16);
        while (it != end)
        {
            if (it != begin)
            {
                stream << separator;
            }
            stream << QString::number(*it, 16);
            ++it;
        }
        stream.flush();
        return buffer;
    }

    template <typename C>
    static QString formatBytes(C const& container)
    {
        return formatBytes(std::begin(container), std::end(container));
    }
};


#endif //MIDIMONITOR_FORMAT_HPP
