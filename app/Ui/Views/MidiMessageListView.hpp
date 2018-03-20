//
// Created by Io on 31/12/2017.
//

#ifndef MIDIMONITOR_MIDIMESSAGELISTVIEW_HPP
#define MIDIMONITOR_MIDIMESSAGELISTVIEW_HPP
#include <QTreeView>

class QMidiManager;
class QMidiMessageModel;
class QMidiMessage;

class QSettings;

class MidiMessageListView : public QTreeView
{
public:
    explicit MidiMessageListView(QMidiManager* const midiManager, QWidget* parent = nullptr);

    void append(QMidiMessage const& message);
    void setAutoScrollToBottomEnabled(bool const enabled);
    void loadSettings(QSettings& settings);
    void saveSettings(QSettings& settings) const;
private:
    QMidiManager* const m_midiManager;
    QMidiMessageModel* const m_messageModel;
    bool m_autoScroll;
};

#endif //MIDIMONITOR_MIDIMESSAGELISTVIEW_HPP
