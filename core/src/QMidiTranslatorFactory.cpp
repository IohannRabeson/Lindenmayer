//
// Created by Io on 31/12/2017.
//

#include "QMidiTranslatorFactory.hpp"
#include "QAbstractMidiTranslator.hpp"
#include "QDefaultMidiTranslator.hpp"

#include <QtDebug>

QMidiTranslatorFactory::TranslatorCreatorItem::TranslatorCreatorItem(QString const& label, QMidiTranslatorFactory::Creator&& creator)
: creator(std::move(creator))
, label(label)
{
}

QMidiTranslatorFactory::QMidiTranslatorFactory(QObject* parent)
: QAbstractListModel(parent)
{
    add<QDefaultMidiTranslator>("Default", true);
}

QAbstractMidiTranslator* QMidiTranslatorFactory::create(int const key) const
{
    QAbstractMidiTranslator* result = nullptr;

    if (key > -1 && key < m_schemes.size())
    {
        result = m_schemes[key].creator();

        qDebug() << "[QMidiTranslatorFactory] Create scheme" << key << result->getTranslatorName() << result->getTranslatorManufacturer();
    }
    return result;
}

int QMidiTranslatorFactory::rowCount(QModelIndex const& parent) const
{
    return parent.isValid() ? 0 : static_cast<int>(m_schemes.size());
}

QVariant QMidiTranslatorFactory::data(const QModelIndex& index, int role) const
{
    QVariant result;

    if (index.isValid() && role == Qt::DisplayRole)
    {
        result = m_schemes[index.row()].label;
    }
    return result;
}

QAbstractMidiTranslator* QMidiTranslatorFactory::createDefault() const
{
    QAbstractMidiTranslator* result = nullptr;

    if (m_defaultKey > -1 && m_defaultKey < m_schemes.size())
    {
        result = create(m_defaultKey);
    }
    return result;
}

QAbstractMidiTranslator* QMidiTranslatorFactory::create(QModelIndex const& key) const
{
    return key.isValid() ? create(key.row()) : nullptr;
}

QModelIndex QMidiTranslatorFactory::defaultScheme() const
{
    return index(m_defaultKey);
}
