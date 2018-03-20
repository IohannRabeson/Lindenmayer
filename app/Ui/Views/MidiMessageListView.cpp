//
// Created by Io on 31/12/2017.
//

#include "MidiMessageListView.hpp"

#include "Ui/CommonUi.hpp"
#include "Ui/Delegates/MidiDelegates.hpp"
#include "Ui/SettingsUtils.hpp"

#include <QMidiMessageModel.hpp>
#include <QMidiManager.hpp>

#include <QHeaderView>

MidiMessageListView::MidiMessageListView(QMidiManager* const midiManager, QWidget* parent)
: QTreeView(parent)
, m_midiManager(midiManager)
, m_messageModel(new QMidiMessageModel(this))
, m_autoScroll(true)
{
    setUniformRowHeights(true);
    setModel(m_messageModel);
    setItemDelegateForColumn(QMidiMessageModel::Columns::Type, new MidiMessageDelegates::MidiMessageTypeDelegate(this));
    setItemDelegateForColumn(QMidiMessageModel::Columns::Input, new MidiMessageDelegates::MidiInPortDelegate(m_midiManager->getInputDeviceModel(), this));
    setItemDelegateForColumn(QMidiMessageModel::Columns::Channel, new MidiMessageDelegates::MidiChannelDelegate(this));
    setItemDelegateForColumn(QMidiMessageModel::Columns::Timestamp, new MidiMessageDelegates::MidiTimeDelegate(this));
    setItemDelegateForColumn(QMidiMessageModel::Columns::Value, new MidiMessageDelegates::MidiValueDelegate(m_messageModel, m_midiManager->getManufacturerModel(), this));
    setItemDelegateForColumn(QMidiMessageModel::Columns::Data, new MidiMessageDelegates::MidiDataDelegate(m_messageModel, this));

    CommonUi::standardTreeView(this);

    connect(m_messageModel, &QMidiMessageModel::rowsInserted, [this]()
    {
        if (m_autoScroll)
        {
            scrollToBottom();
        }
    });
}

void MidiMessageListView::append(QMidiMessage const& message)
{
    m_messageModel->append(message);
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

void MidiMessageListView::saveSettings(QSettings& settings) const
{
    settings.beginGroup("message_view");
    settings.setValue("geometry", saveGeometry());
    settings.beginGroup("header_view");
    settings.setValue("geometry", header()->saveGeometry());
    settings.setValue("state", header()->saveState());
    settings.endGroup();
    settings.endGroup();
}
