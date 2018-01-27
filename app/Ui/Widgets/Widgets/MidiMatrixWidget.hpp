//
// Created by Io on 27/01/2018.
//

#ifndef MIDIMONITOR_MIDIMATRIXWIDGET_HPP
#define MIDIMONITOR_MIDIMATRIXWIDGET_HPP
#include <QWidget>

class QAbstractTableModel;
class QTableView;

class MidiMatrixWidget : public QWidget
{
public:
    explicit MidiMatrixWidget(QWidget* parent = nullptr);

    void setModel(QAbstractTableModel* model);
private:
    QTableView* const m_table;
};

#endif //MIDIMONITOR_MIDIMATRIXWIDGET_HPP
