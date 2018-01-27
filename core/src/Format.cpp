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
        tr("C"),
        tr("C#"),
        tr("D"),
        tr("D#"),
        tr("E"),
        tr("F"),
        tr("F#"),
        tr("G"),
        tr("G#"),
        tr("A"),
        tr("A#"),
        tr("B"),
    };

    return NoteTexts.value(note % 12) % "-" % QString::number(note / 12);
}

QString Format::formatMidiNote(unsigned char octave, unsigned char const note)
{
    return formatMidiNote(octave * 12u + note);
}
