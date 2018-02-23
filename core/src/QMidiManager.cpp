//
// Created by Io on 04/01/2018.
//

#include "QMidiManager.hpp"
#include "QMidiDeviceModel.hpp"
#include "QMidiIn.hpp"
#include "QMidiOut.hpp"
#include "QMidiMessageMatrixModel.hpp"
#include "QMidiMessageFilterModel.hpp"
#include "QMidiMessage.hpp"
#include "QMidiIn.hpp"
#include "QMidiOut.hpp"

#include <QtDebug>
#include <QSize>

#include <vector>

class QMidiManagerPrivate
{
    Q_DECLARE_PUBLIC(QMidiManager)
public:
    explicit QMidiManagerPrivate(QMidiManager* q)
    : q_ptr(q)
    , m_inputDeviceModel(new QMidiDeviceModel(q))
    , m_outputDeviceModel(new QMidiDeviceModel(q))
    , m_matrixModel(new QMidiMessageMatrixModel(q))
    {
    }

    void resetMidiInPorts(QMap<int, int>& inputRemappings);
    void resetMidiOutPorts(QMap<int, int>& outputRemappings);
    void closeOutputPorts();
    void closeInputPorts();
    void resetPhysicalMidiInPorts();
    void resetPhysicalMidiOutPorts();
    void forwardMidiMessage(QMidiMessage const& message);
private:
    QMidiManager* const q_ptr;
    QMidiDeviceModel* const m_inputDeviceModel;
    QMidiDeviceModel* const m_outputDeviceModel;
    QMidiMessageMatrixModel* const m_matrixModel;
    std::vector<std::unique_ptr<QAbstractMidiIn>> m_midiIns;
    std::vector<std::unique_ptr<QAbstractMidiOut>> m_midiOuts;
};

void QMidiManagerPrivate::resetPhysicalMidiInPorts()
{
    // We need one instance of QMidiOut in order to get the port count.
    // This limitation is discused here: https://github.com/thestk/rtmidi/issues/50
    auto firstMidiPort = std::make_unique<QMidiIn>();
    auto const portCount = firstMidiPort->portCount();

    if (portCount > 0u)
    {
        m_midiIns.emplace_back(std::move(firstMidiPort));

        // Instanciate midi other inputs
        for (int i = 0; i < portCount - 1; ++i)
        {
            m_midiIns.emplace_back(std::make_unique<QMidiIn>());
        }
        for (int i = 0; i < m_midiIns.size(); ++i)
        {
            m_midiIns[i]->openPort(i);
        }
    }
    m_inputDeviceModel->reset(m_midiIns);
}

void QMidiManagerPrivate::resetMidiInPorts(QMap<int, int>& inputRemappings)
{
    Q_Q(QMidiManager);

    // Store for each port a pair name/index.
    // We use the name after ports reset to try to found previous inputs port
    // and replaces the old indexes by the new ones.
    QMap<QString, int> oldPortNameIndexes;

    for (int i = 0; i < m_inputDeviceModel->rowCount(); ++i)
    {
        oldPortNameIndexes.insert(m_inputDeviceModel->name(i), i);
    }

    closeInputPorts();
    resetPhysicalMidiInPorts();

    if (m_midiIns.empty())
    {
        qWarning() << "[MidiMonitor]: No midi inputs";
    }
    else
    {
        // Open midi inputs
        for (int i = 0; i < m_midiIns.size(); ++i)
        {
            auto const& midiIn = m_midiIns[i];

            midiIn->addMessageReceivedListener([this, q](QMidiMessage const& message)
                                               {
                                                   emit q->messageReceived(message);
                                                   forwardMidiMessage(message);
                                               });
        }
    }

    // Replace the old inputs ports indexes by the new ones if possible.
    for (int i = 0; i < m_inputDeviceModel->rowCount(); ++i)
    {
        auto const name = m_inputDeviceModel->name(i);
        auto const oldIndex = oldPortNameIndexes.value(name);

        inputRemappings.insert(oldIndex, i);
    }
}

void QMidiManagerPrivate::resetMidiOutPorts(QMap<int, int>& outputRemappings)
{
    // Store for each port a pair name/index.
    // We use the name after ports reset to try to found previous outputs port
    // and replaces the old indexes by the new ones.
    QMap<QString, int> oldPortNameIndexes;

    for (int i = 0; i < m_outputDeviceModel->rowCount(); ++i)
    {
        oldPortNameIndexes.insert(m_outputDeviceModel->name(i), i);
    }

    closeOutputPorts();
    resetPhysicalMidiOutPorts();

    if (m_midiOuts.empty())
    {
        qWarning() << "[MidiMonitor]: No midi outputs";
    }

    // Replace the old outputs ports indexes by the new ones if possible.
    for (int i = 0; i < m_outputDeviceModel->rowCount(); ++i)
    {
        auto const name = m_outputDeviceModel->name(i);
        auto const oldIndex = oldPortNameIndexes.value(name);

        outputRemappings.insert(oldIndex, i);
    }
}

void QMidiManagerPrivate::resetPhysicalMidiOutPorts()
{
    // We need one instance of QMidiOut in order to get the port count.
    // This limitation is discused here: https://github.com/thestk/rtmidi/issues/50
    // It could be done by using a static instance of MidiApi but it can cause several
    // drawback. The first I get in mind it can become difficult to manage several port instances into
    // different thread because ports should share the same MidiApi instance.
    auto firstMidiOut = std::make_unique<QMidiOut>();

    if (firstMidiOut->portCount() > 0u)
    {
        m_midiOuts.emplace_back(std::move(firstMidiOut));

        // Instanciate midi other inputs
        for (int i = 0; i < m_midiOuts.front()->portCount() - 1; ++i)
        {
            auto midiOut = std::make_unique<QMidiOut>();

            m_midiOuts.emplace_back(std::move(midiOut));
        }
        for (int i = 0; i < m_midiOuts.size(); ++i)
        {
            m_midiOuts[i]->openPort(i);
        }
    }
    m_outputDeviceModel->reset(m_midiOuts);
}

void QMidiManagerPrivate::closeOutputPorts()
{
    m_midiOuts.clear();
}

void QMidiManagerPrivate::closeInputPorts()
{
    m_midiIns.clear();
}

QMidiManager::QMidiManager(QObject* parent)
: QObject(parent)
, d_ptr(new QMidiManagerPrivate(this))
{
}

QMidiManager::~QMidiManager()
{
}

QMidiDeviceModel* QMidiManager::getInputDeviceModel() const
{
    Q_D(const QMidiManager);

    return d->m_inputDeviceModel;
}

QMidiDeviceModel* QMidiManager::getOutputDeviceModel() const
{
    Q_D(const QMidiManager);

    return d->m_outputDeviceModel;
}

/*!
 * \brief Rescan physical MIDI ports
 */
void QMidiManager::rescanPorts()
{
    QMap<int, int> inputRemappings;
    QMap<int, int> outputRemappings;

    rescanPorts(inputRemappings, outputRemappings);
}

namespace
{
    template <class PortContainer>
    inline QMap<int, QString> extractPortNames(PortContainer const& ports)
    {
        QMap<int, QString> results;

        for (decltype(ports.size()) i = 0; i < ports.size(); ++i)
        {
            results.insert(i, ports[i]->portName());
        }
        return results;
    };
}

// TODO: should be better if inputRemappings is returned as value instead of passed as parameter because
// we can try to changes the values of inputs remappings passed as parameters but it changes nothing.
//
// But the return type is not used to define method signature so if I remove the parameter mapping then
// I must change the name of one overload (e.g rescanPortsAndGetMappings()).
/*!
 * \brief Rescans MIDI ports
 *
 * Input ports model, output ports model and matrix model are updated.
 */
void QMidiManager::rescanPorts(QMap<int, int>& inputRemapping, QMap<int, int>& outputRemapping)
{
    Q_D(QMidiManager);

    d->resetMidiInPorts(inputRemapping);
    d->resetMidiOutPorts(outputRemapping);
    d->m_matrixModel->reset(d->m_midiOuts.size(), d->m_midiIns.size(), extractPortNames(d->m_midiOuts), extractPortNames(d->m_midiIns));
    emit portsRescanned();
}

void QMidiManager::closeAllPorts()
{
    Q_D(QMidiManager);

    d->closeOutputPorts();
    d->closeInputPorts();
    d->m_matrixModel->clear();
}

void QMidiManager::setInputPortEnabled(int const portId, bool const enabled)
{
    Q_D(QMidiManager);

    for (auto const& midiIn : d->m_midiIns)
    {
        if (midiIn->portOpened() == portId)
        {
            midiIn->setPortEnabled(enabled);
            return;
        }
    }
}

void QMidiManager::setOutputPortEnabled(int const portId, bool const enabled)
{
    Q_D(QMidiManager);

    for (auto const& midiOut : d->m_midiOuts)
    {
        if (midiOut->portOpened() == portId)
        {
            midiOut->setEnabled(enabled);
            return;
        }
    }
}

QMidiMessageMatrixModel* QMidiManager::getMessageMatrixModel() const
{
    Q_D(const QMidiManager);

    return d->m_matrixModel;
}

/*!
 * \brief Method called by a MIDI input port when a message is received.
 * \param message Message forwarded through the MIDI message matrix.
 */
void QMidiManagerPrivate::forwardMidiMessage(QMidiMessage const& message)
{
    auto const& matrix = m_matrixModel->matrix();

    matrix.forachInput(message.port(),
                         [this, &message](auto out, auto in)
                         {
                             m_midiOuts.at(out)->sendMessage(message);
                         });
}

int QMidiManager::addInputPort(std::unique_ptr<QAbstractMidiIn>&& midiIn)
{
    Q_ASSERT( midiIn != nullptr );
    Q_D(QMidiManager);

    int const newPortIndex = d->m_midiIns.size();
    int returnedPortIndex = -1;

    if (midiIn->isPortOpen() || midiIn->openPort(newPortIndex))
    {
        midiIn->addMessageReceivedListener([this, d](QMidiMessage const& message)
                                           {
                                               emit messageReceived(message);
                                               d->forwardMidiMessage(message);
                                           });
        d->m_inputDeviceModel->append(midiIn->portName());
        d->m_midiIns.emplace_back(std::move(midiIn));
        d->m_matrixModel->reset(d->m_midiOuts.size(), d->m_midiIns.size(), extractPortNames(d->m_midiOuts), extractPortNames(d->m_midiIns));
        returnedPortIndex = newPortIndex;
    }
    return returnedPortIndex;
}

int QMidiManager::addOutputPort(std::unique_ptr<QAbstractMidiOut>&& midiOut)
{
    Q_ASSERT( midiOut != nullptr );
    Q_D(QMidiManager);

    int const newPortIndex = d->m_midiIns.size();
    int returnedPortIndex = -1;

    if (midiOut->isPortOpen() || midiOut->openPort(newPortIndex))
    {
        d->m_outputDeviceModel->append(midiOut->portName());
        d->m_midiOuts.emplace_back(std::move(midiOut));
        d->m_matrixModel->reset(d->m_midiOuts.size(), d->m_midiIns.size(), extractPortNames(d->m_midiOuts), extractPortNames(d->m_midiIns));
        returnedPortIndex = newPortIndex;
    }
    return returnedPortIndex;
}