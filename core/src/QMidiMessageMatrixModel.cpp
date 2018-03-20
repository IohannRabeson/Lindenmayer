//
// Created by Io on 17/01/2018.
//

#include "QMidiMessageMatrixModel.hpp"
#include <QSize>

namespace imp
{
    QMap<int, QString> removeEntry(QMap<int, QString> const& entries, int first, int last)
    {
        QMap<int, QString> newEntries;

        for (auto it = entries.begin(); it != entries.end(); ++it)
        {
            if (it.key() < first)
            {
                newEntries.insert(it.key(), it.value());
            }
            else if (it.key() > last)
            {
                auto const sub = last - first + 1;

                newEntries.insert(it.key() - sub, it.value());
            }
        }

        return newEntries;
    }
}

int QMidiMessageMatrixModel::rowCount(QModelIndex const& parent) const
{
    return parent.isValid() ? 0 : m_matrix.inputCount();
}

int QMidiMessageMatrixModel::columnCount(QModelIndex const& parent) const
{
    return parent.isValid() ? 0 : m_matrix.outputCount();
}

QVariant QMidiMessageMatrixModel::data(const QModelIndex& index, int role) const
{
    QVariant result;

    if (index.isValid())
    {
        switch (role)
        {
            case Qt::CheckStateRole:
                result = qVariantFromValue(m_matrix.get(index.column(), index.row()) ? Qt::Checked : Qt::Unchecked);
                break;
            case Qt::ToolTipRole:
                result = tr("Connection from input '%0' to output '%1'").arg(m_rowNames.value(index.row())).arg(m_columnNames.value(index.column()));
                break;
            default:
                break;
        }
    }
    return result;
}

bool QMidiMessageMatrixModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    bool result = false;

    if (index.isValid())
    {
        switch (role)
        {
            case Qt::CheckStateRole:
                m_matrix.set(index.column(), index.row(), value.value<Qt::CheckState>() == Qt::Checked);
                emit dataChanged(index, index, QVector<int>({Qt::CheckStateRole}));
                break;
            default:
                break;
        }
    }
    return result;
}

void QMidiMessageMatrixModel::reset(int const columns, int const rows,
                                    QMap<int, QString> const& columnNames,
                                    QMap<int, QString> const& rowNames)
{
    beginResetModel();
    m_matrix.resize(columns, rows);
    m_columnNames = columnNames;
    m_rowNames = rowNames;
    endResetModel();
}

void QMidiMessageMatrixModel::clear()
{
    reset(0, 0, {}, {});
}

Qt::ItemFlags QMidiMessageMatrixModel::flags(const QModelIndex& index) const
{
    return QAbstractTableModel::flags(index) | Qt::ItemIsUserCheckable;
}

QMidiMessageMatrix const& QMidiMessageMatrixModel::matrix() const
{
    return m_matrix;
}

void QMidiMessageMatrixModel::onMidiInputRemoved(QModelIndex const& parent, int first, int last)
{
    if (!parent.isValid())
    {
        beginRemoveRows(QModelIndex(), first, last);
        for (int i = first; i <= last; ++i)
        {
            m_matrix.removeInput(i);
        }

        m_rowNames = imp::removeEntry(m_rowNames, first, last);

        endRemoveRows();
    }
}

void QMidiMessageMatrixModel::onMidiOutputRemoved(QModelIndex const& parent, int first, int last)
{
    if (!parent.isValid())
    {
        beginRemoveColumns(QModelIndex(), first, last);
        for (int i = first; i <= last; ++i)
        {
            m_matrix.removeOutput(i);
        }

        m_columnNames = imp::removeEntry(m_columnNames, first, last);

        endRemoveColumns();
    }
}

QVariant QMidiMessageMatrixModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant result;

    if (orientation == Qt::Vertical)
    {
        switch (role)
        {
            case Qt::DisplayRole:
                result = m_rowNames.value(section);
                break;
            case Qt::ToolTipRole:
                result = tr("Input '%0'").arg(m_rowNames.value(section));
                break;
            default:
                break;
        }
    }
    else if (orientation == Qt::Horizontal)
    {
        switch (role)
        {
            case Qt::DisplayRole:
                result = m_columnNames.value(section);
                break;
            case Qt::ToolTipRole:
                result = tr("Output '%0'").arg(m_columnNames.value(section));
                break;
            default:
                break;
        }
    }
    return result;
}
