//
// Created by Io on 03/03/2018.
//

#include "QMidiInListModel.hpp"

/*!
 * \class QMidiInListModel::AbstractTreeNode
 * \brief Abstract node
 */
class QMidiInListModel::AbstractTreeNode : public std::enable_shared_from_this<AbstractTreeNode>
{
public:
    using NodePtr = std::shared_ptr<AbstractTreeNode>;
    using ParentNode = std::weak_ptr<AbstractTreeNode>;

    enum class Type
    {
        Root,
        MidiIn,
        MidiFilter
    };

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
class QMidiInListModel::RootTreeNode : public AbstractTreeNode
{
public:
    AbstractTreeNode::Type type() const override { return AbstractTreeNode::Type::Root; }
    QVariant data(int const role) const override { return QVariant(); }
    bool setData(QVariant const& value, int const role) override { return false; }
    Qt::ItemFlags flags(int const) const override { return Qt::NoItemFlags; }
    int columnCount() const override { return 0; }

    std::shared_ptr<MidiPortTreeNode> add(std::shared_ptr<QAbstractMidiIn> const& port);
};

/*!
 * \class QMidiInListModel::MidiPortTreeNode
 * \brief Midi in port node
 */
class QMidiInListModel::MidiPortTreeNode : public AbstractTreeNode
{
public:
    explicit MidiPortTreeNode(std::shared_ptr<QAbstractMidiIn> const& port)
    : m_port(port)
    {
        Q_ASSERT(m_port != nullptr);
    }

    AbstractTreeNode::Type type() const override { return AbstractTreeNode::Type::MidiIn; }

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
 * \class QMidiInListModel::MidiFilterTreeNode
 * \brief Filter node
 */
class QMidiInListModel::MidiFilterTreeNode : public AbstractTreeNode
{
public:
    explicit MidiFilterTreeNode(std::shared_ptr<QAbstractMidiMessageFilter> const& filter)
    : m_filter(filter)
    {
    }

    AbstractTreeNode::Type type() const override { return AbstractTreeNode::Type::MidiFilter; }

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

std::shared_ptr<QMidiInListModel::MidiPortTreeNode>
QMidiInListModel::RootTreeNode::add(std::shared_ptr<QAbstractMidiIn> const& port)
{
    auto const node = std::make_shared<MidiPortTreeNode>(port);

    node->setParent(shared_from_this());
    return node;
}

std::shared_ptr<QMidiInListModel::MidiFilterTreeNode>
QMidiInListModel::MidiPortTreeNode::add(std::shared_ptr<QAbstractMidiMessageFilter> const& filter)
{
    auto const node = std::make_shared<MidiFilterTreeNode>(filter);

    node->setParent(shared_from_this());
    return node;
}

QMidiInListModel::QMidiInListModel(QObject* parent)
: QAbstractItemModel(parent)
{
    m_root.reset(new RootTreeNode);
}

int QMidiInListModel::rowCount(QModelIndex const& parent) const
{
    return getNode(parent)->childrenCount();
}

int QMidiInListModel::columnCount(QModelIndex const& parent) const
{
    return getNode(parent)->columnCount();
}

QVariant QMidiInListModel::data(QModelIndex const& index, int role) const
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

void QMidiInListModel::clear()
{
    beginResetModel();
    m_root->destroyChildren();
    endResetModel();
}

void QMidiInListModel::reset(Loader&& loader)
{
    beginResetModel();
    m_root.reset(new RootTreeNode);
    loader(m_root);
    endResetModel();
}

void QMidiInListModel::reset(std::vector<std::shared_ptr<QAbstractMidiIn>> const& midiIns)
{
    reset([&midiIns = midiIns](std::shared_ptr<RootTreeNode>& root)
          {
              for (auto i = 0; i < midiIns.size(); ++i)
              {
                  auto node = std::make_shared<MidiPortTreeNode>(midiIns[i]);

                  node->setParent(root);
              };
          });
}

QModelIndex QMidiInListModel::add(std::shared_ptr<QAbstractMidiIn> const& port)
{
    Q_ASSERT( port != nullptr );
    Q_ASSERT( port->isPortOpen() );

    auto const newRow = m_root->childrenCount();

    beginInsertRows(QModelIndex(), newRow, newRow);
    m_root->add(port);
    endInsertRows();
    return index(newRow, 0);
}

QModelIndex QMidiInListModel::add(QModelIndex const& portIndex, std::shared_ptr<QAbstractMidiMessageFilter> const& filter)
{
    Q_ASSERT( filter != nullptr );

    QModelIndex result;

    if (portIndex.isValid())
    {
        auto const portNode = getPortNode(portIndex);

        if (portNode)
        {
            auto const newRow = portNode->childrenCount();

            beginInsertRows(portIndex, newRow, newRow);
            portNode->add(filter);
            endInsertRows();
            result = index(newRow, 0, portIndex);
        }
    }
    return result;
}

Qt::ItemFlags QMidiInListModel::flags(QModelIndex const& index) const
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

QString QMidiInListModel::getPortName(int const row) const
{
    auto const rowIndex = index(row, 0);

    return rowIndex.isValid() ? getPortNode(rowIndex)->portName() : QString();
}

bool QMidiInListModel::setData(const QModelIndex& index, const QVariant& value, int role)
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

QModelIndex QMidiInListModel::index(int row, int column, QModelIndex const& parent) const
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

QModelIndex QMidiInListModel::parent(const QModelIndex& child) const
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

std::shared_ptr<QMidiInListModel::AbstractTreeNode> QMidiInListModel::getNode(QModelIndex const& index) const
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

std::shared_ptr<QMidiInListModel::MidiPortTreeNode> QMidiInListModel::getPortNode(QModelIndex const& index) const
{
    auto const node = getNode(index);

    return (node && node->type() == AbstractTreeNode::Type::MidiIn) ? std::static_pointer_cast<MidiPortTreeNode>(node) : std::shared_ptr<QMidiInListModel::MidiPortTreeNode>();
}
