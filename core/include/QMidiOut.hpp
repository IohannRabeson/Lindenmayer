//
// Created by Io on 06/01/2018.
//

#ifndef MIDIMONITOR_QMIDIOUT_HPP
#define MIDIMONITOR_QMIDIOUT_HPP
#include <QObject>

class QMidiMessage;
class QMidiOutPrivate;

class QAbstractMidiOut : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;

    virtual bool openPort(int const portIndex) noexcept = 0;
    virtual void closePort() noexcept = 0;
    virtual int portOpened() const noexcept = 0;
    virtual QString portName(int const index) const noexcept = 0;
    virtual void setEnabled(bool const enabled) = 0;
    virtual void sendMessage(QMidiMessage const& message) = 0;
signals:
    void messageSended(QMidiMessage const& message);
    void error(QString const& error);
};

class QMidiOut : public QAbstractMidiOut
{
    Q_DECLARE_PRIVATE(QMidiOut)
    Q_OBJECT
public:
    explicit QMidiOut(QObject* parent = nullptr);
    ~QMidiOut();

    bool openPort(int const portIndex) noexcept override;
    void closePort() noexcept override;
    int portOpened() const noexcept override;
    QString portName(int const index) const noexcept override;
    void setEnabled(bool const enabled) override;
    void sendMessage(QMidiMessage const& message) override;

    int portCount() const noexcept;
signals:
    void messageSended(QMidiMessage const& message);
    void error(QString const& error);
private:
    QScopedPointer<QMidiOutPrivate> d_ptr;
};

#endif //MIDIMONITOR_QMIDIOUT_HPP
