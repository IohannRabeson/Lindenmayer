//
// Created by Io on 31/12/2017.
//

#include "MidiInputWidget.hpp"
#include "MessageListView.hpp"

#include <QVBoxLayout>

#include <QMidiMessageModel.hpp>
#include <QMidiIn.hpp>
#include <QtDebug>

MidiInputWidget::MidiInputWidget(QWidget* parent)
: QWidget(parent)
, m_midiIn(new QMidiIn(this))
, m_model(new QMidiMessageModel(this))
, m_messageList(new MessageListView(m_model, this))
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->setMargin(3);
    layout->addWidget(m_messageList);

    m_messageList->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_messageList->setGridStyle(Qt::NoPen);

    connect(m_model, &QMidiMessageModel::rowsInserted, [this]()
    {
        m_messageList->scrollToBottom();
    });

    connect(m_messageList, &QTableView::clicked, [this](QModelIndex const& index)
    {
        if (m_model)
        {
            emit messageClicked(m_model->getMessage(index.row()));
        }
    });

    connect(m_messageList, &QTableView::doubleClicked, [this](QModelIndex const& index)
    {
        if (m_model)
        {
            emit messageDoubleClicked(m_model->getMessage(index.row()));
        }
    });

    connect(m_midiIn, &QMidiIn::messageReceived, m_model, &QMidiMessageModel::append);
    connect(m_midiIn, &QMidiIn::error, [](QString const& error)
    {
        qWarning() << error;
    });

    m_midiIn->openPort(0);
}

