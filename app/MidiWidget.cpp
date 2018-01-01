//
// Created by Io on 31/12/2017.
//

#include "MidiWidget.hpp"
#include "MidiMessageListView.hpp"

#include <QMidiMessageModel.hpp>
#include <QMidiPortModel.hpp>

#include <QVBoxLayout>
#include <QComboBox>

MidiWidget::MidiWidget(QWidget* parent)
: m_model(new QMidiMessageModel(this))
, m_portSelector(new QComboBox(this))
, m_messageList(new MidiMessageListView(m_model, this))
, m_autoScroll(true)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->setMargin(3);
    layout->setSpacing(3);
    layout->addWidget(m_portSelector);
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

    connect(m_portSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, &MidiWidget::onMidiPortChanged);
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

void MidiWidget::setPortModel(QMidiPortModel* midiPortModel)
{
    m_portSelector->setModel(midiPortModel);
}
