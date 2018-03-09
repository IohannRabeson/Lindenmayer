//
// Created by Io on 08/03/2018.
//

#include "MidiPortTreeView.hpp"
#include "QMidiMessageFilterFactory.hpp"
#include "QMidiPortModel.hpp"

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
}

MidiPortTreeView::MidiPortTreeView(QMidiPortModel* portModel, QMidiMessageFilterFactory* filterFactory, QWidget* parent)
: QTreeView(parent)
, m_portModel(portModel)
, m_filterFactory(filterFactory)
, m_filterSelectorSignalMapper(new QSignalMapper(this))
, m_actionRemoveFilter(new QAction(tr("Remove"), this))
{
    setModel(portModel);
    connect(m_filterFactory, &QMidiMessageFilterFactory::modelReset, this, &MidiPortTreeView::onFilterFactoryResetted);
    connect(m_filterFactory, &QMidiMessageFilterFactory::rowsInserted, this, &MidiPortTreeView::onFilterFactoryRowsInserted);
    connect(m_filterSelectorSignalMapper, qOverload<int>(&QSignalMapper::mapped), this, &MidiPortTreeView::onAddFilterActionTriggered);
}

void MidiPortTreeView::addCreateFilterAction(int i)
{
    auto const index = m_filterFactory->index(i);
    QString const filterLabel = m_filterFactory->getLabel(index);
    QAction* const action = new QAction(this);

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

        m_portModel->add(currentPortIndex, newFilter);
    }
}

void MidiPortTreeView::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu(this);
    QMenu* const addFilterSubmenu = menu.addMenu(tr("Add filter"));

    updateActions();

    addFilterSubmenu->setEnabled(!m_actionAddFilters.isEmpty());
    addFilterSubmenu->addActions(m_actionAddFilters);
    menu.addAction(m_actionRemoveFilter);
    menu.exec(event->globalPos());
}

void MidiPortTreeView::updateActions()
{
    auto const filterSelected = m_portModel->getItemType(currentIndex()) == QMidiPortModel::ItemType::Filter;

    m_actionRemoveFilter->setEnabled(filterSelected);
}
