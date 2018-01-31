#include "QMidiMessageModel.hpp"
#include "QDefaultMidiTranslator.hpp"

#include <QMap>
#include <QVector>
#include <QMetaEnum>

class QMidiMessageModelPrivate
{
    Q_DECLARE_TR_FUNCTIONS(QMidiMessageModelPrivate)
public:
    static QMap<int, QString> const s_header;
    static QMap<QMidiMessage::Type, QString> const s_messageTypes;

    QVariant getValue(int const column, QMidiMessage const& message) const;

    QVector<QMidiMessage> m_messages;
};

QMap<int, QString> const QMidiMessageModelPrivate::s_header =
{
    {QMidiMessageModel::Columns::Type, tr("Type")},
    {QMidiMessageModel::Columns::Timestamp, tr("Timestamp")},
    {QMidiMessageModel::Columns::Channel, tr("Channel")},
    {QMidiMessageModel::Columns::Value, tr("Value")},
    {QMidiMessageModel::Columns::Data, tr("Data")},
    {QMidiMessageModel::Columns::Input, tr("Input")}
};

QMap<QMidiMessage::Type, QString> const QMidiMessageModelPrivate::s_messageTypes =
{
    {QMidiMessage::Type::NoteOn, tr("Note On")},
    {QMidiMessage::Type::NoteOff, tr("Note Off")},
    {QMidiMessage::Type::ControlChange, tr("CC")},
    {QMidiMessage::Type::ProgramChange, tr("PC")},
    {QMidiMessage::Type::SystemExclusive, tr("SysEx")},
    {QMidiMessage::Type::Undefined, tr("Undefined")},
};

QMidiMessageModel::QMidiMessageModel(QObject *parent)
    : QAbstractTableModel(parent)
    , d_ptr(new QMidiMessageModelPrivate)
{
}

QMidiMessageModel::~QMidiMessageModel() = default;

int QMidiMessageModel::append(const QMidiMessage &message)
{
    Q_D(QMidiMessageModel);

    auto const newRowIndex = d->m_messages.size();

    beginInsertRows(QModelIndex(), newRowIndex, newRowIndex);
    d->m_messages.append(message);
    endInsertRows();
    return newRowIndex;
}

int QMidiMessageModel::rowCount(const QModelIndex &parent) const
{
    Q_D(const QMidiMessageModel);

    return parent.isValid() ? 0 : d->m_messages.size();
}

int QMidiMessageModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : Columns::ColumnCount;
}

QVariant QMidiMessageModelPrivate::getValue(int const column, QMidiMessage const& message) const
{
    QVariant result;

    switch (column)
    {
        case QMidiMessageModel::Columns::Type:
            result = qVariantFromValue(message.type());
            break;
        case QMidiMessageModel::Columns::Timestamp:
            result = qVariantFromValue(message.timestamp());
            break;
        case QMidiMessageModel::Columns::Input:
            result = qVariantFromValue(message.port());
            break;
        case QMidiMessageModel::Columns::Channel:
            result = qVariantFromValue(message.getChannel());
            break;
        case QMidiMessageModel::Columns::Value:
        case QMidiMessageModel::Columns::Data:
            result = qVariantFromValue(message.bytes());
            break;
        default:
            break;
    }
    return result;
}

QVariant QMidiMessageModel::data(const QModelIndex &index, int role) const
{
    Q_D(const QMidiMessageModel);

    QVariant result;

    if (index.isValid())
    {
        switch (role)
        {
        case Qt::DisplayRole:
        case Qt::EditRole:
            result = d->getValue(index.column(), d->m_messages[index.row()]);
            break;
        default:
            break;
        }
    }
    return result;
}

void QMidiMessageModel::clear()
{
    Q_D(QMidiMessageModel);

    beginResetModel();
    d->m_messages.clear();
    endResetModel();
}

QVariant QMidiMessageModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant result;

    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        result = QMidiMessageModelPrivate::s_header.value(section);
    }
    else
    {
        result = QAbstractItemModel::headerData(section, orientation, role);
    }
    return result;
}

QMidiMessage QMidiMessageModel::getMessage(int const row) const
{
    Q_D(const QMidiMessageModel);

    return d->m_messages.value(row);
}

void QMidiMessageModel::remapInputPorts(QMap<int, int>& portRemappings)
{
    Q_D(QMidiMessageModel);

    for (auto& message : d->m_messages)
    {
        message.remapPort(portRemappings);
    }
    auto const topIndex = index(0, Columns::Input);
    auto const bottomIndex = index(d->m_messages.size() - 1, Columns::Input);

    emit dataChanged(topIndex, bottomIndex);
}
