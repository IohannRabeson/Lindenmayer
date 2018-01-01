//
// Created by Io on 31/12/2017.
//

#ifndef SYSEXMONITOR_MIDIWIDGET_HPP
#define SYSEXMONITOR_MIDIWIDGET_HPP
#include <QWidget>

class QMidiMessage;
class QMidiMessageModel;
class QMidiPortModel;

class MidiMessageListView;

class QComboBox;

class MidiWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool autoScroll READ isAutoScrollEnabled WRITE setAutoScroll)
public:
    explicit MidiWidget(QWidget* parent = nullptr);

    void setPortModel(QMidiPortModel* midiPortModel);
    void setAutoScroll(bool const autoscroll);
    bool isAutoScrollEnabled() const;
private:
    virtual void onMidiPortChanged(int const portIndex) = 0;
public slots:
    void append(QMidiMessage const& message);
    void clear();
signals:
    void messageClicked(QMidiMessage const& message);
    void messageDoubleClicked(QMidiMessage const& message);
private:
    QMidiMessageModel* const m_model;
    QComboBox* const m_portSelector;
    MidiMessageListView* const m_messageList;
    bool m_autoScroll = true;
};

#endif //SYSEXMONITOR_MIDIWIDGET_HPP
