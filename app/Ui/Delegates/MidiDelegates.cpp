//
// Created by Io on 03/01/2018.
//

#include "MidiDelegates.hpp"
#include <Ui/Format.hpp>

#include <QMetaEnum>
#include <QMidiMessageModel.hpp>

MidiMessageTypeDelegate::MidiMessageTypeDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QString MidiMessageTypeDelegate::displayText(const QVariant &value, const QLocale &) const
{
    QMidiMessage::Type const type = value.value<QMidiMessage::Type>();
    auto const metaEnumIndex = QMidiMessage::staticMetaObject.indexOfEnumerator("QMidiMessage::Type");
    QMetaEnum const metaEnum = QMidiMessage::staticMetaObject.enumerator(metaEnumIndex);

    return m_types.value(type, metaEnum.valueToKey(type));
}

MidiPortDelegate::MidiPortDelegate(const QMidiDeviceModel * const model, QObject *parent)
    : QStyledItemDelegate(parent)
    , m_model(model)
{
}

QString MidiPortDelegate::displayText(const QVariant &value, const QLocale &) const
{
    QString result;
    bool isOk = true;
    auto const portIndex = value.toInt(&isOk);

    if (isOk && portIndex > -1)
    {
        result = m_model->name(portIndex);
    }
    return result;
}

QString MidiChannelDelegate::displayText(const QVariant &value, const QLocale &) const
{
    return QString::number(value.toInt());
}

QString MidiTimeDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    return value.toDateTime().time().toString("HH:mm:ss");
}

MidiValueDelegate::MidiValueDelegate(const QMidiMessageModel * const model, const QMidiManufacturerModel * const manufacturerModel, QObject *parent)
    : QStyledItemDelegate(parent)
    , m_model(model)
    , m_manufacturerModel(manufacturerModel)
{
}

void MidiValueDelegate::initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const
{
    QMidiMessage const message = m_model->getMessage(index.row());
    QString text;

    QStyledItemDelegate::initStyleOption(option, index);
    switch (message.type())
    {
    case QMidiMessage::Type::NoteOn:
        text = QString("%0 Velocity: %1").arg(message.getNote()).arg(message.getVelocity());
        break;
    case QMidiMessage::Type::NoteOff:
        text = QString("%0 Velocity: %1").arg(message.getNote()).arg(message.getVelocity());
        break;
    case QMidiMessage::Type::ControlChange:
        text = QString("CC%0: %1").arg(message.getControlChangeNumber()).arg(message.getControlChangeValue());
        break;
    case QMidiMessage::Type::ProgramChange:
        text = QString("PC: %0").arg(message.getProgramChange());
        break;
    case QMidiMessage::Type::SystemExclusive:
    {
        auto const manufacturerIndex = m_manufacturerModel->findCode(message);

        if (manufacturerIndex != -1)
        {
            text = m_manufacturerModel->getName(manufacturerIndex);
        }
        break;
    }
    default:
        break;
    }
    option->text = text;
}

MidiDataDelegate::MidiDataDelegate(const QMidiMessageModel * const model, QObject *parent)
    : QStyledItemDelegate(parent)
    , m_model(model)
{
}

void MidiDataDelegate::initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const
{
    QMidiMessage const message = m_model->getMessage(index.row());
    QString text;

    QStyledItemDelegate::initStyleOption(option, index);

    option->text = Format::formatBytes(message.bytes());
}
