//
// Created by Io on 08/03/2018.
//

#include "MidiMessageFilterFactory.hpp"
#include <QtDebug>

int MidiMessageFilterFactory::rowCount(QModelIndex const& parent) const
{
    return m_creators.size();
}

QVariant MidiMessageFilterFactory::data(const QModelIndex& index, int role) const
{
    QVariant result;

    if (index.isValid() && role == Qt::DisplayRole)
    {
        result = m_creators[index.row()].label;
    }
    return result;
}

void MidiMessageFilterFactory::add(QString const& label, MidiMessageFilterFactory::Creator&& creator)
{
    Q_ASSERT(!label.trimmed().isEmpty());

    if (std::find_if(std::begin(m_creators), std::end(m_creators), [label = label](CreatorEntry const& entry)
    {
        return entry.label == label;
    }) != std::end(m_creators))
    {
        qWarning() << "[MidiMessageFilterFactory] Duplicated label:" << label;
        return;
    }

    auto const newRow = rowCount();

    beginInsertRows(QModelIndex(), newRow, newRow);
    m_creators.emplace_back(std::move(creator), label);
    endInsertRows();
}

MidiMessageFilterFactory::Pointer MidiMessageFilterFactory::create(QModelIndex const& index) const
{
    Pointer result;

    if (index.isValid())
    {
        auto const& entry = m_creators[index.row()];

        result = entry.creator(entry.label);
    }
    return result;
}

MidiMessageFilterFactory::CreatorEntry::CreatorEntry(MidiMessageFilterFactory::Creator&& creator, QString const& label)
: creator(std::move(creator))
, label(label)
{
}
