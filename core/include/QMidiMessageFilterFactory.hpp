//
// Created by Io on 08/03/2018.
//

#ifndef MIDIMONITOR_MIDIMESSAGEFILTERFACTORY_HPP
#define MIDIMONITOR_MIDIMESSAGEFILTERFACTORY_HPP
#include <QAbstractListModel>
#include <QAbstractMidiMessageFilter.hpp>
#include <functional>
#include <memory>

class QMidiMessageFilterFactory : public QAbstractListModel
{
public:
    using Pointer = std::shared_ptr<QAbstractMidiMessageFilter>;
    using Creator = std::function<Pointer(QString const&)>;

    using QAbstractListModel::QAbstractListModel;

    template <class T>
    void add(QString const& label)
    {
        add(label, [](QString const& label) { return std::make_shared<T>(label); });
    }

    void add(QString const& label, Creator&& creator);
    int rowCount(QModelIndex const& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

    QString getLabel(QModelIndex const& index) const;
    Pointer create(QModelIndex const& index) const;
private:
    struct CreatorEntry
    {
        CreatorEntry(Creator&& creator, QString const& label);

        Creator creator;
        QString label;
    };

    inline CreatorEntry const& getCreator(int const index) const
    {
        Q_ASSERT( index > -1 );
        Q_ASSERT( index < static_cast<int>(m_creators.size()) );

        return m_creators[static_cast<std::size_t>(index)];
    }
private:
    std::vector<CreatorEntry> m_creators;
};


#endif //MIDIMONITOR_MIDIMESSAGEFILTERFACTORY_HPP
