//
// Created by Io on 04/01/2018.
//

#ifndef MIDIMONITOR_QMIDIMANAGER_HPP
#define MIDIMONITOR_QMIDIMANAGER_HPP
#include <QObject>
#include <QVector>

class QMidiDeviceModel;
class QAbstractMidiIn;
class QAbstractMidiOut;
class QMidiMessage;
class QMidiMessageMatrixModel;

class QMidiManager : public QObject
{
    Q_OBJECT
public:
    QMidiManager(QObject* parent = nullptr);

    void rescanPorts();
    void rescanPorts(QMap<int, int>& inputRemapping, QMap<int, int>& outputRemapping);
    void addInputPort(QAbstractMidiIn* midiIn);
    void addOutputPort(QAbstractMidiOut* midiOut);
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
    void portsRescaned();
private:
    void resetMidiInPorts(QMap<int, int>& inputRemappings);
    void resetMidiOutPorts(QMap<int, int>& outputRemappings);
    void closeOutputPorts();
    void closeInputPorts();
    void resetPhysicalMidiInPorts();
    void resetPhysicalMidiOutPorts();
    void sendToOutputs(QMidiMessage const& message);
private:
    QMidiDeviceModel* const m_inputDeviceModel;
    QMidiDeviceModel* const m_outputDeviceModel;
    QMidiMessageMatrixModel* const m_matrixModel;

    // TODO: use unique_ptr and std::vector instead of QVector and raw pointers.
    QVector<QAbstractMidiIn*> m_physicalMidiIns;
    QVector<QAbstractMidiOut*> m_physicalMidiOuts;
};

#endif //MIDIMONITOR_QMIDIMANAGER_HPP
