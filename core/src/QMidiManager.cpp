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

QMidiManager::QMidiManager(QObject* parent)
: QObject(parent)
, m_inputDeviceModel(new QMidiDeviceModel(this))
, m_outputDeviceModel(new QMidiDeviceModel(this))
, m_matrixModel(new QMidiMessageMatrixModel(this))
{
    rescanPorts();
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
    m_matrixModel->reset(m_physicalMidiOuts.size(), m_physicalMidiIns.size());
    emit portsRescaned();
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

    if (m_physicalMidiIns.isEmpty())
    {
        qWarning() << "[MidiMonitor]: No midi inputs";
    }
    else
    {
        // Open midi inputs
        for (int i = 0; i < m_physicalMidiIns.size(); ++i)
        {
            auto* const midiIn = m_physicalMidiIns[i];

            connect(midiIn, &QMidiIn::messageReceived, this, &QMidiManager::messageReceived, Qt::DirectConnection);
            connect(midiIn, &QMidiIn::messageReceived, this, &QMidiManager::sendToOutputs, Qt::DirectConnection);

            midiIn->openPort(i);
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

void QMidiManager::resetPhysicalMidiInPorts()
{
    // Instanciate the first MIDI port, then scans available ports.
    // Wierd but it seems to be required by RtMidi
    auto* const firstMidiIn = new QMidiIn(this);

    m_physicalMidiIns.append(firstMidiIn);
    m_inputDeviceModel->rescan(firstMidiIn);

    // Instanciate midi other inputs
    for (int i = 0; i < m_inputDeviceModel->rowCount() - 1; ++i)
    {
        auto* midiIn = new QMidiIn(this);

        m_physicalMidiIns.append(midiIn);
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

    if (m_physicalMidiOuts.isEmpty())
    {
        qWarning() << "[MidiMonitor]: No midi outputs";
    }
    else
    {
        // Open midi outputs
        for (int i = 0; i < m_physicalMidiOuts.size(); ++i)
        {
            auto* const midiOut = m_physicalMidiOuts[i];

            midiOut->openPort(i);
        }
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

    m_physicalMidiOuts.append(firstMidiOut);
    m_outputDeviceModel->rescan(firstMidiOut);

    // Instanciate midi other inputs
    for (int i = 0; i < m_outputDeviceModel->rowCount() - 1; ++i)
    {
        auto* const midiOut = new QMidiOut(this);

        m_physicalMidiOuts.append(midiOut);
    }
}

void QMidiManager::closeAll()
{
    closeOutputPorts();
    closeInputPorts();
    m_matrixModel->clear();
}

void QMidiManager::setInputPortEnabled(int const portId, bool const enabled)
{
    for (auto* midiIn : m_physicalMidiIns)
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
    for (auto* midiOut : m_physicalMidiOuts)
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
    qDeleteAll(m_physicalMidiOuts);
    m_physicalMidiOuts.clear();
}

void QMidiManager::closeInputPorts()
{
    qDeleteAll(m_physicalMidiIns);
    m_physicalMidiIns.clear();
}

void QMidiManager::sendMessage(QMidiMessage const& message)
{
    for (auto* midiOut : m_physicalMidiOuts)
    {
        midiOut->sendMessage(message);
    }
    emit messageSent(message);
}

QMidiMessageMatrixModel* QMidiManager::getMessageMatrixModel() const
{
    return m_matrixModel;
}

void QMidiManager::sendToOutputs(QMidiMessage const& message)
{
    auto const& matrix = m_matrixModel->matrix();

    matrix.foreachInLine(message.port(),
                         [this, &message](auto in, auto out, auto value)
                         {
                              if (value)
                              {
                                  m_physicalMidiOuts.at(in)->sendMessage(message);
                                  qDebug() << in << out;
                              }
                         });
}

void QMidiManager::addInputPort(QAbstractMidiIn* midiIn)
{

}

void QMidiManager::addOutputPort(QAbstractMidiOut* midiOut)
{

}
