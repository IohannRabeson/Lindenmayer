//
// Created by Io on 01/01/2018.
//

#ifndef MIDIMONITOR_PULSE2SCHEME_HPP
#define MIDIMONITOR_PULSE2SCHEME_HPP
#include <QDefaultMidiTranslator.hpp>
#include "Plugins/Commons/TextDatabase.hpp"

class Pulse2Translator : public QDefaultMidiTranslator
{
public:
    Pulse2Translator();

    QString getTranslatorManufacturer() const override;
    QString getTranslatorName() const override;
    QString getTranslatorVersion() const override;

    QString translateControlChangeName(unsigned char const control) const override;
    QString translateControlChangeValue(unsigned char const control, unsigned char const value) const override;
private:
    ChecksumFunction checksumFunction() const override;
private:
    TextDatabase m_translator;
};


#endif //MIDIMONITOR_PULSE2SCHEME_HPP
