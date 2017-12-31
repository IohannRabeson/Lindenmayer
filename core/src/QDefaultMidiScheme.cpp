//
// Created by Io on 31/12/2017.
//

#include "QDefaultMidiScheme.hpp"

QString QDefaultMidiScheme::schemeName() const
{
    return "Default";
}

QString QDefaultMidiScheme::controlChangeName(unsigned char const control) const
{
    return QString::number(control);
}

QString QDefaultMidiScheme::formatControlValue(unsigned char const, unsigned char const value) const
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
    return "[CC] %0: %1";
}
