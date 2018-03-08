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

    bool openPort(int const portIndex) override;
    void closePort() override;
    int portOpened() const override;
    QString portName() const override;
    void setPortEnabled(bool const enabled) override;
    bool isPortEnabled() const override;

    int portCount() const;
private:
    void outputMessage(QMidiMessage const& message) override;
private:
    QScopedPointer<QMidiOutPrivate> d_ptr;
};

#endif //MIDIMONITOR_QMIDIOUT_HPP
