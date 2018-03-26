//
// Created by Io on 08/03/2018.
//

#include "QMidiMessageFilterFactory.hpp"
#include <QtDebug>

int QMidiMessageFilterFactory::rowCount(QModelIndex const& parent) const
{
    return parent.isValid() ? 0 : static_cast<int>(m_creators.size());
}

QVariant QMidiMessageFilterFactory::data(const QModelIndex& index, int role) const
{
    QVariant result;

    if (index.isValid() && role == Qt::DisplayRole)
    {
        result = getCreator(index.row()).label;
    }
    return result;
}

void QMidiMessageFilterFactory::add(QString const& label, QMidiMessageFilterFactory::Creator&& creator)
{
    Q_ASSERT(!label.trimmed().isEmpty());

    if (std::find_if(std::begin(m_creators), std::end(m_creators), [label = label](CreatorEntry const& entry)
    {
        return entry.label == label;
    }) != std::end(m_creators))
    {
        qWarning() << "[QMidiMessageFilterFactory] Duplicated label:" << label;
        return;
    }

    auto const newRow = rowCount();

    beginInsertRows(QModelIndex(), newRow, newRow);
    m_creators.emplace_back(std::move(creator), label);
    endInsertRows();
}

QMidiMessageFilterFactory::Pointer QMidiMessageFilterFactory::create(QModelIndex const& index) const
{
    Pointer result;

    if (index.isValid())
    {
        auto const& entry = getCreator(index.row());

        result = entry.creator(entry.label);
    }
    return result;
}

QString QMidiMessageFilterFactory::getLabel(QModelIndex const& index) const
{
    QString result;

    if (index.isValid())
    {
        auto const& entry = getCreator(index.row());

        result = entry.label;
    }
    return result;
}

QMidiMessageFilterFactory::CreatorEntry::CreatorEntry(QMidiMessageFilterFactory::Creator&& creator, QString const& label)
: creator(std::move(creator))
, label(label)
{
}
