//
// Created by Io on 31/12/2017.
//

#ifndef MIDIMONITOR_QDEFAULTTRANSLATOR_HPP
#define MIDIMONITOR_QDEFAULTTRANSLATOR_HPP

#include <QAbstractMidiTranslator.hpp>

class QDefaultMidiTranslator : public QAbstractMidiTranslator
{
public:
    QString getTranslatorManufacturer() const override;
    QString getTranslatorName() const override;
    QString getTranslatorVersion() const override;

    QString translateControlChangeName(unsigned char const control) const override;
    QString translateControlChangeValue(unsigned char const control, unsigned char const value) const override;

    ChecksumFunction checksumFunction() const override;
};

#endif //MIDIMONITOR_QDEFAULTTRANSLATOR_HPP
