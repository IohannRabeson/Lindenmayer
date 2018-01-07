//
// Created by Io on 31/12/2017.
//

#include "QDefaultMidiTranslator.hpp"
#include "QMidiChecksums.hpp"

#include <QObject>

QString QDefaultMidiTranslator::getTranslatorManufacturer() const
{
    return "Dead Devs Society";
}

QString QDefaultMidiTranslator::getTranslatorName() const
{
    return QObject::tr("Default");
}

QString QDefaultMidiTranslator::translateControlChangeName(unsigned char const control) const
{
    return QString("[CC%0]").arg(control);
}

QString QDefaultMidiTranslator::translateControlChangeValue(unsigned char const, unsigned char const value) const
{
    return QString::number(value);
}

QAbstractMidiTranslator::ChecksumFunction QDefaultMidiTranslator::checksumFunction() const
{
    return &QMidiChecksums::standardChecksum;
}

QString QDefaultMidiTranslator::getTranslatorVersion() const
{
    return "0.0.0";
}
