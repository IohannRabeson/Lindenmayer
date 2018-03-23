//
// Created by Io on 06/01/2018.
//

#ifndef MIDIMONITOR_MIDINOTETRIGGERWIDGET_HPP
#define MIDIMONITOR_MIDINOTETRIGGERWIDGET_HPP
#include <QWidget>
#include "QAbstractMidiIn.hpp"
#include "QMidiInBase.hpp"

class QMidiMessage;
class QPushButton;
class QSpinBox;
class QComboBox;
class QSettings;
class QMidiNoteModel;

class MidiNoteTriggerWidget : public QWidget, public QMidiInBase
{
    Q_OBJECT
public:
    explicit MidiNoteTriggerWidget(QWidget* parent);

    void saveSettings(QSettings& settings) const;
    void loadSettings(QSettings& settings);
    bool isPortRemovable() const override;
private:
    bool eventFilter(QObject *object, QEvent *event) override;
    void onPressed();
    void onReleased();

    unsigned char note() const;
    unsigned char velocity() const;
    unsigned char channel() const;
private:
    QPushButton* const m_trigger;
    QComboBox* const m_note;
    QSpinBox* const m_velocity;
    QSpinBox* const m_channel;
    QMidiNoteModel* const m_noteModel;
};

#endif //MIDIMONITOR_MIDINOTETRIGGERWIDGET_HPP
