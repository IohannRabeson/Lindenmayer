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
    QMidiIn();
    ~QMidiIn();

    bool openPort(int portIndex) noexcept;
    void closePort() noexcept;
signals:
    void messageReceived(QMidiMessage const& message);
    void error(QString const& error);
private:
    QScopedPointer<QMidiInPrivate> d_ptr;
};

#endif //SYSEXVIEWER_QMIDIIN_HPP
