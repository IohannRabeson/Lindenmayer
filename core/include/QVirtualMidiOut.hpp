#ifndef QVIRTUALMIDIOUT_HPP
#define QVIRTUALMIDIOUT_HPP
#include <QAbstractMidiOut.hpp>
#include <QScopedPointer>

class QVirtualMidiOutPrivate;

class QVirtualMidiOut : public QAbstractMidiOut
{
    Q_DECLARE_PRIVATE(QVirtualMidiOut)
public:
    QVirtualMidiOut();
    ~QVirtualMidiOut();

    bool openPort(int const portIndex) override;
    void closePort() override;
    int portOpened() const override;
    QString portName() const override;
    void setPortEnabled(bool const enabled) override;
    bool isPortEnabled() const override;
    bool isPortRemovable() const override;
private:
    void outputMessage(QMidiMessage const& message) override;
private:
    QScopedPointer<QVirtualMidiOutPrivate> d_ptr;
};

#endif // QVIRTUALMIDIOUT_HPP
