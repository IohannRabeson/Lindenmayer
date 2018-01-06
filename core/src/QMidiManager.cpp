//
// Created by Io on 04/01/2018.
//

#include "QMidiManager.hpp"
#include "QMidiDeviceModel.hpp"
#include "QMidiIn.hpp"
#include "QMidiOut.hpp"

#include <QtDebug>

QMidiManager::QMidiManager(QObject* parent)
: m_inputDeviceModel(new QMidiDeviceModel(this))
, m_outputDeviceModel(new QMidiDeviceModel(this))
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

void QMidiManager::resetMidiInPorts()
{
    closeInputPorts();
    // Instanciate the first MIDI port, then scans available ports.
    // Wierd but seem to be mandatory.
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

void QMidiManager::resetMidiOutPorts()
{
    closeOutputPorts();

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

//            QObject::connect(midiOut, &QMidiOut::messageReceived, this, &QMidiManager::messageReceived);

            midiOut->openPort(i);
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

void QMidiManager::resetPorts()
{
    resetMidiInPorts();
    resetMidiOutPorts();
}
