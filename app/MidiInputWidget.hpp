//
// Created by Io on 31/12/2017.
//

#ifndef SYSEXMONITOR_MIDIINPUTWIDGET_HPP
#define SYSEXMONITOR_MIDIINPUTWIDGET_HPP
#include <QWidget>

class MessageListView;
class QMidiIn;
class QMidiMessage;
class QMidiMessageModel;

class MidiInputWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MidiInputWidget(QWidget* parent = nullptr);
signals:
    void messageClicked(QMidiMessage const& message);
    void messageDoubleClicked(QMidiMessage const& message);
private:
    QMidiIn* const m_midiIn;
    QMidiMessageModel* const m_model;
    MessageListView* const m_messageList;
};

#endif //SYSEXMONITOR_MIDIINPUTWIDGET_HPP
