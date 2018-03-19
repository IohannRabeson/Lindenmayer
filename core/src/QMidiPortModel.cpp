//
// Created by Io on 03/03/2018.
//

#include "QMidiPortModel.hpp"
#include "VectorHelpers.hpp"
#include "MidiPortModelPrivate.hpp"

QMidiPortModel::QMidiPortModel(QObject* parent)
: QAbstractItemModel(parent)
{
    m_root.reset(new RootTreeNode);
}

int QMidiPortModel::rowCount(QModelIndex const& parent) const
{
    return getNode(parent)->childrenCount();
}

int QMidiPortModel::columnCount(QModelIndex const& parent) const
{
    return getNode(parent)->columnCount();
}

QVariant QMidiPortModel::data(QModelIndex const& index, int role) const
{
    QVariant result;

    if (index.isValid())
    {
        auto const node = getNode(index);

        result = node->data(index.column(), role);
    }
    return result;
}

void QMidiPortModel::clear()
{
    beginResetModel();
    m_root->destroyChildren();
    endResetModel();
}

QVariant QMidiPortModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    static QStringList const ColumnLabels =
    {
        tr("Name"),
        tr("Value")
    };

    QVariant result;

    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        result = ColumnLabels.value(section);
    }
    return result;
}

void QMidiPortModel::reset(Loader&& loader)
{
    beginResetModel();
    m_root->destroyChildren();
    loader(m_root);
    endResetModel();
}

void QMidiPortModel::reset(std::vector<std::shared_ptr<QAbstractMidiIn>> const& midiIns)
{
    reset([&midiIns = midiIns](std::shared_ptr<RootTreeNode>& root)
          {
              for (auto i = 0u; i < midiIns.size(); ++i)
              {
                  auto node = std::make_shared<MidiInputPortTreeNode>(midiIns[i]);

                  root->addChild(node);
              };
          });
}

void QMidiPortModel::reset(std::vector<std::shared_ptr<QAbstractMidiOut>> const& midiOuts)
{
    reset([&midiOuts = midiOuts](std::shared_ptr<RootTreeNode>& root)
          {
              for (auto i = 0u; i < midiOuts.size(); ++i)
              {
                  auto node = std::make_shared<MidiOutputPortTreeNode>(midiOuts[i]);

                  root->addChild(node);
              };
          });
}

QModelIndex QMidiPortModel::add(std::shared_ptr<QAbstractMidiIn> const& port)
{
    Q_ASSERT( port != nullptr );

    auto const newRow = m_root->childrenCount();
    auto const node = std::make_shared<MidiInputPortTreeNode>(port);

    beginInsertRows(QModelIndex(), newRow, newRow);
    m_root->addChild(node);
    endInsertRows();
    return index(newRow, 0);
}

QModelIndex QMidiPortModel::add(std::shared_ptr<QAbstractMidiOut> const& port)
{
    Q_ASSERT( port != nullptr );

    auto const newRow = m_root->childrenCount();
    auto const node = std::make_shared<MidiOutputPortTreeNode>(port);

    beginInsertRows(QModelIndex(), newRow, newRow);
    m_root->addChild(node);
    endInsertRows();
    return index(newRow, 0);
}

QModelIndex QMidiPortModel::add(QModelIndex const& portIndex, std::shared_ptr<QAbstractMidiMessageFilter> const& filter)
{
    Q_ASSERT( filter != nullptr );

    QModelIndex result;

    if (portIndex.isValid())
    {
        auto const node = getNode(portIndex);
        auto const newRow = node->childrenCount();
        auto const filterNode = std::make_shared<MidiFilterTreeNode>(filter);

        beginInsertRows(portIndex, newRow, newRow);

        node->addChild(filterNode);

        for (auto i = 0u; i < filterNode->getFilter()->parameterCount(); ++i)
        {
            auto const paramNode = std::make_shared<MidiFilterPropertyTreeNode>(filter, i);

            filterNode->addChild(paramNode);
        }

        endInsertRows();
        result = index(newRow, 0, portIndex);
    }
    return result;
}

void QMidiPortModel::remove(QModelIndex const& index)
{
    if (!index.isValid())
    {
        return;
    }

    auto const parentIndex = parent(index);
    auto const parentNode = getNode(parentIndex);
    auto const nodeToRemove = getNode(index);
    auto const rowIndex = nodeToRemove->childIndex();

    if (rowIndex != -1)
    {
        beginRemoveRows(parentIndex, rowIndex, rowIndex);
        parentNode->removeChild(nodeToRemove->childIndex());
        endRemoveRows();
    }
}

Qt::ItemFlags QMidiPortModel::flags(QModelIndex const& index) const
{
    Qt::ItemFlags result = Qt::NoItemFlags;

    if (index.isValid())
    {
        auto const node = getNode(index);

        if (index.column() < node->columnCount())
        {
            result = node->flags(index.column());
        }
    }
    return result;
}

QString QMidiPortModel::getPortName(int const row) const
{
    auto const rowIndex = index(row, 0);
    QString result;

    if (rowIndex.isValid())
    {
        auto const node = getNode(rowIndex);

        switch (node->type())
        {
            case AbstractTreeNode::Type::InputPort:
                result = getPortName(std::static_pointer_cast<MidiInputPortTreeNode>(node));
                break;
            case AbstractTreeNode::Type::OutputPort:
                result = getPortName(std::static_pointer_cast<MidiOutputPortTreeNode>(node));
                break;
            default:
                break;
        }
    }

    return result;
}

std::shared_ptr<QAbstractMidiIn> QMidiPortModel::getInputPort(QModelIndex const& index) const
{
    std::shared_ptr<QAbstractMidiIn> port;

    if (getItemType(index) == ItemType::InputPort)
    {
        auto const node = getNode(index);
        auto const portNode = std::static_pointer_cast<MidiInputPortTreeNode>(node);

        port = portNode->port();
    }

    return port;
}

std::shared_ptr<QAbstractMidiOut> QMidiPortModel::getOutputPort(QModelIndex const& index) const
{
    std::shared_ptr<QAbstractMidiOut> port;

    if (getItemType(index) == ItemType::OutputPort)
    {
        auto const node = getNode(index);
        auto const portNode = std::static_pointer_cast<MidiOutputPortTreeNode>(node);

        port = portNode->port();
    }

    return port;
}

bool QMidiPortModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    bool result = false;

    if (index.isValid())
    {
        auto const node = getNode(index);

        if (node && node->setData(index.column(), value, role))
        {
            emit dataChanged(index, index);
            result = true;
        }
    }
    return result;
}

QModelIndex QMidiPortModel::index(int row, int column, QModelIndex const& parent) const
{
    AbstractTreeNode* parentNode = nullptr;
    QModelIndex finalIndex;

    if (row > -1 && column > -1)
    {
        if (parent.isValid())
        {
            parentNode = static_cast<AbstractTreeNode*>(parent.internalPointer());
        }
        else
        {
            parentNode = m_root.get();
        }

        if (row < parentNode->childrenCount())
        {
            auto const& currentNode = parentNode->child(row);

            if (currentNode)
            {
                finalIndex = createIndex(row, column, currentNode.get());
            }
        }
    }
    return finalIndex;
}

QModelIndex QMidiPortModel::parent(const QModelIndex& child) const
{
    QModelIndex parentIndex;
    auto* const childNode = static_cast<AbstractTreeNode*>(child.internalPointer());

    if (childNode)
    {
        auto const parentNode = childNode->parent();

        if (parentNode && parentNode != m_root)
        {
            parentIndex = createIndex(parentNode->childIndex(), 0, parentNode.get());
        }
    }
    return parentIndex;
}

auto QMidiPortModel::getNode(QModelIndex const& index) const -> std::shared_ptr<AbstractTreeNode>
{
    if (index.isValid())
    {
        auto* const rawNode = static_cast<AbstractTreeNode*>(index.internalPointer());

        return rawNode->shared_from_this();
    }
    return m_root;
}

template <class Node>
QString QMidiPortModel::getPortName(std::shared_ptr<Node> const& node)
{
    return node->port()->portName();
}

QMidiPortModel::ItemType QMidiPortModel::getItemType(QModelIndex const& index) const
{
    return getNode(index)->type();
}

bool QMidiPortModel::isRemovable(QModelIndex const& index) const
{
    return getNode(index)->isRemovable();
}
