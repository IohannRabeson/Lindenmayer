//
// Created by Io on 31/12/2017.
//

#include "MidiMessageListView.hpp"
#include "CommonUi.hpp"

#include <QMidiMessageModel.hpp>
#include <QHeaderView>

#include "Ui/SettingsUtils.hpp"

MidiMessageListView::MidiMessageListView(QMidiMessageModel* model, QWidget* parent)
: QTreeView(parent)
, m_autoScroll(true)
{
    setUniformRowHeights(true);
    setModel(model);
    CommonUi::standardTreeView(this);

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

void MidiMessageListView::loadSettings(QSettings& settings)
{
    settings.beginGroup("message_view");
    restoreGeometry(restoreFrom<QByteArray>(settings, "geometry"));
    settings.beginGroup("header_view");
    header()->restoreGeometry(restoreFrom<QByteArray>(settings, "geometry"));
    header()->restoreState(restoreFrom<QByteArray>(settings, "state"));
    settings.endGroup();
    settings.endGroup();
}
