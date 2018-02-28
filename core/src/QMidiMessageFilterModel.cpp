//
// Created by Io on 18/02/2018.
//

#include <QObject>
#include <QAbstractItemModel>
#include "QMidiMessageFilterModel.hpp"

#include <vector>

class QMidiMessageFilterModelPrivate
{
public:
    QVariant getData(int const row, int const role) const
    {
        Q_ASSERT( row < m_filters.size() );

        auto const& filter = m_filters[row];
        QVariant result;

        if (filter)
        {
            switch (role)
            {
                case Qt::DisplayRole:
                    result = filter->name();
                    break;
                case Qt::CheckStateRole:
                    result = filter->isEnabled() ? Qt::Checked : Qt::Unchecked;
                    break;
                default:
                    break;
            }
        }

        return result;
    }

    bool setData(int const row, int const role, QVariant const& value)
    {
        Q_ASSERT( row < m_filters.size() );

        auto& filter = m_filters[row];
        bool result = false;

        if (filter)
        {
            switch (role)
            {
                case Qt::DisplayRole:
                    filter->setName(value.toString());
                    result = true;
                    break;
                case Qt::CheckStateRole:
                    filter->setEnabled(value.value<Qt::CheckState>() == Qt::Checked);
                    result = true;
                    break;
                default:
                    break;
            }
        }

        return result;
    }

    std::vector<QMidiMessageFilterModel::FilterPointer> m_filters;
};

QMidiMessageFilterModel::QMidiMessageFilterModel(QObject* parent)
: QAbstractListModel(parent)
, d_ptr(new QMidiMessageFilterModelPrivate)
{
}

QMidiMessageFilterModel::~QMidiMessageFilterModel() = default;

QVariant QMidiMessageFilterModel::data(const QModelIndex& index, int role) const
{
    Q_D(const QMidiMessageFilterModel);

    QVariant result;

    if (index.isValid())
    {
        result = d->getData(index.row(), role);
    }
    return result;
}

bool QMidiMessageFilterModel::setData(QModelIndex const& index, QVariant const& value, int role)
{
    Q_D(QMidiMessageFilterModel);
    bool result = false;

    if (index.isValid())
    {
        if (d->setData(index.row(), role, value))
        {
            emit dataChanged(index, index);
            result = true;
        }
    }
    return result;
}

bool QMidiMessageFilterModel::acceptMessage(QMidiMessage const& message) const
{
    Q_D(const QMidiMessageFilterModel);
    auto const& filter = d->m_filters;

    return filter.empty() || std::all_of(filter.begin(), filter.end(), [&message](FilterPointer const& filter)
    {
        return filter->acceptMessage(message);
    });
}

int QMidiMessageFilterModel::rowCount(QModelIndex const& parent) const
{
    Q_D(const QMidiMessageFilterModel);

    return d->m_filters.size();
}

QModelIndex QMidiMessageFilterModel::addFilter(FilterPointer&& filter)
{
    Q_ASSERT(filter != nullptr);

    Q_D(QMidiMessageFilterModel);
    QModelIndex newIndex;

    int const newRowIndex = d->m_filters.size();

    filter->setupWidget();

    beginInsertRows({}, newRowIndex, newRowIndex);
    d->m_filters.emplace_back(std::move(filter));
    endInsertRows();

    newIndex = index(newRowIndex);
    return newIndex;
}

void QMidiMessageFilterModel::removeFilter(QModelIndex const& index)
{
    Q_D(QMidiMessageFilterModel);

    if (index.isValid())
    {
        int const rowIndex = index.row();

        beginRemoveRows({}, rowIndex, rowIndex);
        d->m_filters.erase(d->m_filters.begin() + rowIndex);
        endRemoveRows();
    }
}

void QMidiMessageFilterModel::clear()
{
    Q_D(QMidiMessageFilterModel);

    beginResetModel();
    d->m_filters.clear();
    endResetModel();
}

Qt::ItemFlags QMidiMessageFilterModel::flags(QModelIndex const& index) const
{
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsUserCheckable;
}

std::unique_ptr<QWidget> const& QMidiMessageFilterModel::editorWidget(QModelIndex const& index) const
{
    Q_D(const QMidiMessageFilterModel);

    Q_ASSERT( index.isValid() );
    Q_ASSERT( index.model() == this );

    auto const& filter = d->m_filters[index.row()];

    return filter->getWidget();
}

void QMidiMessageFilterModel::setFilterEnabled(QModelIndex const& index, bool const enabled)
{
    Q_D(QMidiMessageFilterModel);

    Q_ASSERT( index.isValid() );
    Q_ASSERT( index.model() == this );

    auto const& filter = d->m_filters[index.row()];

    filter->setEnabled(enabled);

    dataChanged(index, index);
}

