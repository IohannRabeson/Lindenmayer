//
// Created by Io on 01/01/2018.
//

#include "CommonUi.hpp"

#include <QTableView>
#include <QTreeView>
#include <QHeaderView>
#include <QLayout>

void CommonUi::standardLayout(QLayout* const layout)
{
    layout->setSpacing(3);
    layout->setMargin(6);
}

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
    view->setUniformRowHeights(true);
    view->header()->setStretchLastSection(true);
    view->header()->setVisible(showTopHeader);
    view->setSelectionBehavior(QTableView::SelectionBehavior::SelectRows);
}