//
// Created by Io on 31/12/2017.
//

#ifndef SYSEXMONITOR_QDEFAULTMIDISCHEME_HPP
#define SYSEXMONITOR_QDEFAULTMIDISCHEME_HPP

#include <QAbstractMidiScheme.hpp>

class QDefaultMidiScheme : public QAbstractMidiScheme
{
public:
    QString schemeName() const override;
    QString controlChangeName(unsigned char const control) const override;
    QString formatControlValue(unsigned char const control, unsigned char const value) const override;
    QString formatControlChangeDataText() const override;;
    ChecksumFunction checksumFunction() const override;
};

#endif //SYSEXMONITOR_QDEFAULTMIDISCHEME_HPP
