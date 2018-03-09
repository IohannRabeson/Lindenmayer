//
// Created by Io on 08/03/2018.
//

#ifndef MIDIMONITOR_MIDIPORTTREEVIEW_HPP
#define MIDIMONITOR_MIDIPORTTREEVIEW_HPP
#include <QTreeView>
#include <QPointer>

class QMidiMessageFilterFactory;
class QSignalMapper;
class QContextMenuEvent;

class QMidiPortModel;

class MidiPortTreeView : public QTreeView
{
public:
    MidiPortTreeView(QMidiPortModel* model, QMidiMessageFilterFactory* filterFactory, QWidget* parent = nullptr);
protected:
    void contextMenuEvent(QContextMenuEvent *event);
private:
    void updateActions();

    void addCreateFilterAction(int i);

    void onFilterFactoryResetted();
    void onFilterFactoryRowsInserted(QModelIndex const& parent, int first, int last);
    void onAddFilterActionTriggered(int const filterRow);
private:
    QPointer<QMidiPortModel> m_portModel;
    QPointer<QMidiMessageFilterFactory> m_filterFactory;
    QSignalMapper* const m_filterSelectorSignalMapper;
    QList<QAction*> m_actionAddFilters;
    QAction* const m_actionRemoveFilter;
};

#endif //MIDIMONITOR_MIDIPORTTREEVIEW_HPP
