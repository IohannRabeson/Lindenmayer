//
// Created by Io on 04/01/2018.
//

#ifndef MIDIMONITOR_QMIDIMANAGER_HPP
#define MIDIMONITOR_QMIDIMANAGER_HPP
#include <QObject>
#include <memory>

class QMidiDeviceModel;
class QAbstractMidiIn;
class QAbstractMidiOut;
class QMidiMessage;
class QMidiMessageMatrixModel;
class QMidiManagerPrivate;

class QMidiManager : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QMidiManager)
public:
    explicit QMidiManager(QObject* parent = nullptr);
    ~QMidiManager();

    void rescanPorts();
    void rescanPorts(QMap<int, int>& inputRemapping, QMap<int, int>& outputRemapping);
    void addInputPort(std::unique_ptr<QAbstractMidiIn>&& midiIn);
    void addOutputPort(std::unique_ptr<QAbstractMidiOut>&& midiOut);
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
    QScopedPointer<QMidiManagerPrivate> d_ptr;
};

#endif //MIDIMONITOR_QMIDIMANAGER_HPP
