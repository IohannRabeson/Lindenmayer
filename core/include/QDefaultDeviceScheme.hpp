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
    QString schemeVersion() const override;

    QString translateControlChangeName(unsigned char const control) const override;
    QString translateControlChangeValue(unsigned char const control, unsigned char const value) const override;

    ChecksumFunction checksumFunction() const override;
};

#endif //MIDIMONITOR_QDEFAULTDEVICESCHEME_HPP
