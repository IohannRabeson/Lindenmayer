//
// Created by Io on 31/12/2017.
//

#ifndef SYSEXMONITOR_QDEFAULTMIDISCHEME_HPP
#define SYSEXMONITOR_QDEFAULTMIDISCHEME_HPP

#include <QAbstractMidiScheme.hpp>

class QDefaultMidiScheme : public QAbstractMidiScheme
{
public:
    QString schemeManufacturer() const override;
    QString schemeName() const override;
    QString formatControlChangeName(unsigned char const control) const override;
    QString formatControlChangeValue(unsigned char const control, unsigned char const value) const override;
    QString formatControlChangeDataText() const override;
    ChecksumFunction checksumFunction() const override;
};

#endif //SYSEXMONITOR_QDEFAULTMIDISCHEME_HPP
