//
// Created by Io on 01/01/2018.
//

#ifndef MIDIMONITOR_PULSE2SCHEME_HPP
#define MIDIMONITOR_PULSE2SCHEME_HPP
#include <QDefaultDeviceScheme.hpp>
#include "Plugins/Commons/TextDatabase.hpp"

class Pulse2Scheme : public QDefaultDeviceScheme
{
public:
    Pulse2Scheme();

    QString schemeManufacturer() const override;
    QString schemeName() const override;
    QString schemeVersion() const override;
    QString translateControlChangeName(unsigned char const control) const override;
    QString translateControlChangeValue(unsigned char const control, unsigned char const value) const override;
private:
    ChecksumFunction checksumFunction() const override;
private:
    TextDatabase m_translator;
};


#endif //MIDIMONITOR_PULSE2SCHEME_HPP
