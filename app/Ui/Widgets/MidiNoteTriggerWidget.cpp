//
// Created by Io on 06/01/2018.
//

#include "MidiNoteTriggerWidget.hpp"
#include "Ui/Delegates/MidiDelegates.hpp"
#include "Ui/CommonUi.hpp"

#include <QFormLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QComboBox>
#include <QLineEdit>
#include <QSettings>

#include <QMidiMessageBuilder.hpp>
#include <QMidiNoteModel.hpp>

/*!
 * \class MidiNoteTriggerWidget
 * \brief Widget allowing to trigger note on/off MIDI messages
 */

MidiNoteTriggerWidget::MidiNoteTriggerWidget(QWidget* parent)
: QWidget(parent)
, QMidiInBase(tr("Note trigger"))
, m_trigger(new QPushButton(tr("Trigger"), this))
, m_note(new QComboBox(this))
, m_velocity(new QSpinBox(this))
, m_channel(new QSpinBox(this))
, m_noteModel(new QMidiNoteModel(this))
{
    QFormLayout* layout = new QFormLayout(this);

    CommonUi::standardLayout(layout);

    m_note->setEditable(true);
    m_note->lineEdit()->setReadOnly(true);
    m_note->setModel(m_noteModel);
    m_velocity->setRange(1, 127);
    m_velocity->setValue(100);
    m_channel->setRange(1, 16);
    m_trigger->installEventFilter(this);

    layout->addRow(tr("Note:"), m_note);
    layout->addRow(tr("Velocity:"), m_velocity);
    layout->addRow(tr("Channel:"), m_channel);
    layout->addRow(m_trigger);
}

bool MidiNoteTriggerWidget::eventFilter(QObject* object, QEvent* event)
{
    switch (event->type())
    {
        case QEvent::Type::MouseButtonPress:
            onPressed();
            break;
        case QEvent::Type::MouseButtonRelease:
            onReleased();
            break;
        default:
            break;
    }
    return QObject::eventFilter(object, event);
}

unsigned char MidiNoteTriggerWidget::note() const
{
    return static_cast<unsigned char>(m_note->currentIndex());
}

unsigned char MidiNoteTriggerWidget::velocity() const
{
    Q_ASSERT(m_velocity->value() > -1);
    Q_ASSERT(m_velocity->value() < 128);

    return static_cast<unsigned char>(m_velocity->value());
}

unsigned char MidiNoteTriggerWidget::channel() const
{
    Q_ASSERT(m_velocity->value() > 0);
    Q_ASSERT(m_channel->value() < 17);

    return static_cast<unsigned char>(m_channel->value() - 1);
}

void MidiNoteTriggerWidget::saveSettings(QSettings& settings) const
{
    settings.beginGroup("midi_note_trigger_widget");
    settings.setValue("note", m_note->currentIndex());
    settings.setValue("velocity", m_velocity->value());
    settings.setValue("channel", m_channel->value());
    settings.endGroup();
}

void MidiNoteTriggerWidget::loadSettings(QSettings& settings)
{
    settings.beginGroup("midi_note_trigger_widget");
    m_note->setCurrentIndex(settings.value("note", 60).toInt());
    m_velocity->setValue(settings.value("velocity", 100).toInt());
    m_channel->setValue(settings.value("channel", 1).toInt());
    settings.endGroup();
}

void MidiNoteTriggerWidget::onPressed()
{
    messageReceived(QMidiMessageBuilder::note(note(), velocity(), channel(), portOpened(), true));
}

void MidiNoteTriggerWidget::onReleased()
{
    messageReceived(QMidiMessageBuilder::note(note(), 0, channel(), portOpened(), false));
}
