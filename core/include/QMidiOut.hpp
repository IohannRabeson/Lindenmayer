//
// Created by Io on 06/01/2018.
//

#ifndef MIDIMONITOR_QMIDIOUT_HPP
#define MIDIMONITOR_QMIDIOUT_HPP
#include <QObject>

class QMidiMessage;
class QMidiOutPrivate;

class QMidiOut : public QObject
{
    Q_DECLARE_PRIVATE(QMidiOut)
    Q_OBJECT
public:
    explicit QMidiOut(QObject* parent = nullptr);
    ~QMidiOut();

    bool openPort(int const portIndex) noexcept;
    void closePort() noexcept;
    int portCount() const noexcept;
    int portOpened() const noexcept;
    QString portName(int const index) const noexcept;
public slots:
    void setEnabled(bool const enabled);
    void sendMessage(QMidiMessage const& message);
signals:
    void messageSended(QMidiMessage const& message);
    void error(QString const& error);
private:
    QScopedPointer<QMidiOutPrivate> d_ptr;
};

#endif //MIDIMONITOR_QMIDIOUT_HPP
