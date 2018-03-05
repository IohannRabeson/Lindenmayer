//
// Created by Io on 25/01/2018.
//

#ifndef MIDIMONITOR_QMIDIINBASE_HPP
#define MIDIMONITOR_QMIDIINBASE_HPP

#include "QAbstractMidiIn.hpp"
#include <QString>

class QMidiInBase : public QAbstractMidiIn
{
public:
    explicit QMidiInBase(QString const& name = QString());

    bool openPort(int const portIndex) noexcept override;
    void closePort() noexcept override;
    int portOpened() const noexcept override;
    QString portName() const noexcept override;
    void setPortEnabled(bool const enabled) noexcept override;
    bool isPortEnabled() const noexcept override;

    void messageReceived(QMidiMessage const& message) override;
private:
    QString const m_name;
    int m_portIndex = -1;
    bool m_portEnabled = true;
};

#endif //MIDIMONITOR_QMIDIINBASE_HPP
