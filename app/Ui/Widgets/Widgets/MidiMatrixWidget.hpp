//
// Created by Io on 27/01/2018.
//

#ifndef MIDIMONITOR_MIDIMATRIXWIDGET_HPP
#define MIDIMONITOR_MIDIMATRIXWIDGET_HPP
#include <QWidget>
#include <QPointer>

class QAbstractTableModel;
class QTableView;

class MidiMatrixWidget : public QWidget
{
    static constexpr int const MatrixCellSize = 32;
public:
    explicit MidiMatrixWidget(QWidget* parent = nullptr);

    void setModel(QAbstractTableModel* model);
private slots:
    void onModelReset();
private:
    QTableView* const m_table;
    QPointer<QAbstractTableModel> m_model;
};

#endif //MIDIMONITOR_MIDIMATRIXWIDGET_HPP
