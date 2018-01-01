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

    QString getText(int const column, QMidiMessage const& message) const;

    QVector<QMidiMessage> m_messages;
    std::unique_ptr<QAbstractDeviceScheme> m_scheme;
};

QMap<int, QString> const QMidiMessageModelPrivate::s_header =
        {
            {QMidiMessageModel::Columns::Type, "Type"},
            {QMidiMessageModel::Columns::Timestamp, "Timestamp"},
            {QMidiMessageModel::Columns::Data, "Data"}
        };

QMap<QMidiMessage::Type, QString> const QMidiMessageModelPrivate::s_messageTypes =
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

QString QMidiMessageModelPrivate::getText(int const column, QMidiMessage const& message) const
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

                        result = m_scheme->formatControlChangeData(control, value);
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
    Q_D(const QMidiMessageModel);

    QVariant result;

    if (index.isValid())
    {
        switch (role)
        {
        case Qt::DisplayRole:
        case Qt::EditRole:
            result = d->getText(index.column(), d->m_messages[index.row()]);
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
