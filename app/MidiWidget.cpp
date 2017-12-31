//
// Created by Io on 31/12/2017.
//

#include "MidiWidget.hpp"
#include "MessageListView.hpp"

#include <QMidiMessageModel.hpp>

#include <QVBoxLayout>

MidiWidget::MidiWidget(QWidget* parent)
: m_model(new QMidiMessageModel(this))
, m_messageList(new MessageListView(m_model, this))
, m_autoScroll(true)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->setMargin(3);
    layout->addWidget(m_messageList);

    m_messageList->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_messageList->setGridStyle(Qt::NoPen);

    connect(m_model, &QMidiMessageModel::rowsInserted, [this]()
    {
        if (m_autoScroll)
        {
            m_messageList->scrollToBottom();
        }
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
}

void MidiWidget::setAutoScroll(bool const autoscroll)
{
    m_autoScroll = autoscroll;
}

bool MidiWidget::isAutoScrollEnabled() const
{
    return m_autoScroll;
}

void MidiWidget::append(QMidiMessage const& message)
{
    m_model->append(message);
}

void MidiWidget::clear()
{
    m_model->clear();
}