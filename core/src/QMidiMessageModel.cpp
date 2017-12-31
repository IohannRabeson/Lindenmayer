#include "QMidiMessageModel.hpp"
#include <QMetaEnum>

QMap<int, QString> const QMidiMessageModel::s_header =
        {
            {QMidiMessageModel::Columns::Type, "Type"},
            {QMidiMessageModel::Columns::Timestamp, "Timestamp"},
            {QMidiMessageModel::Columns::Data, "Data"}
        };

QMap<QMidiMessage::Type, QString> const QMidiMessageModel::s_messageTypes =
        {
                {QMidiMessage::Type::NoteOn, "Note On"},
                {QMidiMessage::Type::NoteOff, "Note Off"},
                {QMidiMessage::Type::ControlChange, "Control change"},
                {QMidiMessage::Type::ProgramChange, "Program change"},
                {QMidiMessage::Type::SystemExclusive, "Sysex"},
                {QMidiMessage::Type::Undefined, "Undefined"},

        };

QMidiMessageModel::QMidiMessageModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int QMidiMessageModel::append(const QMidiMessage &message)
{
    auto const newRowIndex = m_messages.size();

    beginInsertRows(QModelIndex(), newRowIndex, newRowIndex);
    m_messages.append(message);
    endInsertRows();
    return newRowIndex;
}

int QMidiMessageModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_messages.size();
}

int QMidiMessageModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : Columns::ColumnCount;
}

QString QMidiMessageModel::getText(int const column, QMidiMessage const& message)
{
    static auto const metaEnum = QMetaEnum::fromType<QMidiMessage::Type>();
    QString result;

    switch (column)
    {
        case QMidiMessageModel::Columns::Type:
            result = s_messageTypes.value(message.type());
            break;
        case QMidiMessageModel::Columns::Timestamp:
            result = QMidiMessage::timePointToString(message.timestamp());
            break;
        case QMidiMessageModel::Columns::Data:
            switch (message.type())
            {
                case QMidiMessage::Type::ControlChange:
                {
                    unsigned char control = 0u;
                    unsigned char value = 0u;

                    message.getControlChange(control, value);
                    result = QString("CC%0: %1").arg(control).arg(value);
                }
                    break;
                case QMidiMessage::Type::ProgramChange:
                {
                    unsigned char program = 0u;

                    message.getProgramChange(program);
                    result = QString("Program change: %0").arg(program + 1u);
                }
                    break;
                case QMidiMessage::Type::SystemExclusive:
                    result = message.toString();
                    break;
                default:
                    result = QString("%0 bytes").arg(message.byteCount());
                    break;
            }
            break;
        default:
            break;
    }
    return result;
}

QVariant QMidiMessageModel::data(const QModelIndex &index, int role) const
{
    QVariant result;

    if (index.isValid())
    {
        switch (role)
        {
        case Qt::DisplayRole:
        case Qt::EditRole:
            result = getText(index.column(), m_messages[index.row()]);
            break;
        default:
            break;
        }
    }
    return result;
}

void QMidiMessageModel::clear()
{
    beginResetModel();
    m_messages.clear();
    endResetModel();
}

QVariant QMidiMessageModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant result;

    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        result = s_header.value(section);
    }
    else
    {
        result = QAbstractItemModel::headerData(section, orientation, role);
    }
    return result;
}

QMidiMessage QMidiMessageModel::getMessage(int const row) const
{
    return m_messages.value(row);
}
