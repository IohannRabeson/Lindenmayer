//
// Created by Io on 03/01/2018.
//

#ifndef MIDIMONITOR_MIDIDELEGATES_HPP
#define MIDIMONITOR_MIDIDELEGATES_HPP
#include <QStyledItemDelegate>
#include <QMidiMessage.hpp>
#include <QMidiDeviceModel.hpp>
#include <QMidiManufacturerModel.hpp>

class QMidiMessageModel;

class MidiMessageTypeDelegate : public QStyledItemDelegate
{
public:
    explicit MidiMessageTypeDelegate(QObject* parent = nullptr);

    QString displayText(QVariant const& value, QLocale const&) const override;
private:
    QMap<QMidiMessage::Type, QString> const m_types =
    {
        {QMidiMessage::Type::NoteOn, QObject::tr("Note On")},
        {QMidiMessage::Type::NoteOff, QObject::tr("Note Off")},
        {QMidiMessage::Type::ControlChange, QObject::tr("Control Change")},
        {QMidiMessage::Type::ProgramChange, QObject::tr("Program Change")},
        {QMidiMessage::Type::SystemExclusive, QObject::tr("System Exclusive")},
        {QMidiMessage::Type::Undefined, QObject::tr("Unknow")}
    };
};

class MidiPortDelegate : public QStyledItemDelegate
{
public:
    explicit MidiPortDelegate(QMidiDeviceModel const* const model, QObject* parent = nullptr);

    QString displayText(QVariant const& value, QLocale const&) const override;
private:
    QMidiDeviceModel const* const m_model;
};

class MidiChannelDelegate : public QStyledItemDelegate
{
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    QString displayText(QVariant const& value, QLocale const&) const override;
};

class MidiTimeDelegate : public QStyledItemDelegate
{
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    QString displayText(QVariant const& value, QLocale const& locale) const override;
};

class MidiValueDelegate : public QStyledItemDelegate
{
    QMidiMessageModel const* const m_model;
    QMidiManufacturerModel const* const m_manufacturerModel;
public:
    MidiValueDelegate(QMidiMessageModel const* const model, QMidiManufacturerModel const* const manufacturerModel, QObject* parent);

    void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const override;
};

class MidiDataDelegate : public QStyledItemDelegate
{
    QMidiMessageModel const* const m_model;
public:
    MidiDataDelegate(QMidiMessageModel const* const model, QObject* parent);

    void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const override;
};

#endif //MIDIMONITOR_MIDIDELEGATES_HPP
