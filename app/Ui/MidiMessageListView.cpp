//
// Created by Io on 31/12/2017.
//

#include "MidiMessageListView.hpp"
#include "CommonUi.hpp"

#include <QMidiMessageModel.hpp>

MidiMessageListView::MidiMessageListView(QMidiMessageModel* model, QWidget* parent)
: QTableView(parent)
, m_autoScroll(true)
{
    static constexpr int const ItemHeight = 18;

    setGridStyle(Qt::NoPen);
    setModel(model);
    CommonUi::standardTableView(this);

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
