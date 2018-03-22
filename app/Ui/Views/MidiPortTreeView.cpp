//
// Created by Io on 08/03/2018.
//

#include "MidiPortTreeView.hpp"
#include "Ui/CommonUi.hpp"
#include "Ui/Views/MidiConsoleView.hpp"

#include <QMidiMessageFilterFactory.hpp>
#include <QMidiPortModel.hpp>
#include <QVirtualMidiIn.hpp>
#include <QVirtualMidiOut.hpp>
#include <QMidiManager.hpp>

#include <QSignalMapper>
#include <QAction>
#include <QContextMenuEvent>
#include <QMenu>

namespace
{
    QModelIndex getCurrentPortIndex(QTreeView const* const view, QMidiPortModel const* const portModel)
    {
        auto currentIndex = view->currentIndex();

        if (portModel->getItemType(currentIndex) == QMidiPortModel::ItemType::Filter)
        {
            currentIndex = portModel->parent(currentIndex);
        }
        return currentIndex;
    }

    QModelIndex getCurrentFilterIndex(QTreeView const* const view, QMidiPortModel const* const portModel)
    {
        auto currentIndex = view->currentIndex();
        QModelIndex result;

        if (portModel->getItemType(currentIndex) == QMidiPortModel::ItemType::Filter)
        {
            result = currentIndex;
        }
        return result;
    }
}

MidiPortTreeView::MidiPortTreeView(Mode const mode, QMidiManager* const manager, QWidget* parent)
: QTreeView(parent)
, m_midiManager(manager)
, m_portModel(mode == Mode::In ? manager->getInputDeviceModel() : manager->getOutputDeviceModel())
, m_filterFactory(manager->getMessageFilterFactory())
, m_filterSelectorSignalMapper(new QSignalMapper(this))
, m_actionRemove(new QAction(tr("Remove"), this))
, m_actionAddVirtualMidiInput(new QAction(tr("Add virtual MIDI in"), this))
, m_actionAddVirtualMidiOutput(new QAction(tr("Add virtual MIDI out"), this))
, m_actionAddMidiConsole(new QAction(tr("Add MIDI console"), this))
, m_mode(mode)
{
    CommonUi::standardTreeView(this, true);

    setModel(m_portModel);

    connect(m_filterFactory, &QMidiMessageFilterFactory::modelReset, this, &MidiPortTreeView::onFilterFactoryResetted);
    connect(m_filterFactory, &QMidiMessageFilterFactory::rowsInserted, this, &MidiPortTreeView::onFilterFactoryRowsInserted);
    connect(m_filterSelectorSignalMapper, qOverload<int>(&QSignalMapper::mapped), this, &MidiPortTreeView::onAddFilterActionTriggered);
    connect(m_actionRemove, &QAction::triggered, this, &MidiPortTreeView::onRemoveFilterActionTriggered);
    connect(m_actionAddVirtualMidiInput, &QAction::triggered, this, &MidiPortTreeView::onAddVirtualMidiInputTriggered);
    connect(m_actionAddVirtualMidiOutput, &QAction::triggered, this, &MidiPortTreeView::onAddVirtualMidiOutputTriggered);
    connect(m_actionAddMidiConsole, &QAction::triggered, this, &MidiPortTreeView::onAddOutputLoggerTriggered);

    setRootIsDecorated(true);
    setExpandsOnDoubleClick(true);
    setItemDelegateForColumn(1, new ValueColumnDelegate(this));
    setSelectionMode(QTreeView::SelectionMode::SingleSelection);
    setSelectionBehavior(QTreeView::SelectionBehavior::SelectRows);
    setHeaderHidden(false);

    onFilterFactoryResetted();
}

void MidiPortTreeView::addCreateFilterAction(int i)
{
    auto const index = m_filterFactory->index(i);
    QString const filterLabel = m_filterFactory->getLabel(index);
    QAction* const action = new QAction(filterLabel, this);

    m_actionAddFilters.append(action);
    m_filterSelectorSignalMapper->setMapping(action, i);
    connect(action, &QAction::triggered, m_filterSelectorSignalMapper, qOverload<>(&QSignalMapper::map));
}

void MidiPortTreeView::onFilterFactoryResetted()
{
    qDeleteAll(m_actionAddFilters);
    m_actionAddFilters.clear();

    for (auto i = 0; i < m_filterFactory->rowCount(); ++i)
    {
        addCreateFilterAction(i);
    }
}

void MidiPortTreeView::onFilterFactoryRowsInserted(QModelIndex const& parent, int first, int last)
{
    for (int i = first; i <= last; ++i)
    {
        addCreateFilterAction(i);
    }
}

void MidiPortTreeView::onAddFilterActionTriggered(int const filterRow)
{
    auto const currentPortIndex = getCurrentPortIndex(this, m_portModel);

    if (currentPortIndex.isValid())
    {
        auto const filterCreatorIndex = m_filterFactory->index(filterRow);
        auto const newFilter = m_filterFactory->create(filterCreatorIndex);
        auto const newFilterIndex = m_portModel->add(currentPortIndex, newFilter);

        setCurrentIndex(newFilterIndex);
        setExpanded(currentPortIndex, true);
    }
}

void MidiPortTreeView::onRemoveFilterActionTriggered()
{
    auto const currentIndex = this->currentIndex();

    if (currentIndex.isValid() && m_portModel->isRemovable(currentIndex))
    {
        m_portModel->remove(currentIndex);
    }
}

void MidiPortTreeView::onAddVirtualMidiInputTriggered()
{
    auto const newIndex = m_portModel->add(std::make_shared<QVirtualMidiIn>());

    setCurrentIndex(newIndex);
}

void MidiPortTreeView::onAddVirtualMidiOutputTriggered()
{
    auto const newIndex = m_portModel->add(std::make_shared<QVirtualMidiOut>());

    setCurrentIndex(newIndex);
}

void MidiPortTreeView::onAddOutputLoggerTriggered()
{
    auto const newIndex = m_midiManager->getOutputDeviceModel()->add(std::make_shared<MidiConsoleView>(m_midiManager));

    setCurrentIndex(newIndex);
}

void MidiPortTreeView::contextMenuEvent(QContextMenuEvent* event)
{
    // TODO: Produce memory leaks on OSX: it seems the cocoa code behind
    // produce a leak repeatedly...
    // 19/03/2018
    QMenu menu(this);
    QMenu addFilterSubmenu(tr("Add filter"));

    auto const currentCanHaveFilter = getCurrentPortIndex(this, m_portModel).isValid();

    addFilterSubmenu.setEnabled(!m_actionAddFilters.isEmpty() && currentCanHaveFilter);

    switch (m_mode)
    {
    case Mode::In:
        menu.addAction(m_actionAddVirtualMidiInput);
        break;
    case Mode::Out:
        menu.addAction(m_actionAddVirtualMidiOutput);
        menu.addAction(m_actionAddMidiConsole);
        break;
    }
    menu.addMenu(&addFilterSubmenu);
    menu.addSeparator();
    menu.addAction(m_actionRemove);

    addFilterSubmenu.addSeparator();
    addFilterSubmenu.addActions(m_actionAddFilters);
    addFilterSubmenu.addSeparator();

    updateActions();

    menu.exec(event->globalPos());
}

void MidiPortTreeView::updateActions()
{  
    m_actionRemove->setEnabled(m_portModel->isRemovable(currentIndex()));
}
