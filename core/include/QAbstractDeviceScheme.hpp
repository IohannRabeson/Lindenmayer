//
// Created by Io on 31/12/2017.
//

#ifndef MIDIMONITOR_QABSTRACTDEVICESCHEME_HPP
#define MIDIMONITOR_QABSTRACTDEVICESCHEME_HPP
#include <QMidiMessage.hpp>

/*!
 * \brief Define device specifics things
 *
 * They are one midi scheme by specific device.
 */
// TODO: virtual method returning dump request format and dump data format
// TODO: Add graphics property such as device rectangle, background texture, buttons positions, etc...
class QAbstractDeviceScheme
{
public:
    using ChecksumFunction = unsigned char (*)(QMidiMessage::Bytes const&, std::size_t const, std::size_t const);

    virtual ~QAbstractDeviceScheme() = default;

    /*!
     * \brief Get the scheme manufacturer name
     */
    virtual QString schemeManufacturer() const = 0;

    /*!
     * \brief Get the scheme name
     */
    virtual QString schemeName() const = 0;

    /*!
     * \brief Get the scheme version
     */
    virtual QString schemeVersion() const = 0;

    /*!
     * \brief Compute checksum of \e bytes
     * \param bytes Data used to compute the checksum. Not all bytes are used. Only from byte 6 to end - 2 (-2 because
     * the last byte should be always F7, it is used to mark the end. The byte before store the checksum.
     * \param count
     * \return
     */
    unsigned char computeChecksum(QMidiMessage const& message) const;

    /*!
     * \brief Helper to checking checksum integrity of a message.
     * \param message
     * \return
     */
    bool verifyChecksum(QMidiMessage const& message) const;

    /*!
     * \brief Format text displayed by column "Data"
     * \param control Control change number
     * \param value Control change value
     * \return Text with the control name and the new control value.
     *
     * By default the pattern "%0: %1" is used where %0 is the control text returned by translateControlChangeName
     * and %1 is the text returned translateControlChangeValue.
     */
    virtual QString formatControlChangeData(unsigned char const control, unsigned char const value) const;
protected:
    /*!
     * \brief Return name for a control
     * \param control Control identifier
     */
    virtual QString translateControlChangeName(unsigned char const control) const = 0;

    /*!
     * \brief Return displayable value for a control
     * \param control Control identifier
     * \param value Value to format
     */
    virtual QString translateControlChangeValue(unsigned char const control, unsigned char const value) const = 0;

    /*!
     * \brief Return the checksum function.
     */
    virtual ChecksumFunction checksumFunction() const = 0;
};

#endif //MIDIMONITOR_QABSTRACTDEVICESCHEME_HPP
