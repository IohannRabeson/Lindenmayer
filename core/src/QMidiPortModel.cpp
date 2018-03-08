//
// Created by Io on 03/03/2018.
//

#include "QMidiPortModel.hpp"

/*!
 * \class QMidiInListModel::AbstractTreeNode
 * \brief Abstract node
 */
class QMidiPortModel::AbstractTreeNode : public std::enable_shared_from_this<AbstractTreeNode>
{
public:
    using NodePtr = std::shared_ptr<AbstractTreeNode>;
    using ParentNode = std::weak_ptr<AbstractTreeNode>;
    using Type = QMidiPortModel::ItemType;

    virtual ~AbstractTreeNode() = default;

    virtual Type type() const = 0;
    virtual QVariant data(int const role) const = 0;
    virtual bool setData(QVariant const& value, int const role) = 0;
    virtual Qt::ItemFlags flags(int const column) const = 0;
    virtual int columnCount() const = 0;

    NodePtr const& child(int const index) const
    {
        Q_ASSERT(index > -1);
        Q_ASSERT(index < m_children.size());

        return m_children[index];
    }

    NodePtr parent() const
    {
        return m_parent.lock();
    }

    void setParent(NodePtr const& parent)
    {
        if (parent.get() != this && m_parent.lock() != parent)
        {
            int newChildIndex = -1;

            auto const oldParentNode = m_parent.lock();

            if (oldParentNode)
            {
                oldParentNode->m_children.erase(oldParentNode->m_children.begin() + m_childIndex);
            }

            if (parent)
            {
                auto& newParentChildren = parent->m_children;

                Q_ASSERT (std::find(newParentChildren.begin(), newParentChildren.end(), shared_from_this()) == newParentChildren.end());

                newChildIndex = newParentChildren.size();

                newParentChildren.push_back(shared_from_this());
            }
            m_childIndex = newChildIndex;
            m_parent = parent;
        }

        // Ensure m_childIndex is set to -1 if the parent is null or
        // ensure m_childIndex is not set to -1 if the parent is not null.
        Q_ASSERT( (m_parent.lock() == nullptr && m_childIndex == -1) ||
                  (m_parent.lock() != nullptr && m_childIndex > -1 && m_childIndex < m_parent.lock()->childrenCount()) );
    }

    void destroyChildren()
    {
        m_children.clear();
    }

    int childIndex() const
    {
        return m_childIndex;
    }

    int childrenCount() const
    {
        return m_children.size();
    }
private:
    std::vector<NodePtr> m_children;
    ParentNode m_parent;
    int m_childIndex = -1;
};

/*!
 * \class QMidiInListModel::RootTreeNode
 * \brief Root node
 */
class QMidiPortModel::RootTreeNode : public AbstractTreeNode
{
public:
    AbstractTreeNode::Type type() const override { return AbstractTreeNode::Type::Invalid; }
    QVariant data(int const role) const override { return QVariant(); }
    bool setData(QVariant const& value, int const role) override { return false; }
    Qt::ItemFlags flags(int const) const override { return Qt::NoItemFlags; }
    int columnCount() const override { return 1; }

    std::shared_ptr<MidiInputPortTreeNode> add(std::shared_ptr<QAbstractMidiIn> const& port);
    std::shared_ptr<MidiOutputPortTreeNode> add(std::shared_ptr<QAbstractMidiOut> const& port);
};

/*!
 * \class QMidiInListModel::MidiPortTreeNode
 * \brief Midi in port node
 */
class QMidiPortModel::MidiInputPortTreeNode : public AbstractTreeNode
{
public:
    explicit MidiInputPortTreeNode(std::shared_ptr<QAbstractMidiIn> const& port)
    : m_port(port)
    {
        Q_ASSERT(m_port != nullptr);
    }

    AbstractTreeNode::Type type() const override { return AbstractTreeNode::Type::InputPort; }

    QString portName() const
    {
        return m_port->portName();
    }

    int columnCount() const override { return 1; }

    Qt::ItemFlags flags(int const) const override
    {
        return Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
    }

    QVariant data(int const role) const override
    {
        QVariant result;

        switch (role)
        {
            case Roles::Name:
                result = portName();
                break;
            case Roles::Index:
                result = m_port->portOpened();
                break;
            case Roles::Checked:
                result = m_port->isPortEnabled() ? Qt::Checked : Qt::Unchecked;
                break;
            default:
                break;
        }
        return result;
    }

    bool setData(QVariant const& value, int const role) override
    {
        bool result = false;

        switch (role)
        {
            case Qt::CheckStateRole:
            {
                auto const checked = (value.value<Qt::CheckState>() == Qt::Checked);

                if (checked != m_port->isPortEnabled())
                {
                    m_port->setPortEnabled(checked);
                    result = true;
                }
            }
            default:
                break;
        }
        return result;
    }

    std::shared_ptr<MidiFilterTreeNode> add(std::shared_ptr<QAbstractMidiMessageFilter> const& filter);
private:
    std::shared_ptr<QAbstractMidiIn> const m_port;
};

/*!
 * \class QMidiInListModel::MidiPortTreeNode
 * \brief Midi in port node
 */
class QMidiPortModel::MidiOutputPortTreeNode : public AbstractTreeNode
{
public:
    explicit MidiOutputPortTreeNode(std::shared_ptr<QAbstractMidiOut> const& port)
    : m_port(port)
    {
        Q_ASSERT(m_port != nullptr);
    }

    AbstractTreeNode::Type type() const override { return AbstractTreeNode::Type::OutputPort; }

    QString portName() const
    {
        return m_port->portName();
    }

    int columnCount() const override { return 1; }

    Qt::ItemFlags flags(int const) const override
    {
        return Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
    }

    QVariant data(int const role) const override
    {
        QVariant result;

        switch (role)
        {
            case Roles::Name:
                result = portName();
                break;
            case Roles::Index:
                result = m_port->portOpened();
                break;
            case Roles::Checked:
                result = m_port->isPortEnabled() ? Qt::Checked : Qt::Unchecked;
                break;
            default:
                break;
        }
        return result;
    }

    bool setData(QVariant const& value, int const role) override
    {
        bool result = false;

        switch (role)
        {
            case Qt::CheckStateRole:
            {
                auto const checked = (value.value<Qt::CheckState>() == Qt::Checked);

                if (checked != m_port->isPortEnabled())
                {
                    m_port->setPortEnabled(checked);
                    result = true;
                }
            }
            default:
                break;
        }
        return result;
    }

    std::shared_ptr<MidiFilterTreeNode> add(std::shared_ptr<QAbstractMidiMessageFilter> const& filter);
private:
    std::shared_ptr<QAbstractMidiOut> const m_port;
};

/*!
 * \class QMidiInListModel::MidiFilterTreeNode
 * \brief Filter node
 */
class QMidiPortModel::MidiFilterTreeNode : public AbstractTreeNode
{
public:
    explicit MidiFilterTreeNode(std::shared_ptr<QAbstractMidiMessageFilter> const& filter)
    : m_filter(filter)
    {
    }

    AbstractTreeNode::Type type() const override { return AbstractTreeNode::Type::Filter; }

    int columnCount() const override { return 1; }

    Qt::ItemFlags flags(int const) const override
    {
        return Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
    }

    QVariant data(int const role) const override
    {
        QVariant result;

        switch (role)
        {
            case Roles::Name:
                result = m_filter->name();
                break;
            case Roles::Checked:
                result = m_filter->isEnabled() ? Qt::Checked : Qt::Unchecked;
                break;
            default:
                break;
        }
        return result;
    }

    bool setData(QVariant const& value, int const role) override
    {
        bool result = false;

        switch (role)
        {
            case Qt::CheckStateRole:
            {
                auto const checked = (value.value<Qt::CheckState>() == Qt::Checked);

                if (checked != m_filter->isEnabled())
                {
                    m_filter->setEnabled(checked);
                    result = true;
                }
            }
            default:
                break;
        }
        return result;
    }
private:
    std::shared_ptr<QAbstractMidiMessageFilter> const m_filter;
};

auto QMidiPortModel::RootTreeNode::add(std::shared_ptr<QAbstractMidiIn> const& port) -> std::shared_ptr<MidiInputPortTreeNode>
{
    auto const node = std::make_shared<MidiInputPortTreeNode>(port);

    node->setParent(shared_from_this());
    return node;
}

auto QMidiPortModel::RootTreeNode::add(std::shared_ptr<QAbstractMidiOut> const& port) -> std::shared_ptr<MidiOutputPortTreeNode>
{
    auto const node = std::make_shared<MidiOutputPortTreeNode>(port);

    node->setParent(shared_from_this());
    return node;
}

auto QMidiPortModel::MidiInputPortTreeNode::add(std::shared_ptr<QAbstractMidiMessageFilter> const& filter) -> std::shared_ptr<MidiFilterTreeNode>
{
    auto const node = std::make_shared<MidiFilterTreeNode>(filter);

    node->setParent(shared_from_this());
    return node;
}

auto QMidiPortModel::MidiOutputPortTreeNode::add(std::shared_ptr<QAbstractMidiMessageFilter> const& filter) -> std::shared_ptr<MidiFilterTreeNode>
{
    auto const node = std::make_shared<MidiFilterTreeNode>(filter);

    node->setParent(shared_from_this());
    return node;
}

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

        if (index.column() < node->columnCount())
        {
            result = node->data(role);
        }
    }
    return result;
}

void QMidiPortModel::clear()
{
    beginResetModel();
    m_root->destroyChildren();
    endResetModel();
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
              for (auto i = 0; i < midiIns.size(); ++i)
              {
                  auto node = std::make_shared<MidiInputPortTreeNode>(midiIns[i]);

                  node->setParent(root);
              };
          });
}

void QMidiPortModel::reset(std::vector<std::shared_ptr<QAbstractMidiOut>> const& midiOuts)
{
    reset([&midiOuts = midiOuts](std::shared_ptr<RootTreeNode>& root)
          {
              for (auto i = 0; i < midiOuts.size(); ++i)
              {
                  auto node = std::make_shared<MidiOutputPortTreeNode>(midiOuts[i]);

                  node->setParent(root);
              };
          });
}

QModelIndex QMidiPortModel::add(std::shared_ptr<QAbstractMidiIn> const& port)
{
    Q_ASSERT( port != nullptr );
    Q_ASSERT( port->isPortOpen() );

    auto const newRow = m_root->childrenCount();

    beginInsertRows(QModelIndex(), newRow, newRow);
    m_root->add(port);
    endInsertRows();
    return index(newRow, 0);
}

QModelIndex QMidiPortModel::add(std::shared_ptr<QAbstractMidiOut> const& port)
{
    Q_ASSERT( port != nullptr );
    Q_ASSERT( port->isPortOpen() );

    auto const newRow = m_root->childrenCount();

    beginInsertRows(QModelIndex(), newRow, newRow);
    m_root->add(port);
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

        switch (node->type())
        {
            case AbstractTreeNode::Type::InputPort:
                beginInsertRows(portIndex, newRow, newRow);
                addPortFilter(std::static_pointer_cast<MidiInputPortTreeNode>(node), filter);
                endInsertRows();
                result = index(newRow, 0, portIndex);
                break;
            case AbstractTreeNode::Type::OutputPort:
                beginInsertRows(portIndex, newRow, newRow);
                addPortFilter(std::static_pointer_cast<MidiOutputPortTreeNode>(node), filter);
                endInsertRows();
                result = index(newRow, 0, portIndex);
                break;
            default:
                break;
        }
    }
    return result;
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

bool QMidiPortModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    bool result = false;

    if (index.isValid())
    {
        auto const node = getNode(index);

        if (node && index.column() < node->columnCount() && node->setData(value, role))
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

        if (parentNode != m_root)
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
    else
    {
        return m_root;
    }
}

template <class Node>
void QMidiPortModel::addPortFilter(std::shared_ptr<Node> const& node, std::shared_ptr<QAbstractMidiMessageFilter> const& filter)
{
    auto const newRow = node->childrenCount();

    node->add(filter);
}

template <class Node>
QString QMidiPortModel::getPortName(std::shared_ptr<Node> const& node)
{
    return node->portName();
}

QMidiPortModel::ItemType QMidiPortModel::getItemType(QModelIndex const& index) const
{
    return getNode(index)->type();
}
