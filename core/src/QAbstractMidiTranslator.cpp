//
// Created by Io on 31/12/2017.
//

#include "QAbstractMidiTranslator.hpp"
#include <QObject>

/*!
 * \brief
 * \param scheme
 * \param message
 * \return
 *
 * Reference: http://www.mamosa.org/jenfi.home/utilitaires/midichecksum.php
 */
bool QAbstractMidiTranslator::verifyChecksum(QMidiMessage const& message) const
{
    auto const checksum = computeChecksum(message);
    auto const expectedChecksum = message.getChecksum();

    return expectedChecksum == checksum;
}

unsigned char QAbstractMidiTranslator::computeChecksum(QMidiMessage const& message) const
{
    return checksumFunction()(message.bytes(), 6u, message.byteCount() - 6u - 2u);
}

QString QAbstractMidiTranslator::formatControlChangeData(unsigned char const control, unsigned char const value) const
{
    QString const formatedControlName = translateControlChangeName(control);
    QString const formatedValue = translateControlChangeValue(control, value);
    //: Format for control change display text
    QString const format = QObject::tr("%0: %1");

    return format.arg(formatedControlName).arg(formatedValue);
}
