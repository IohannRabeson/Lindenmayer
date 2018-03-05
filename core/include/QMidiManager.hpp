//
// Created by Io on 04/01/2018.
//

#ifndef MIDIMONITOR_QMIDIMANAGER_HPP
#define MIDIMONITOR_QMIDIMANAGER_HPP
#include <QObject>
#include <memory>

class QMidiDeviceModel;
class QMidiInListModel;
class QAbstractMidiIn;
class QAbstractMidiOut;
class QMidiMessage;
class QMidiMessageMatrixModel;
class QMidiMessageFilterModel;
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
     * \brief Rescan MIDI ports
     *
     * Each ports are destroyed, including ports added using addInputPort() and addOutputPort().
     *
     * \param inputRemapping Mapping between the old and the new index of each ports
     * \param outputRemapping Mapping between the old and the new index of each ports
     */
    void rescanPorts(QMap<int, int>& inputRemapping, QMap<int, int>& outputRemapping);

    /*!
     * \brief Add an input port
     * \param midiIn Input port to add
     * Before adding it, this function open the port if it not already opened.
     *
     * When the input port added will receives a message it will be forwarded through
     * the MIDI message matrix to the outputs ports.
     */
    int addInputPort(std::shared_ptr<QAbstractMidiIn>&& midiIn);

    /*!
     * \brief Add an output port
     * \param midiIn Output port to add
     * Before adding it, this function open the port if it not already opened.
     */
    int addOutputPort(std::unique_ptr<QAbstractMidiOut>&& midiOut);

    /*!
     * \brief Close and destroy each input and output ports.
     */
    void closeAllPorts();

    QMidiInListModel* getInputDeviceModel() const;
    QMidiDeviceModel* getOutputDeviceModel() const;
    QMidiMessageMatrixModel* getMessageMatrixModel() const;

    void setInputPortEnabled(int const portId, bool const enabled);
    void setOutputPortEnabled(int const portId, bool const enabled);
signals:
    void messageReceived(QMidiMessage const& message);
    void messageSent(QMidiMessage const& message);
    void portsRescanned();
private:
    QScopedPointer<QMidiManagerPrivate> d_ptr;
};

#endif //MIDIMONITOR_QMIDIMANAGER_HPP
