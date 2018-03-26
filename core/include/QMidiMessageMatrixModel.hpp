//
// Created by Io on 17/01/2018.
//

#ifndef MIDIMONITOR_QMIDIMESSAGEMATRIXMODEL_HPP
#define MIDIMONITOR_QMIDIMESSAGEMATRIXMODEL_HPP
#include <QAbstractTableModel>
#include "QMidiMessageMatrix.hpp"

/*!
 * \brief The QMidiMessageMatrixModel class
 *
 * The input ports are the rows of the matrix and the output ports are the columns.
 */
class QMidiMessageMatrixModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    using QAbstractTableModel::QAbstractTableModel;

    void reset(int const columns, int const rows, QMap<int, QString> const& columnNames, QMap<int, QString> const& rowNames);
    void connectInputToOutputs(int const row, bool const connected);
    void connectOutputToInputs(int const column, bool const connected);

    void clear();

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex& index, int role) const override;

    int rowCount(QModelIndex const& parent = QModelIndex()) const override;
    int columnCount(QModelIndex const& parent = QModelIndex()) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QMidiMessageMatrix const& matrix() const;

    void onMidiInputRemoved(QModelIndex const& parent, int first, int last);
    void onMidiOutputRemoved(QModelIndex const& parent, int first, int last);
private:
    QMidiMessageMatrix m_matrix;
    QMap<int, QString> m_columnNames;
    QMap<int, QString> m_rowNames;
};

#endif //MIDIMONITOR_QMIDIMESSAGEMATRIXMODEL_HPP