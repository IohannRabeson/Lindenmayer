//
// Created by Io on 31/12/2017.
//

#include "MidiMessageListView.hpp"

#include <QMidiMessageModel.hpp>

#include <QHeaderView>

MidiMessageListView::MidiMessageListView(QMidiMessageModel* model, QWidget* parent)
: QTableView(parent)
{
    static constexpr int const ItemHeight = 18;

    setModel(model);
    horizontalHeader()->setStretchLastSection(true);
    verticalHeader()->setVisible(false);
    verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Fixed);
    verticalHeader()->setMaximumSectionSize(ItemHeight);
    verticalHeader()->setDefaultSectionSize(ItemHeight);
}