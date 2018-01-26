//
// Created by Io on 07/01/2018.
//

#ifndef MIDIMONITOR_MIDIKEYBOARDWIDGET_HPP
#define MIDIMONITOR_MIDIKEYBOARDWIDGET_HPP
#include <QWidget>
#include <QGraphicsItem>
#include <QMap>
#include <QAbstractMidiIn.hpp>

class QGraphicsView;
class QGraphicsScene;
class QSpinBox;
class QSettings;
class QStateMachine;
class QPushButton;
class QGroupBox;

class QMidiMessage;

class MidiMessageSender;
class KeyboardGraphicsScene;
class KeyboardGraphicsView;

class MidiKeyboardWidget : public QWidget, public QMidiInBase
{
    Q_OBJECT
public:
    explicit MidiKeyboardWidget(QWidget* parent = nullptr);
    ~MidiKeyboardWidget();

    void saveSettings(QSettings& settings) const;
    void loadSettings(QSettings& settings);
private:
    void setChordEditionEnabled(bool const enabled);
    void setChordEnabled(bool const enabled);
    bool isChordEnabled() const;
    void clearChord();
protected:
    void showEvent(QShowEvent* e);
    void resizeEvent(QResizeEvent* e);
//signals:
//    void sendMessage(QMidiMessage const& message);
private:
    KeyboardGraphicsScene* const m_scene;
    KeyboardGraphicsView* const m_view;
    QSpinBox* const m_velocity;
    QSpinBox* const m_channel;
    QPushButton* const m_editChord;
    QPushButton* const m_clearChord;
    QGroupBox* const m_chordGroup;
    bool m_chordEditor;
};

#endif //MIDIMONITOR_MIDIKEYBOARDWIDGET_HPP
