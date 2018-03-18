//
// Created by Io on 08/03/2018.
//

#include "MidiPortTreeView.hpp"
#include "Ui/CommonUi.hpp"

#include <QMidiMessageFilterFactory.hpp>
#include <QMidiPortModel.hpp>
#include <QVirtualMidiIn.hpp>

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

MidiPortTreeView::MidiPortTreeView(QMidiPortModel* portModel, QMidiMessageFilterFactory* filterFactory, QWidget* parent)
: QTreeView(parent)
, m_portModel(portModel)
, m_filterFactory(filterFactory)
, m_filterSelectorSignalMapper(new QSignalMapper(this))
, m_actionRemoveFilter(new QAction(tr("Remove"), this))
, m_actionAddVirtualMidiInput(new QAction(this))
{
    CommonUi::standardTreeView(this, true);

    setModel(portModel);

    connect(m_filterFactory, &QMidiMessageFilterFactory::modelReset, this, &MidiPortTreeView::onFilterFactoryResetted);
    connect(m_filterFactory, &QMidiMessageFilterFactory::rowsInserted, this, &MidiPortTreeView::onFilterFactoryRowsInserted);
    connect(m_filterSelectorSignalMapper, qOverload<int>(&QSignalMapper::mapped), this, &MidiPortTreeView::onAddFilterActionTriggered);
    connect(m_actionRemoveFilter, &QAction::triggered, this, &MidiPortTreeView::onRemoveFilterActionTriggered);
    connect(m_actionAddVirtualMidiInput, &QAction::triggered, this, &MidiPortTreeView::onAddVirtualMidiInputTriggered);

    setRootIsDecorated(true);
    setExpandsOnDoubleClick(true);
    setItemDelegateForColumn(1, new ValueColumnDelegate(this));
    setSelectionMode(QTreeView::SelectionMode::SingleSelection);
    setSelectionBehavior(QTreeView::SelectionBehavior::SelectRows);

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
    auto const currentIndex = getCurrentFilterIndex(this, m_portModel);

    if (currentIndex.isValid())
    {
        m_portModel->remove(currentIndex);
    }
}

void MidiPortTreeView::onAddVirtualMidiInputTriggered()
{
    m_portModel->add(std::make_shared<QVirtualMidiIn>());
}

void MidiPortTreeView::contextMenuEvent(QContextMenuEvent* event)
{
    // Produce memory leaks on OSX: it seems the cocoa code behind
    // produce a leak repeatedly...
    QMenu menu(this);

    menu.addAction(m_actionAddVirtualMidiInput);

    QMenu* const addFilterSubmenu = menu.addMenu(tr("Add filter"));
    auto const currentCanHaveFilter = getCurrentPortIndex(this, m_portModel).isValid();

    updateActions();

    addFilterSubmenu->setEnabled(!m_actionAddFilters.isEmpty() && currentCanHaveFilter);
    addFilterSubmenu->addActions(m_actionAddFilters);
    menu.addAction(m_actionRemoveFilter);
    menu.exec(event->globalPos());
}

void MidiPortTreeView::updateActions()
{
    auto const filterSelected = m_portModel->getItemType(currentIndex()) == QMidiPortModel::ItemType::Filter;

    m_actionRemoveFilter->setEnabled(filterSelected);
}
