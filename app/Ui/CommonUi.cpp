//
// Created by Io on 01/01/2018.
//

#include "CommonUi.hpp"

#include <QTableView>
#include <QTreeView>
#include <QHeaderView>

void CommonUi::standardTableView(QTableView* const view, bool const showTopHeader)
{
    static constexpr int const ItemHeight = 18;

    view->setGridStyle(Qt::NoPen);
    view->horizontalHeader()->setStretchLastSection(true);
    view->horizontalHeader()->setVisible(showTopHeader);
    view->verticalHeader()->setVisible(false);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Fixed);
    view->verticalHeader()->setMaximumSectionSize(ItemHeight);
    view->verticalHeader()->setDefaultSectionSize(ItemHeight);
    view->setSelectionBehavior(QTableView::SelectionBehavior::SelectRows);
}

void CommonUi::standardTreeView(QTreeView* const view, bool const showTopHeader)
{
    static constexpr int const ItemHeight = 18;

    view->header()->setStretchLastSection(true);
    view->header()->setVisible(showTopHeader);
    view->setSelectionBehavior(QTableView::SelectionBehavior::SelectRows);
}