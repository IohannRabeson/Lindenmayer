//
// Created by Io on 03/03/2018.
//

#ifndef MIDIMONITOR_QMIDIPORTMODEL_HPP
#define MIDIMONITOR_QMIDIPORTMODEL_HPP
#include "QAbstractMidiIn.hpp"
#include "QAbstractMidiOut.hpp"
#include <QAbstractItemModel>

class QMidiPortModel : public QAbstractItemModel
{
    Q_OBJECT

    class AbstractTreeNode;
    class RootTreeNode;
    class MidiInputPortTreeNode;
    class MidiOutputPortTreeNode;
    class MidiFilterTreeNode;
public:
    enum Roles
    {
        Name = Qt::DisplayRole,
        Index = Qt::UserRole,
        Checked = Qt::CheckStateRole
    };

    enum class ItemType
    {
        Invalid,
        InputPort,
        OutputPort,
        Filter
    };

    using Loader = std::function<void(std::shared_ptr<RootTreeNode>&)>;

    explicit QMidiPortModel(QObject* parent = nullptr);

    void reset(Loader&& loader);
    void reset(std::vector<std::shared_ptr<QAbstractMidiIn>> const& midiIns);
    void reset(std::vector<std::shared_ptr<QAbstractMidiOut>> const& midiOuts);
    QModelIndex add(std::shared_ptr<QAbstractMidiIn> const& port);
    QModelIndex add(std::shared_ptr<QAbstractMidiOut> const& port);
    QModelIndex add(QModelIndex const& portIndex, std::shared_ptr<QAbstractMidiMessageFilter> const& filter);
    void remove(QModelIndex const& index);
    void clear();

    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    Qt::ItemFlags flags(QModelIndex const& index) const override;
    int rowCount(QModelIndex const& parent = QModelIndex()) const override;
    int columnCount(QModelIndex const& parent = QModelIndex()) const override;
    QModelIndex index(int row, int column, QModelIndex const& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& child) const override;
    ItemType getItemType(QModelIndex const& index) const;

    QString getPortName(int const row) const;
private:
    std::shared_ptr<AbstractTreeNode> getNode(QModelIndex const& index) const;

    template <class Node>
    static void addPortFilter(std::shared_ptr<Node> const& node, std::shared_ptr<QAbstractMidiMessageFilter> const& filter);

    template <class Node>
    static QString getPortName(std::shared_ptr<Node> const& node);
private:
    std::shared_ptr<RootTreeNode> m_root;
};

#endif //MIDIMONITOR_QMIDIINLISTMODEL_HPP
