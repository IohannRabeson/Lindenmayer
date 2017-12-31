//
// Created by Io on 31/12/2017.
//

#include "QAbstractMidiScheme.hpp"

/*!
 * \brief
 * \param scheme
 * \param message
 * \return
 *
 * Reference: http://www.mamosa.org/jenfi.home/utilitaires/midichecksum.php
 */
bool QAbstractMidiScheme::verifyChecksum(QMidiMessage const& message) const
{
    auto const checksum = computeChecksum(message);
    auto const expectedChecksum = message.getChecksum();

    return expectedChecksum == checksum;
}

unsigned char QAbstractMidiScheme::computeChecksum(QMidiMessage const& message) const
{
    return checksumFunction()(message.bytes(), 6u, message.byteCount() - 6u - 2u);
}
