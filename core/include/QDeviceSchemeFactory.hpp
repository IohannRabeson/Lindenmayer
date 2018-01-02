//
// Created by Io on 31/12/2017.
//

#ifndef MIDIMONITOR_QDEVICESCHEMEFACTORY_HPP
#define MIDIMONITOR_QDEVICESCHEMEFACTORY_HPP
#include <QAbstractListModel>
#include <functional>
#include <limits>

class QAbstractDeviceScheme;

class QDeviceSchemeFactory : public QAbstractListModel
{
public:
    using Creator = std::function<QAbstractDeviceScheme*()>;

    explicit QDeviceSchemeFactory(QObject* parent = nullptr);

    template <typename T>
    QModelIndex add(QString const& label, bool const isDefault = false)
    {
        static_assert( std::is_base_of<QAbstractDeviceScheme, T>::value );

        int const newId = m_schemes.size();

        beginInsertRows(QModelIndex(), newId, newId);
        m_schemes.emplace_back(label, [](){ return new T;});
        endInsertRows();

        if (isDefault || m_schemes.size() == 1u)
        {
            m_defaultKey = newId;
        }

        return index(newId);
    }

    QAbstractDeviceScheme* createDefault() const;
    QAbstractDeviceScheme* create(int const key) const;
    QAbstractDeviceScheme* create(QModelIndex const& key) const;

    QModelIndex defaultScheme() const;

    int rowCount(QModelIndex const& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
private:
    struct Scheme
    {
        Scheme(QString const& label, Creator&& creator);

        Creator creator;
        QString label;
    };

    std::vector<Scheme> m_schemes;
    int m_defaultKey = -1;
};

#endif //MIDIMONITOR_QDEVICESCHEMEFACTORY_HPP
