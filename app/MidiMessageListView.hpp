//
// Created by Io on 31/12/2017.
//

#ifndef SYSEXMONITOR_MIDIMESSAGELISTVIEW_HPP
#define SYSEXMONITOR_MIDIMESSAGELISTVIEW_HPP
#include <QTableView>

class QMidiMessageModel;

class MidiMessageListView : public QTableView
{
public:
    explicit MidiMessageListView(QMidiMessageModel* model, QWidget* parent = nullptr);
};

#endif //SYSEXMONITOR_MIDIMESSAGELISTVIEW_HPP
