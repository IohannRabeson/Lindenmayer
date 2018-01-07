//
// Created by Io on 31/12/2017.
//

#ifndef MIDIMONITOR_QMIDITRANSLATORFACTORY_HPP
#define MIDIMONITOR_QMIDITRANSLATORFACTORY_HPP
#include <QAbstractListModel>
#include <functional>
#include <limits>

class QAbstractMidiTranslator;

class QMidiTranslatorFactory : public QAbstractListModel
{
public:
    using Creator = std::function<QAbstractMidiTranslator*()>;

    explicit QMidiTranslatorFactory(QObject* parent = nullptr);

    template <typename T>
    QModelIndex add(QString const& label, bool const isDefault = false)
    {
        static_assert( std::is_base_of<QAbstractMidiTranslator, T>::value );

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

    QAbstractMidiTranslator* createDefault() const;
    QAbstractMidiTranslator* create(int const key) const;
    QAbstractMidiTranslator* create(QModelIndex const& key) const;

    QModelIndex defaultScheme() const;

    int rowCount(QModelIndex const& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
private:
    struct TranslatorCreatorItem
    {
        TranslatorCreatorItem(QString const& label, Creator&& creator);

        Creator creator;
        QString label;
    };

    std::vector<TranslatorCreatorItem> m_schemes;
    int m_defaultKey = -1;
};

#endif //MIDIMONITOR_QMIDITRANSLATORFACTORY_HPP
