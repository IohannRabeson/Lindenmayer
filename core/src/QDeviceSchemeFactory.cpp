//
// Created by Io on 31/12/2017.
//

#include "QDeviceSchemeFactory.hpp"
#include "QAbstractDeviceScheme.hpp"
#include "QDefaultDeviceScheme.hpp"

#include <QtDebug>

QDeviceSchemeFactory::Scheme::Scheme(QString const& label, QDeviceSchemeFactory::Creator&& creator)
: creator(std::move(creator))
, label(label)
{
}

QDeviceSchemeFactory::QDeviceSchemeFactory(QObject* parent)
: QAbstractListModel(parent)
{
    add<QDefaultDeviceScheme>("Default", true);
}

QAbstractDeviceScheme* QDeviceSchemeFactory::create(int const key) const
{
    QAbstractDeviceScheme* result = nullptr;

    if (key > -1 && key < m_schemes.size())
    {
        result = m_schemes[key].creator();

        qDebug() << "[QDeviceSchemeFactory] Create scheme" << key << result->schemeName() << result->schemeManufacturer();
    }
    return result;
}

int QDeviceSchemeFactory::rowCount(QModelIndex const& parent) const
{
    return parent.isValid() ? 0 : static_cast<int>(m_schemes.size());
}

QVariant QDeviceSchemeFactory::data(const QModelIndex& index, int role) const
{
    QVariant result;

    if (index.isValid() && role == Qt::DisplayRole)
    {
        result = m_schemes[index.row()].label;
    }
    return result;
}

QAbstractDeviceScheme* QDeviceSchemeFactory::createDefault() const
{
    QAbstractDeviceScheme* result = nullptr;

    if (m_defaultKey > -1 && m_defaultKey < m_schemes.size())
    {
        result = create(m_defaultKey);
    }
    return result;
}

QAbstractDeviceScheme* QDeviceSchemeFactory::create(QModelIndex const& key) const
{
    return key.isValid() ? create(key.row()) : nullptr;
}

QModelIndex QDeviceSchemeFactory::defaultScheme() const
{
    return index(m_defaultKey);
}
