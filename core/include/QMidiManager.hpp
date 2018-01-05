//
// Created by Io on 04/01/2018.
//

#ifndef MIDIMONITOR_QMIDIMANAGER_HPP
#define MIDIMONITOR_QMIDIMANAGER_HPP
#include <QObject>
#include <QVector>

class QMidiDeviceModel;
class QMidiIn;
class QMidiMessage;

class QMidiManager : public QObject
{
    Q_OBJECT
public:
    QMidiManager(QObject* parent = nullptr);

    QMidiDeviceModel* getInputDeviceModel() const;

    void setInputPortEnabled(int const portId, bool const enabled);
    void resetMidiInPorts();
    void closeAll();
signals:
    void messageReceived(QMidiMessage const& message);
private:
    QMidiDeviceModel* const m_inputDeviceModel;
    QVector<QMidiIn*> m_midiIns;
};


#endif //MIDIMONITOR_QMIDIMANAGER_HPP
