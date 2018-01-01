//
// Created by Io on 31/12/2017.
//

#include "QDefaultDeviceScheme.hpp"
#include <QObject>

QString QDefaultDeviceScheme::schemeManufacturer() const
{
    return "Dead Devs Society";
}

QString QDefaultDeviceScheme::schemeName() const
{
    return QObject::tr("Default");
}

QString QDefaultDeviceScheme::formatControlChangeName(unsigned char const control) const
{
    return QString::number(control);
}

QString QDefaultDeviceScheme::formatControlChangeValue(unsigned char const, unsigned char const value) const
{
    return QString::number(value);
}

QAbstractDeviceScheme::ChecksumFunction QDefaultDeviceScheme::checksumFunction() const
{
    return [](QMidiMessage::Bytes const& bytes, std::size_t const start, std::size_t const size) -> unsigned char
    {
        return std::accumulate(bytes.begin() + start, bytes.begin() + size + start, 0u);
    };
}

QString QDefaultDeviceScheme::formatControlChangeDataText() const
{
    return QObject::tr("[CC] %0: %1");
}
