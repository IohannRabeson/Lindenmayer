//
// Created by Io on 31/12/2017.
//

#ifndef MIDIMONITOR_MIDIMESSAGELISTVIEW_HPP
#define MIDIMONITOR_MIDIMESSAGELISTVIEW_HPP
#include <QTreeView>

#include <QMidiOutBase.hpp>

class QMidiManager;
class QMidiMessageModel;
class QMidiMessage;

class QSettings;

class MidiMessageListView : public QTreeView, public QMidiOutBase
{
    Q_OBJECT
public:
    explicit MidiMessageListView(QMidiManager* const midiManager, QWidget* parent = nullptr);

    void append(QMidiMessage const& message);
    void setAutoScrollToBottomEnabled(bool const enabled);
    void loadSettings(QSettings& settings);
    void saveSettings(QSettings& settings) const;
private:
    void outputMessage(const QMidiMessage &message) override;
private:
    QMidiManager* const m_midiManager;
    QMidiMessageModel* const m_messageModel;
    bool m_autoScroll;
};

#endif //MIDIMONITOR_MIDIMESSAGELISTVIEW_HPP
