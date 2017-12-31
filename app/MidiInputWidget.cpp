//
// Created by Io on 31/12/2017.
//

#include "MidiInputWidget.hpp"
#include "MessageListView.hpp"

#include <QVBoxLayout>

#include <QMidiMessageModel.hpp>
#include <QMidiIn.hpp>
#include <QtDebug>

MidiInputWidget::MidiInputWidget(QWidget* parent)
: MidiWidget(parent)
, m_midiIn(new QMidiIn(this))
{
    connect(m_midiIn, &QMidiIn::messageReceived, this, &MidiInputWidget::append);
    connect(m_midiIn, &QMidiIn::error, [](QString const& error)
    {
        qWarning() << error;
    });
    m_midiIn->openPort(0);
}

