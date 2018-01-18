//
// Created by Io on 04/01/2018.
//

#include "QMidiManager.hpp"
#include "QMidiDeviceModel.hpp"
#include "QMidiIn.hpp"
#include "QMidiOut.hpp"
#include "QMidiMessageMatrixModel.hpp"

#include <QtDebug>
#include <QSize>

QMidiManager::QMidiManager(QObject* parent)
: QObject(parent)
, m_inputDeviceModel(new QMidiDeviceModel(this))
, m_outputDeviceModel(new QMidiDeviceModel(this))
, m_matrixModel(new QMidiMessageMatrixModel(this))
{
    resetPorts();

}

QMidiDeviceModel* QMidiManager::getInputDeviceModel() const
{
    return m_inputDeviceModel;
}

QMidiDeviceModel* QMidiManager::getOutputDeviceModel() const
{
    return m_outputDeviceModel;
}

void QMidiManager::resetPorts()
{
    QMap<int, int> inputRemappings;

    resetPorts(inputRemappings);
    m_matrixModel->reset(m_midiIns.size(), m_midiOuts.size());
}

void QMidiManager::resetPorts(QMap<int, int>& inputRemappings)
{
    resetMidiInPorts(inputRemappings);
    resetMidiOutPorts();
    m_matrixModel->reset(m_midiIns.size(), m_midiOuts.size());
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

    // Instanciate the first MIDI port, then scans available ports.
    // Wierd but it seems to be required by RtMidi
    // TODO: looking for an alternative to RtMidi?
    m_midiIns.append(new QMidiIn(this));
    m_inputDeviceModel->rescan(m_midiIns.front());

    // Instanciate midi other inputs
    for (int i = 0; i < m_inputDeviceModel->rowCount() - 1; ++i)
    {
        auto* midiIn = new QMidiIn(this);

        m_midiIns.append(midiIn);
    }

    if (m_midiIns.isEmpty())
    {
        qWarning() << "[MidiMonitor]: No midi inputs";
    }
    else
    {
        // Open midi inputs
        for (int i = 0; i < m_midiIns.size(); ++i)
        {
            auto* const midiIn = m_midiIns[i];

            connect(midiIn, &QMidiIn::messageReceived, this, &QMidiManager::messageReceived);

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

void QMidiManager::resetMidiOutPorts()
{
    closeOutputPorts();

    // Instanciate the first MIDI port, then scans available ports.
    // Wierd but it seems to be required by RtMidi
    m_midiOuts.append(new QMidiOut(this));
    m_outputDeviceModel->rescan(m_midiOuts.front());

    // Instanciate midi other inputs
    for (int i = 0; i < m_outputDeviceModel->rowCount() - 1; ++i)
    {
        auto* const midiOut = new QMidiOut(this);

        m_midiOuts.append(midiOut);
    }

    if (m_midiOuts.isEmpty())
    {
        qWarning() << "[MidiMonitor]: No midi inputs";
    }
    else
    {
        // Open midi inputs
        for (int i = 0; i < m_midiOuts.size(); ++i)
        {
            auto* const midiOut = m_midiOuts[i];

            midiOut->openPort(i);
        }
    }
}

void QMidiManager::closeAll()
{
    closeOutputPorts();
    closeInputPorts();
}

void QMidiManager::setInputPortEnabled(int const portId, bool const enabled)
{
    for (auto* midiIn : m_midiIns)
    {
        if (midiIn->portOpened() == portId)
        {
            midiIn->setEnabled(enabled);
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
    qDeleteAll(m_midiIns);
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
