//
// Created by Io on 02/01/2018.
//

#ifndef MIDIMONITOR_FORMAT_HPP
#define MIDIMONITOR_FORMAT_HPP
#include <QString>
#include <QTextStream>
#include <QCoreApplication>

#include <vector>

class Format
{
    Q_DECLARE_TR_FUNCTIONS(Format)
public:
    template <class I>
    static QString formatBytes(I begin, I end, int const base = 16, QChar const separator = ' ')
    {
        QString buffer;
        QTextStream stream(&buffer);
        auto it = begin;

        stream.setIntegerBase(base);
        while (it != end)
        {
            if (it != begin)
            {
                stream << separator;
            }
            stream << QString::number(*it, base);
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

    static QString formatMidiNote(unsigned char octave, unsigned char const note);
    static QString formatMidiNote(unsigned char const note);
};

#endif //MIDIMONITOR_FORMAT_HPP