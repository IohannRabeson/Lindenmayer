//
// Created by Io on 04/01/2018.
//

#include "QMidiManager.hpp"
#include "QMidiDeviceModel.hpp"
#include "QMidiIn.hpp"
#include "QMidiOut.hpp"
#include "QMidiMessageMatrixModel.hpp"
#include "QMidiMessage.hpp"
#include "QMidiIn.hpp"
#include "QMidiOut.hpp"

#include <QtDebug>
#include <QSize>

#include <vector>
#include <memory>

QMidiManager::QMidiManager(QObject* parent)
: QObject(parent)
, m_inputDeviceModel(new QMidiDeviceModel(this))
, m_outputDeviceModel(new QMidiDeviceModel(this))
, m_matrixModel(new QMidiMessageMatrixModel(this))
{
}

QMidiDeviceModel* QMidiManager::getInputDeviceModel() const
{
    return m_inputDeviceModel;
}

QMidiDeviceModel* QMidiManager::getOutputDeviceModel() const
{
    return m_outputDeviceModel;
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
    resetMidiInPorts(inputRemapping);
    resetMidiOutPorts(outputRemapping);
    m_matrixModel->reset(m_midiOuts.size(), m_midiIns.size());
    emit portsRescaned();
}

void QMidiManager::resetPhysicalMidiInPorts()
{
    // Instanciate the first MIDI port, then scans available ports.
    // Wierd but it seems to be required by RtMidi

    auto firstMidiPort = std::make_unique<QMidiIn>();
    auto const portCount = firstMidiPort->portCount();

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
    m_inputDeviceModel->reset(m_midiIns);
}

void QMidiManager::resetMidiInPorts(QMap<int, int>& inputRemappings)
{
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

            midiIn->addMessageReceivedListener([this](QMidiMessage const& message)
                                               {
                                                   emit messageReceived(message);
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

void QMidiManager::resetMidiOutPorts(QMap<int, int>& outputRemappings)
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

    if (m_midiOuts.isEmpty())
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

void QMidiManager::resetPhysicalMidiOutPorts()
{
    // Instanciate the first MIDI port, then scans available ports.
    // Wierd but it seems to be required by RtMidi
    auto* const firstMidiOut = new QMidiOut(this);

    m_midiOuts.append(firstMidiOut);

    // Instanciate midi other inputs
    for (int i = 0; i < firstMidiOut->portCount() - 1; ++i)
    {
        auto* const midiOut = new QMidiOut(this);

        m_midiOuts.append(midiOut);
    }
    for (int i = 0; i < m_midiOuts.size(); ++i)
    {
        m_midiOuts[i]->openPort(i);
    }
    m_outputDeviceModel->reset(m_midiOuts);
}

void QMidiManager::closeAll()
{
    closeOutputPorts();
    closeInputPorts();
    m_matrixModel->clear();
}

void QMidiManager::setInputPortEnabled(int const portId, bool const enabled)
{
    for (auto const& midiIn : m_midiIns)
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
    for (auto* midiOut : m_midiOuts)
    {
        if (midiOut->portOpened() == portId)
        {
            midiOut->setEnabled(enabled);
            return;
        }
    }
}

void QMidiManager::closeOutputPorts()
{
    qDeleteAll(m_midiOuts);
    m_midiOuts.clear();
}

void QMidiManager::closeInputPorts()
{
    m_midiIns.clear();
}

void QMidiManager::sendMessage(QMidiMessage const& message)
{
    for (auto* midiOut : m_midiOuts)
    {
        midiOut->sendMessage(message);
    }
    emit messageSent(message);
}

QMidiMessageMatrixModel* QMidiManager::getMessageMatrixModel() const
{
    return m_matrixModel;
}

void QMidiManager::forwardMidiMessage(QMidiMessage const& message)
{
    auto const& matrix = m_matrixModel->matrix();

    matrix.forachInput(message.port(),
                         [this, &message](auto in, auto out)
                         {
                             m_midiOuts.at(in)->sendMessage(message);
                         });
}

void QMidiManager::addInputPort(std::unique_ptr<QAbstractMidiIn>&& midiIn)
{
    int const newPortIndex = m_midiIns.size();

    if (midiIn->openPort(newPortIndex))
    {
        midiIn->addMessageReceivedListener([this](QMidiMessage const& message)
                                           {
                                               emit messageReceived(message);
                                               forwardMidiMessage(message);
                                           });
        m_inputDeviceModel->append(midiIn->portName());
        m_midiIns.emplace_back(std::move(midiIn));
        m_matrixModel->reset(m_midiOuts.size(), m_midiIns.size());
    }
}

void QMidiManager::addOutputPort(QAbstractMidiOut* midiOut)
{
    m_midiOuts.append(midiOut);
    m_matrixModel->reset(m_midiOuts.size(), m_midiIns.size());
}
