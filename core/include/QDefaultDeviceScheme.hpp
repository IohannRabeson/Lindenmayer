//
// Created by Io on 31/12/2017.
//

#ifndef MIDIMONITOR_QDEFAULTDEVICESCHEME_HPP
#define MIDIMONITOR_QDEFAULTDEVICESCHEME_HPP

#include <QAbstractDeviceScheme.hpp>

class QDefaultDeviceScheme : public QAbstractDeviceScheme
{
public:
    QString schemeManufacturer() const override;
    QString schemeName() const override;
    QString formatControlChangeName(unsigned char const control) const override;
    QString formatControlChangeValue(unsigned char const control, unsigned char const value) const override;
    QString formatControlChangeDataText() const override;
    ChecksumFunction checksumFunction() const override;
};

#endif //MIDIMONITOR_QDEFAULTDEVICESCHEME_HPP
