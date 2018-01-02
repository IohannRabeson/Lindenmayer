//
// Created by Io on 31/12/2017.
//

#ifndef MIDIMONITOR_MIDIMESSAGELISTVIEW_HPP
#define MIDIMONITOR_MIDIMESSAGELISTVIEW_HPP
#include <QTableView>

class QMidiMessageModel;

class MidiMessageListView : public QTableView
{
public:
    explicit MidiMessageListView(QMidiMessageModel* model, QWidget* parent = nullptr);

    void setAutoScrollToBottomEnabled(bool const enabled);
private:
    bool m_autoScroll;
};

#endif //MIDIMONITOR_MIDIMESSAGELISTVIEW_HPP
