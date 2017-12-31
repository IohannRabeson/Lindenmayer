//
// Created by Io on 31/12/2017.
//

#ifndef SYSEXMONITOR_MIDIWIDGET_HPP
#define SYSEXMONITOR_MIDIWIDGET_HPP
#include <QWidget>

class QMidiMessage;
class QMidiMessageModel;
class MessageListView;

class MidiWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool autoScroll READ isAutoScrollEnabled WRITE setAutoScroll)
public:
    explicit MidiWidget(QWidget* parent = nullptr);

    void setAutoScroll(bool const autoscroll);
    bool isAutoScrollEnabled() const;
public slots:
    void append(QMidiMessage const& message);
    void clear();
signals:
    void messageClicked(QMidiMessage const& message);
    void messageDoubleClicked(QMidiMessage const& message);
private:
    QMidiMessageModel* const m_model;
    MessageListView* const m_messageList;
    bool m_autoScroll = true;
};

#endif //SYSEXMONITOR_MIDIWIDGET_HPP
