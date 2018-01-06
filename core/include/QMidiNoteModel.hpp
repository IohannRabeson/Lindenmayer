//
// Created by Io on 06/01/2018.
//

#ifndef MIDIMONITOR_QMIDINOTEMODEL_HPP
#define MIDIMONITOR_QMIDINOTEMODEL_HPP
#include <QAbstractListModel>

class QMidiNoteModel : public QAbstractListModel
{
public:
    enum Roles
    {
        Display,
        Data
    };

    using QAbstractListModel::QAbstractListModel;

    int rowCount(QModelIndex const& parent = QModelIndex()) const override;
    QVariant data(QModelIndex const& index, int role) const override;
};

#endif //MIDIMONITOR_QMIDINOTEMODEL_HPP
