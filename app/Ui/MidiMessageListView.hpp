//
// Created by Io on 31/12/2017.
//

#ifndef MIDIMONITOR_MIDIMESSAGELISTVIEW_HPP
#define MIDIMONITOR_MIDIMESSAGELISTVIEW_HPP
#include <QTreeView>

class QMidiMessageModel;
class QSettings;

class MidiMessageListView : public QTreeView
{
public:
    explicit MidiMessageListView(QMidiMessageModel* model, QWidget* parent = nullptr);

    void setAutoScrollToBottomEnabled(bool const enabled);
    void loadSettings(QSettings& settings);
    void saveSettings(QSettings& settings) const;
private:
    bool m_autoScroll;
};

#endif //MIDIMONITOR_MIDIMESSAGELISTVIEW_HPP
