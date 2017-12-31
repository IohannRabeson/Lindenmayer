//
// Created by Io on 31/12/2017.
//

#ifndef SYSEXMONITOR_MESSAGELISTVIEW_HPP
#define SYSEXMONITOR_MESSAGELISTVIEW_HPP
#include <QTableView>

class QMidiMessageModel;

class MessageListView : public QTableView
{
public:
    MessageListView(QMidiMessageModel* model, QWidget* parent = nullptr);
};

#endif //SYSEXMONITOR_MESSAGELISTVIEW_HPP
