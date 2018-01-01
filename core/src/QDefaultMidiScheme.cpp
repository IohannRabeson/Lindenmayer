//
// Created by Io on 31/12/2017.
//

#include "QDefaultMidiScheme.hpp"
#include <QObject>

QString QDefaultMidiScheme::schemeManufacturer() const
{
    return "Dead Devs Society";
}

QString QDefaultMidiScheme::schemeName() const
{
    return QObject::tr("Default");
}

QString QDefaultMidiScheme::formatControlChangeName(unsigned char const control) const
{
    return QString::number(control);
}

QString QDefaultMidiScheme::formatControlChangeValue(unsigned char const, unsigned char const value) const
{
    return QString::number(value);
}

QAbstractMidiScheme::ChecksumFunction QDefaultMidiScheme::checksumFunction() const
{
    return [](QMidiMessage::Bytes const& bytes, std::size_t const start, std::size_t const size) -> unsigned char
    {
        return std::accumulate(bytes.begin() + start, bytes.begin() + size + start, 0u);
    };
}

QString QDefaultMidiScheme::formatControlChangeDataText() const
{
    return QObject::tr("[CC] %0: %1");
}
