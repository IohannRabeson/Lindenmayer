//
// Created by Io on 31/12/2017.
//

#ifndef SYSEXMONITOR_QABSTRACTMIDISCHEME_HPP
#define SYSEXMONITOR_QABSTRACTMIDISCHEME_HPP
#include <QMidiMessage.hpp>

/*!
 * \brief Define device specifics things
 *
 * They are one midi scheme by specific device.
 */
// TODO: virtual method returning dump request format and dump data format
// TODO: Add graphics property such as device rectangle, background texture, buttons positions, etc...
class QAbstractMidiScheme
{
public:
    using ChecksumFunction = unsigned char (*)(QMidiMessage::Bytes const&, std::size_t const, std::size_t const);

    virtual ~QAbstractMidiScheme() = default;

    /*!
     * \brief Get the scheme manufacturer name
     */
    virtual QString schemeManufacturer() const = 0;

    /*!
     * \brief Get the scheme name
     */
    virtual QString schemeName() const = 0;

    /*!
     * \brief Compute checksum of \e bytes
     * \param bytes Data used to compute the checksum. Not all bytes are used. Only from byte 6 to end - 2 (-2 because
     * the last byte should be always F7, it is used to mark the end. The byte before store the checksum.
     * \param count
     * \return
     */
    virtual unsigned char computeChecksum(QMidiMessage const& message) const;

    /*!
     * \brief Return name for a control
     * \param control Control identifier
     */
    virtual QString formatControlChangeName(unsigned char const control) const = 0;

    /*!
     * \brief Return displayable value for a control
     * \param control Control identifier
     * \param value Value to format
     */
    virtual QString formatControlChangeValue(unsigned char const control, unsigned char const value) const = 0;

    /*!
     * \brief Return the format to display control change value
     * \return A string with placeholders %0 for control name and %1 for value.
     */
    virtual QString formatControlChangeDataText() const = 0;

    /*!
     * \brief Helper to checking checksum integrity of a message.
     * \param message
     * \return
     */
    bool verifyChecksum(QMidiMessage const& message) const;
private:
    virtual ChecksumFunction checksumFunction() const = 0;
};

#endif //SYSEXMONITOR_QABSTRACTMIDISCHEME_HPP
