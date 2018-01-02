//
// Created by Io on 01/01/2018.
//

#ifndef MIDIMONITOR_CONTROLCHANGENAMETRANSLATOR_HPP
#define MIDIMONITOR_CONTROLCHANGENAMETRANSLATOR_HPP
#include <QMap>
#include <QString>

class TextDatabase
{
public:
    /*!
     * \brief Load a database from a JSON file
     * \param jsonFilePath
     * \return
     *
     * The json should respect this pattern:
     * Remarks the copy directive: [copy 34]. This means takes translations values assigned to CC34 and
     * copy them to CC27.
     * \verbatim
     * {
     *   "control_change_names": {
     *     "34": "Osc1 shape",
     *   },
     *   "control_change_values": {
     *     "34": {
     *       "0": "PWM",
     *       "1": "SAW",
     *       "2": "TRI",
     *       "3": "APW",
     *       "4": "UNISON-M",
     *       "5": "UNISON-P",
     *       "6": "UNIAPW-M",
     *       "7": "UNIAPW-P",
     *       "8": "PARA-8",
     *       "9": "PARA-4"
     *     },
     *     "27": "[copy 34]",
     *   }
     * }
     * \endverbatim
     */
    bool load(QString const jsonFilePath);

    QString translateControlChangeNumber(unsigned char const controlNumber) const;
    QString translateControlChangeValue(unsigned char const controlNumber, unsigned char const controlValue) const;
private:
    QMap<unsigned char, QString> m_controlChangeNumberTranslations;
    QMap<unsigned char, QMap<unsigned char, QString>> m_controlChangeValueTranslations;
};

#endif //MIDIMONITOR_CONTROLCHANGENAMETRANSLATOR_HPP
