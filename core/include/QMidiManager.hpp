//
// Created by Io on 04/01/2018.
//

#ifndef MIDIMONITOR_QMIDIMANAGER_HPP
#define MIDIMONITOR_QMIDIMANAGER_HPP
#include <QObject>
#include <QVector>

class QMidiDeviceModel;
class QMidiIn;
class QMidiOut;
class QMidiMessage;
class QMidiMessageMatrixModel;

class QMidiManager : public QObject
{
    Q_OBJECT
public:
    QMidiManager(QObject* parent = nullptr);

    void resetPorts();
    void resetPorts(QMap<int, int>& inputRemappings);
    void sendMessage(QMidiMessage const& message);

    QMidiDeviceModel* getInputDeviceModel() const;
    QMidiDeviceModel* getOutputDeviceModel() const;
    QMidiMessageMatrixModel* getMessageMatrixModel() const;

    void setInputPortEnabled(int const portId, bool const enabled);
    void setOutputPortEnabled(int const portId, bool const enabled);
    void closeAll();
signals:
    void messageReceived(QMidiMessage const& message);
    void messageSent(QMidiMessage const& message);
private:
    void resetMidiInPorts(QMap<int, int>& inputRemappings);
    void resetMidiOutPorts();
    void closeOutputPorts();
    void closeInputPorts();
private:
    QMidiDeviceModel* const m_inputDeviceModel;
    QMidiDeviceModel* const m_outputDeviceModel;
    QMidiMessageMatrixModel* const m_matrixModel;

    // TODO: use unique_ptr and std::vector instead of QVector and raw pointers.
    QVector<QMidiIn*> m_midiIns;
    QVector<QMidiOut*> m_midiOuts;
};

#endif //MIDIMONITOR_QMIDIMANAGER_HPP
