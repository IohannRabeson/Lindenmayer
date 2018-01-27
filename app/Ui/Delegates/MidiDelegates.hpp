//
// Created by Io on 03/01/2018.
//

#ifndef MIDIMONITOR_MIDIDELEGATES_HPP
#define MIDIMONITOR_MIDIDELEGATES_HPP
#include <QStyledItemDelegate>
#include <QCoreApplication>

#include <QMidiMessage.hpp>
#include <QMidiDeviceModel.hpp>
#include <QMidiManufacturerModel.hpp>

class QMidiMessageModel;

class MidiMessageTypeDelegate : public QStyledItemDelegate
{
    Q_DECLARE_TR_FUNCTIONS(MidiMessageTypeDelegate)
public:
    explicit MidiMessageTypeDelegate(QObject* parent = nullptr);

    QString displayText(QVariant const& value, QLocale const&) const override;
private:
    QMap<QMidiMessage::Type, QString> const m_types =
    {
        {QMidiMessage::Type::NoteOn, tr("Note On")},
        {QMidiMessage::Type::NoteOff, tr("Note Off")},
        {QMidiMessage::Type::ControlChange, tr("Control Change")},
        {QMidiMessage::Type::ProgramChange, tr("Program Change")},
        {QMidiMessage::Type::SystemExclusive, tr("System Exclusive")},
        {QMidiMessage::Type::Undefined, tr("Unknown")}
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

#include <QMap>
#include <QScopedPointer>
#include <QAbstractMidiTranslator.hpp>
#include <QDefaultMidiTranslator.hpp>

class MidiTranslatorSelector
{
public:
    using TranslatorPointer = std::unique_ptr<QAbstractMidiTranslator>;

    MidiTranslatorSelector()
    : m_defaultTranslator(new QDefaultMidiTranslator)
    {
    }

    ~MidiTranslatorSelector()
    {
        m_translators.clear();
    }

    void addTranslator(int const portIndex, TranslatorPointer&& translator)
    {
        m_translators.emplace(portIndex, std::move(translator));
    }

    QAbstractMidiTranslator& get(QMidiMessage const& message) const
    {
        auto const it = m_translators.find(message.port());

        return it != m_translators.end() ? *(it->second) : *m_defaultTranslator;
    }
private:
    std::map<int, TranslatorPointer> m_translators;
    TranslatorPointer m_defaultTranslator;
};

class MidiValueDelegate : public QStyledItemDelegate
{
    QMidiMessageModel const* const m_model;
    QMidiManufacturerModel const* const m_manufacturerModel;
public:
    MidiValueDelegate(QMidiMessageModel const* const model, QMidiManufacturerModel const* const manufacturerModel, QObject* parent);

    void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const override;
private:
    MidiTranslatorSelector m_translatorSelector;
};

class MidiDataDelegate : public QStyledItemDelegate
{
    QMidiMessageModel const* const m_model;
public:
    MidiDataDelegate(QMidiMessageModel const* const model, QObject* parent);

    void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const override;
};

#endif //MIDIMONITOR_MIDIDELEGATES_HPP
