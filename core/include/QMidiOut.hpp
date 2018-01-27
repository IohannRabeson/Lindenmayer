//
// Created by Io on 06/01/2018.
//

#ifndef MIDIMONITOR_QMIDIOUT_HPP
#define MIDIMONITOR_QMIDIOUT_HPP

#include "QAbstractMidiOut.hpp"
#include <QScopedPointer>

class QMidiMessage;
class QMidiOutPrivate;

class QMidiOut : public QAbstractMidiOut
{
    Q_DECLARE_PRIVATE(QMidiOut)
public:
    explicit QMidiOut();
    ~QMidiOut();

    bool openPort(int const portIndex) noexcept override;
    void closePort() noexcept override;
    int portOpened() const noexcept override;
    QString portName() const noexcept override;
    void setEnabled(bool const enabled) override;
    void sendMessage(QMidiMessage const& message) override;
    int portCount() const noexcept override;
private:
    QScopedPointer<QMidiOutPrivate> d_ptr;
};

#endif //MIDIMONITOR_QMIDIOUT_HPP
