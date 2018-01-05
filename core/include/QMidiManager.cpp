//
// Created by Io on 04/01/2018.
//

#include "QMidiManager.hpp"
#include "QMidiDeviceModel.hpp"
#include "QMidiIn.hpp"

#include <QtDebug>

QMidiManager::QMidiManager(QObject* parent)
: m_inputDeviceModel(new QMidiDeviceModel(this))
{
    resetMidiInPorts();
}

QMidiDeviceModel* QMidiManager::getInputDeviceModel() const
{
    return m_inputDeviceModel;
}

void QMidiManager::resetMidiInPorts()
{
    closeAll();
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
    qDeleteAll(m_midiIns);
    m_midiIns.clear();
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
