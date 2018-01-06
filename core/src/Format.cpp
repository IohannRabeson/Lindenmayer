//
// Created by Io on 02/01/2018.
//

#include "Format.hpp"
#include <QVector>

#include <QStringBuilder>

QString Format::formatMidiNote(unsigned char const note)
{
    static QVector<QString> const NoteTexts =
    {
        QObject::tr("C"),
        QObject::tr("C#"),
        QObject::tr("D"),
        QObject::tr("D#"),
        QObject::tr("E"),
        QObject::tr("F"),
        QObject::tr("F#"),
        QObject::tr("G"),
        QObject::tr("G#"),
        QObject::tr("A"),
        QObject::tr("A#"),
        QObject::tr("B"),
    };

    return NoteTexts.value(note % 12) % QString::number(note / 12);
}
