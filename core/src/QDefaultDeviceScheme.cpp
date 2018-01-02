//
// Created by Io on 31/12/2017.
//

#include "QDefaultDeviceScheme.hpp"
#include "QMidiChecksums.hpp"

#include <QObject>

QString QDefaultDeviceScheme::schemeManufacturer() const
{
    return "Dead Devs Society";
}

QString QDefaultDeviceScheme::schemeName() const
{
    return QObject::tr("Default");
}

QString QDefaultDeviceScheme::translateControlChangeName(unsigned char const control) const
{
    return QString("[CC%0]").arg(control);
}

QString QDefaultDeviceScheme::translateControlChangeValue(unsigned char const, unsigned char const value) const
{
    return QString::number(value);
}

QAbstractDeviceScheme::ChecksumFunction QDefaultDeviceScheme::checksumFunction() const
{
    return &QMidiChecksums::standardChecksum;
}

QString QDefaultDeviceScheme::schemeVersion() const
{
    return "0.0.0";
}
