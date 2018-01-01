//
// Created by Io on 31/12/2017.
//

#include "QAbstractDeviceScheme.hpp"

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
    QString const formatedControlName = formatControlChangeName(control);
    QString const formatedValue = formatControlChangeValue(control, value);
    QString const format = formatControlChangeDataText();

    return format.arg(formatedControlName).arg(formatedValue);
}