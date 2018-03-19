#ifndef MIDIMONITOR_QVIRTUALMIDIIN_HPP
#define MIDIMONITOR_QVIRTUALMIDIIN_HPP
#include <QAbstractMidiIn.hpp>

class QVirtualMidiInPrivate;

class QVirtualMidiIn : public QAbstractMidiIn
{
    Q_DECLARE_PRIVATE(QVirtualMidiIn)
public:
    QVirtualMidiIn();
    ~QVirtualMidiIn();

    bool openPort(int const portIndex) override;
    void closePort() override;
    int portOpened() const override;
    QString portName() const override;
    void setPortEnabled(const bool enabled) override;
    bool isPortEnabled() const override;
private:
    QScopedPointer<QVirtualMidiInPrivate> d_ptr;
};

#endif //MIDIMONITOR_QVIRTUALMIDIIN_HPP
