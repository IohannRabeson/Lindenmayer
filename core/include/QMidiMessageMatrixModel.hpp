//
// Created by Io on 17/01/2018.
//

#ifndef MIDIMONITOR_QMIDIMESSAGEMATRIXMODEL_HPP
#define MIDIMONITOR_QMIDIMESSAGEMATRIXMODEL_HPP
#include <QAbstractTableModel>
#include "QMidiMessageMatrix.hpp"

class QMidiMessageMatrixModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    using QAbstractTableModel::QAbstractTableModel;

    void reset(int const columns, int const rows, QMap<int, QString> const& columnNames, QMap<int, QString> const& rowNames);
    void clear();

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex& index, int role) const override;

    int rowCount(QModelIndex const& parent) const override;
    int columnCount(QModelIndex const& parent) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QMidiMessageMatrix const& matrix() const;
private:
    QMidiMessageMatrix m_matrix;
    QMap<int, QString> m_columnNames;
    QMap<int, QString> m_rowNames;
};

#endif //MIDIMONITOR_QMIDIMESSAGEMATRIXMODEL_HPP
