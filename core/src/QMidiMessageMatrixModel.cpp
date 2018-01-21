//
// Created by Io on 17/01/2018.
//

#include "QMidiMessageMatrixModel.hpp"
#include <QSize>

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

void QMidiMessageMatrixModel::reset(int const columns, int const rows)
{
    beginResetModel();
    m_matrix.resize(columns, rows);
    endResetModel();
}

void QMidiMessageMatrixModel::clear()
{
    reset(0, 0);
}

Qt::ItemFlags QMidiMessageMatrixModel::flags(const QModelIndex& index) const
{
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsUserCheckable;
}

QMidiMessageMatrix const& QMidiMessageMatrixModel::matrix() const
{
    return m_matrix;
}
