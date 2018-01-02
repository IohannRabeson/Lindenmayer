//
// Created by Io on 30/12/2017.
//

#ifndef SYSEXVIEWER_QMIDIIN_HPP
#define SYSEXVIEWER_QMIDIIN_HPP
#include <QObject>

class QMidiMessage;
class QMidiInPrivate;

class QMidiIn : public QObject
{
    Q_DECLARE_PRIVATE(QMidiIn)
    Q_OBJECT
public:
    explicit QMidiIn(QObject* parent = nullptr);
    ~QMidiIn();

    bool openPort(int const portIndex) noexcept;
    void closePort() noexcept;
    int portCount() const noexcept;
    int portOpened() const noexcept;
    QString portName(int const index) const noexcept;
signals:
    void messageReceived(QMidiMessage const& message);
    void error(QString const& error);
public slots:
    void setEnabled(bool const enabled);
private:
    QScopedPointer<QMidiInPrivate> d_ptr;
};

#endif //SYSEXVIEWER_QMIDIIN_HPP
