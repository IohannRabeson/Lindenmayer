//
// Created by Io on 31/12/2017.
//

#include "MidiInputWidget.hpp"
#include "MidiMessageListView.hpp"

#include <QMidiMessageModel.hpp>
#include <QMidiPortModel.hpp>
#include <QMidiIn.hpp>

#include <QtDebug>

MidiInputWidget::MidiInputWidget(QWidget* parent)
: MidiWidget(parent)
, m_midiIn(new QMidiIn(this))
, m_midiInPortModel(new QMidiPortModel(this))
{
    connect(m_midiIn, &QMidiIn::messageReceived, this, &MidiInputWidget::append);
    connect(m_midiIn, &QMidiIn::error, [](QString const& error)
    {
        qWarning() << error;
    });
    m_midiInPortModel->rescan(m_midiIn);
    setPortModel(m_midiInPortModel);
}

void MidiInputWidget::onMidiPortChanged(int const portIndex)
{
    auto const previousPort = m_midiIn->portOpened();

    if (previousPort != portIndex)
    {
        if (previousPort != -1)
        {
            m_midiIn->closePort();
            m_midiInPortModel->setUsed(portIndex, false);
        }
        if (portIndex != -1)
        {
            m_midiIn->openPort(portIndex);
            m_midiInPortModel->setUsed(portIndex, true);
        }
    }
}

