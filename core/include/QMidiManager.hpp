//
// Created by Io on 04/01/2018.
//

#ifndef MIDIMONITOR_QMIDIMANAGER_HPP
#define MIDIMONITOR_QMIDIMANAGER_HPP
#include <QObject>
#include <memory>

class QMidiDeviceModel;
class QMidiPortModel;
class QAbstractMidiIn;
class QAbstractMidiOut;
class QMidiMessage;
class QMidiMessageMatrixModel;
class QMidiMessageFilterFactory;
class QMidiManufacturerModel;
class QMidiManagerPrivate;

class QMidiManager : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QMidiManager)
public:
    explicit QMidiManager(QObject* parent = nullptr);
    ~QMidiManager();

    /*!
     * \brief Rescan MIDI ports
     *
     * Each ports are destroyed, including ports added using addInputPort() and addOutputPort().
     */
    void rescanPorts();

    /*!
     * \brief Close and destroy each input and output ports.
     */
    void closeAllPorts();

    QMidiPortModel* getInputDeviceModel() const;
    QMidiPortModel* getOutputDeviceModel() const;
    QMidiMessageMatrixModel* getMessageMatrixModel() const;
    QMidiMessageFilterFactory* getMessageFilterFactory() const;
    QMidiManufacturerModel* getManufacturerModel() const;
signals:
    void messageReceived(QMidiMessage const& message);
    void messageSent(QMidiMessage const& message);
    void portsRescanned();
private:
    QScopedPointer<QMidiManagerPrivate> d_ptr;
};

#endif //MIDIMONITOR_QMIDIMANAGER_HPP
