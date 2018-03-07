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

    bool openPort(int const portIndex) override;
    void closePort() override;
    int portOpened() const override;
    QString portName() const override;
    void setPortEnabled(bool const enabled) override;
    bool isPortEnabled() const override;

    void messageReceived(QMidiMessage const& message) override;
private:
    QString const m_name;
    int m_portIndex = -1;
    bool m_portEnabled = true;
};

#endif //MIDIMONITOR_QMIDIINBASE_HPP
