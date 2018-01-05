//
// Created by Io on 01/01/2018.
//

#ifndef MIDIMONITOR_COMMONUI_HPP
#define MIDIMONITOR_COMMONUI_HPP

class QTableView;
class QTreeView;

class CommonUi
{
public:
    static void standardTableView(QTableView* const view, bool const showTopHeader = true);
    static void standardTreeView(QTreeView* const view, bool const showTopHeader = true);
};

#endif //MIDIMONITOR_COMMONUI_HPP
