//
// Created by Io on 31/12/2017.
//

#include "QAbstractDeviceScheme.hpp"
#include <QObject>

/*!
 * \brief
 * \param scheme
 * \param message
 * \return
 *
 * Reference: http://www.mamosa.org/jenfi.home/utilitaires/midichecksum.php
 */
bool QAbstractDeviceScheme::verifyChecksum(QMidiMessage const& message) const
{
    auto const checksum = computeChecksum(message);
    auto const expectedChecksum = message.getChecksum();

    return expectedChecksum == checksum;
}

unsigned char QAbstractDeviceScheme::computeChecksum(QMidiMessage const& message) const
{
    return checksumFunction()(message.bytes(), 6u, message.byteCount() - 6u - 2u);
}

QString QAbstractDeviceScheme::formatControlChangeData(unsigned char const control, unsigned char const value) const
{
    QString const formatedControlName = translateControlChangeName(control);
    QString const formatedValue = translateControlChangeValue(control, value);
    QString const format = QObject::tr("%0: %1");

    return format.arg(formatedControlName).arg(formatedValue);
}
