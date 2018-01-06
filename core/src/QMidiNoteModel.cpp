//
// Created by Io on 06/01/2018.
//

#include "QMidiNoteModel.hpp"
#include "Format.hpp"

int QMidiNoteModel::rowCount(QModelIndex const& parent) const
{
    return parent.isValid() ? 0 : 127;
}

QVariant QMidiNoteModel::data(QModelIndex const& index, int role) const
{
    QVariant result;

    if (index.isValid())
    {
        switch (role)
        {
            case Qt::DisplayRole:
            case Qt::EditRole:
                result = Format::formatMidiNote(index.row());
                break;
            default:
                break;
        }
    }
    return result;
}
