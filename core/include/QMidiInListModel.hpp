//
// Created by Io on 03/03/2018.
//

#ifndef MIDIMONITOR_QMIDIINLISTMODEL_HPP
#define MIDIMONITOR_QMIDIINLISTMODEL_HPP
#include "QAbstractMidiIn.hpp"
#include <QAbstractItemModel>

class QMidiInListModel : public QAbstractItemModel
{
    Q_OBJECT

    class AbstractTreeNode;
    class RootTreeNode;
    class MidiInTreeNode;
    class MidiFilterTreeNode;
public:
    enum Roles
    {
        Name = Qt::DisplayRole,
        Index = Qt::UserRole,
        Checked = Qt::CheckStateRole
    };

    using Loader = std::function<void(std::shared_ptr<RootTreeNode>&)>;

    explicit QMidiInListModel(QObject* parent = nullptr);

    void reset(Loader&& loader);
    void reset(std::vector<std::shared_ptr<QAbstractMidiIn>> const& midiIns);
    QModelIndex append(std::shared_ptr<QAbstractMidiIn> const& port);
    void clear();

    QString getPortName(int const row) const;

    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    Qt::ItemFlags flags(QModelIndex const& index) const override;
    int rowCount(QModelIndex const& parent = QModelIndex()) const override;
    int columnCount(QModelIndex const& parent = QModelIndex()) const override;
    QModelIndex index(int row, int column, QModelIndex const& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& child) const override;
private:
    std::shared_ptr<AbstractTreeNode> getNode(QModelIndex const& index) const;
    std::shared_ptr<MidiInTreeNode> getMidiInNode(QModelIndex const& index) const;
private:
    std::shared_ptr<RootTreeNode> m_root;
};

#endif //MIDIMONITOR_QMIDIINLISTMODEL_HPP
