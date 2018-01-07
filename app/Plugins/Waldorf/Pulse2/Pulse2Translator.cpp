//
// Created by Io on 01/01/2018.
//

#include "Pulse2Translator.hpp"
#include "QMidiChecksums.hpp"

Pulse2Translator::Pulse2Translator()
{
    m_translator.load(":/Waldorf/Pulse2/Waldorf/Pulse2/TextDatabase.json");
}

QString Pulse2Translator::getTranslatorManufacturer() const
{
    return "Dead Devs Society";
}

QString Pulse2Translator::getTranslatorName() const
{
    return "Waldorf Pulse 2";
}

QString Pulse2Translator::getTranslatorVersion() const
{
    return "0.0.0";
}

QString Pulse2Translator::translateControlChangeName(unsigned char const control) const
{
    return m_translator.translateControlChangeNumber(control);
}

QString Pulse2Translator::translateControlChangeValue(unsigned char const control, unsigned char const value) const
{
    return m_translator.translateControlChangeValue(control, value);
}

QAbstractMidiTranslator::ChecksumFunction Pulse2Translator::checksumFunction() const
{
    return &QMidiChecksums::standardChecksum;
}

