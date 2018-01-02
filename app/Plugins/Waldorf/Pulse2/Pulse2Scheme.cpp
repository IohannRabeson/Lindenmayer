//
// Created by Io on 01/01/2018.
//

#include "Pulse2Scheme.hpp"
#include "QMidiChecksums.hpp"

Pulse2Scheme::Pulse2Scheme()
{
    m_translator.load(":/Waldorf/Pulse2/Waldorf/Pulse2/TextDatabase.json");
}

QString Pulse2Scheme::schemeManufacturer() const
{
    return "Dead Devs Society";
}

QString Pulse2Scheme::schemeName() const
{
    return "Waldorf Pulse 2";
}

QString Pulse2Scheme::schemeVersion() const
{
    return "0.0.0";
}

QString Pulse2Scheme::translateControlChangeName(unsigned char const control) const
{
    return m_translator.translateControlChangeNumber(control);
}

QString Pulse2Scheme::translateControlChangeValue(unsigned char const control, unsigned char const value) const
{
    return m_translator.translateControlChangeValue(control, value);
}

QAbstractDeviceScheme::ChecksumFunction Pulse2Scheme::checksumFunction() const
{
    return &QMidiChecksums::standardChecksum;
}

