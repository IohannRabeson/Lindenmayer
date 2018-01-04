#include "QMidiMessageModel.hpp"
#include "QDefaultDeviceScheme.hpp"

#include <QMap>
#include <QVector>
#include <QMetaEnum>

class QMidiMessageModelPrivate
{
public:
    static QMap<int, QString> const s_header;
    static QMap<QMidiMessage::Type, QString> const s_messageTypes;

    QVariant getValue(int const column, QMidiMessage const& message) const;

    QVector<QMidiMessage> m_messages;
    std::unique_ptr<QAbstractDeviceScheme> m_scheme;
};

QMap<int, QString> const QMidiMessageModelPrivate::s_header =
        {
            {QMidiMessageModel::Columns::Type, "Type"},
            {QMidiMessageModel::Columns::Timestamp, "Timestamp"},
            {QMidiMessageModel::Columns::Channel, "Channel"},
            {QMidiMessageModel::Columns::Value, "Value"},
            {QMidiMessageModel::Columns::Data, "Data"},
            {QMidiMessageModel::Columns::Input, "Input"}
        };

QMap<QMidiMessage::Type, QString> const QMidiMessageModelPrivate::s_messageTypes =
        {
                {QMidiMessage::Type::NoteOn, "Note On"},
                {QMidiMessage::Type::NoteOff, "Note Off"},
                {QMidiMessage::Type::ControlChange, "CC"},
                {QMidiMessage::Type::ProgramChange, "PC"},
                {QMidiMessage::Type::SystemExclusive, "Sysex"},
                {QMidiMessage::Type::Undefined, "Undefined"},
        };

QMidiMessageModel::QMidiMessageModel(QObject *parent)
    : QAbstractTableModel(parent)
    , d_ptr(new QMidiMessageModelPrivate)
{
    setScheme(new QDefaultDeviceScheme);
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
    static auto const metaEnum = QMetaEnum::fromType<QMidiMessage::Type>();
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

void QMidiMessageModel::setScheme(QAbstractDeviceScheme* scheme)
{
    Q_D(QMidiMessageModel);

    if (d->m_scheme.get() != scheme)
    {
        beginResetModel();
        d->m_scheme.reset(scheme);
        endResetModel();
    }
}

QAbstractDeviceScheme* QMidiMessageModel::getScheme() const
{
    Q_D(const QMidiMessageModel);

    return d->m_scheme.get();
}
