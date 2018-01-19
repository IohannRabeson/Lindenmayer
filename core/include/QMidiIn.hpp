//
// Created by Io on 30/12/2017.
//

#ifndef SYSEXVIEWER_QMIDIIN_HPP
#define SYSEXVIEWER_QMIDIIN_HPP
#include <QObject>

class QMidiMessage;
class QMidiInPrivate;

class QAbstractMidiIn : public QObject
{
    Q_DECLARE_PRIVATE(QMidiIn)
    Q_OBJECT
public:
    using QObject::QObject;

    virtual bool openPort(int const portIndex) noexcept = 0;
    virtual void closePort() noexcept = 0;
    virtual int portOpened() const noexcept = 0;
    virtual QString portName(int const index) const noexcept = 0;
    virtual void setPortEnabled(bool const enabled) noexcept = 0;
signals:
    void error(QString const& error);
    void messageReceived(QMidiMessage const& message);
};

// TODO: class VirtualMidiIn : public QAbstractMidiIn{};
// TODO: rename QMidiIn to QPhysicalMidiIn
class QMidiIn : public QAbstractMidiIn
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QMidiIn)
public:
    explicit QMidiIn(QObject* parent = nullptr);
    ~QMidiIn();

    bool openPort(int const portIndex) noexcept override;
    void closePort() noexcept override;
    int portOpened() const noexcept override;
    QString portName(int const index) const noexcept override;
    void setPortEnabled(bool const enabled) noexcept override;

    int portCount() const noexcept;
private:
    QScopedPointer<QMidiInPrivate> d_ptr;
};

#endif //SYSEXVIEWER_QMIDIIN_HPP
