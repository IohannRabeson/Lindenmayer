//
// Created by Io on 31/12/2017.
//

#include "MidiMessageListView.hpp"

#include <QMidiMessageModel.hpp>

#include <QHeaderView>

MidiMessageListView::MidiMessageListView(QMidiMessageModel* model, QWidget* parent)
: QTableView(parent)
, m_autoScroll(true)
{
    static constexpr int const ItemHeight = 18;

    setGridStyle(Qt::NoPen);
    setModel(model);
    horizontalHeader()->setStretchLastSection(true);
    verticalHeader()->setVisible(false);
    verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Fixed);
    verticalHeader()->setMaximumSectionSize(ItemHeight);
    verticalHeader()->setDefaultSectionSize(ItemHeight);
    setSelectionBehavior(QTableView::SelectionBehavior::SelectRows);

    connect(model, &QMidiMessageModel::rowsInserted, [this]()
    {
        if (m_autoScroll)
        {
            scrollToBottom();
        }
    });
}

void MidiMessageListView::setAutoScrollToBottomEnabled(bool const enabled)
{
    m_autoScroll = enabled;
}
