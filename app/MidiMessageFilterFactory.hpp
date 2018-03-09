//
// Created by Io on 08/03/2018.
//

#ifndef MIDIMONITOR_MIDIMESSAGEFILTERFACTORY_HPP
#define MIDIMONITOR_MIDIMESSAGEFILTERFACTORY_HPP
#include <QAbstractListModel>
#include <QAbstractMidiMessageFilter.hpp>
#include <functional>
#include <memory>

class MidiMessageFilterFactory : public QAbstractListModel
{
public:
    using Pointer = std::shared_ptr<QAbstractMidiMessageFilter>;
    using Creator = std::function<Pointer(QString const&)>;

    using QAbstractListModel::QAbstractListModel;

    void add(QString const& label, Creator&& creator);
    int rowCount(QModelIndex const& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

    Pointer create(QModelIndex const& index) const;
private:
    struct CreatorEntry
    {
        CreatorEntry(Creator&& creator, QString const& label);

        Creator creator;
        QString label;
    };

    std::vector<CreatorEntry> m_creators;
};


#endif //MIDIMONITOR_MIDIMESSAGEFILTERFACTORY_HPP
