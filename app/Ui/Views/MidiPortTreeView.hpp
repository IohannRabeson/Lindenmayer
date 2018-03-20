//
// Created by Io on 08/03/2018.
//

#ifndef MIDIMONITOR_MIDIPORTTREEVIEW_HPP
#define MIDIMONITOR_MIDIPORTTREEVIEW_HPP
#include <QTreeView>
#include <QPointer>

class QMidiManager;
class QMidiMessageFilterFactory;
class QSignalMapper;
class QContextMenuEvent;

class QMidiPortModel;

class MidiPortTreeView : public QTreeView
{
    Q_OBJECT
public:
    enum class Mode
    {
        In,
        Out
    };

    MidiPortTreeView(Mode const mode, QMidiManager* const manager, QWidget* parent = nullptr);
protected:
    void contextMenuEvent(QContextMenuEvent *event);
private:
    void updateActions();

    void addCreateFilterAction(int i);

    void onFilterFactoryResetted();
    void onFilterFactoryRowsInserted(QModelIndex const& parent, int first, int last);
    void onAddFilterActionTriggered(int const filterRow);
    void onRemoveFilterActionTriggered();
    void onAddVirtualMidiInputTriggered();
    void onAddVirtualMidiOutputTriggered();

    class ValueColumnDelegate;
private:
    QPointer<QMidiManager> m_midiManager;
    QPointer<QMidiPortModel> m_portModel;
    QPointer<QMidiMessageFilterFactory> m_filterFactory;
    QSignalMapper* const m_filterSelectorSignalMapper;
    QList<QAction*> m_actionAddFilters;
    QAction* const m_actionRemove;
    QAction* const m_actionAddVirtualMidiInput;
    QAction* const m_actionAddVirtualMidiOutput;
    Mode const m_mode;
};

#include <QStyledItemDelegate>

class MidiPortTreeView::ValueColumnDelegate : public QStyledItemDelegate
{
public:
    using QStyledItemDelegate::QStyledItemDelegate;
};

#endif //MIDIMONITOR_MIDIPORTTREEVIEW_HPP
