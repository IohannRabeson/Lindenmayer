#ifndef MIDIMONITOR_QVIRTUALMIDIIN_HPP
#define MIDIMONITOR_QVIRTUALMIDIIN_HPP
#include <QAbstractMidiIn.hpp>

class QVirtualMidiIn : public QAbstractMidiIn
{
public:
    bool openPort(int const portIndex) override;
    void closePort() override;
    int portOpened() const override;
    QString portName() const override;
    void setPortEnabled(const bool enabled) override;
    bool isPortEnabled() const override;
private:
    int m_portOpened = -1;
    bool m_portEnabled = true;
    QString m_portName;
};

#endif //MIDIMONITOR_QVIRTUALMIDIIN_HPP
