//
// Created by Io on 09/03/2018.
//

#include "MidiPortModelPrivate.hpp"

//
// class AbstractTreeNode
//

void QMidiPortModel::AbstractTreeNode::setParent(NodePtr const& parent)
{
    if (parent.get() != this && m_parent.lock() != parent)
    {
        int newChildIndex = -1;

        auto const oldParentNode = m_parent.lock();

        if (oldParentNode)
        {
            oldParentNode->onChildRemoved(m_childIndex);

            auto it = oldParentNode->m_children.erase(oldParentNode->m_children.begin() + m_childIndex);

            // Adjust the child index of the remaining children.
            while (it != std::end(oldParentNode->m_children))
            {
                (*it)->m_childIndex -= 1;
                 ++it;
            }
        }

        if (parent)
        {
            auto& newParentChildren = parent->m_children;
            auto const thisNode = shared_from_this();

            Q_ASSERT (std::find(newParentChildren.begin(), newParentChildren.end(), thisNode) == newParentChildren.end());

            newChildIndex = static_cast<int>(newParentChildren.size());
            newParentChildren.push_back(thisNode);
            parent->onChildAdded(newChildIndex);
        }
        m_childIndex = newChildIndex;
        m_parent = parent;
    }

    // Ensure m_childIndex is set to -1 if the parent is null or
    // ensure m_childIndex is not set to -1 if the parent is not null.
    Q_ASSERT( (m_parent.lock() == nullptr && m_childIndex == -1) ||
              (m_parent.lock() != nullptr && m_childIndex > -1 && m_childIndex < m_parent.lock()->childrenCount()) );
}

void QMidiPortModel::AbstractTreeNode::removeChild(int const index)
{
    Q_ASSERT( index > -1 );
    Q_ASSERT( index < static_cast<int>(m_children.size()) );

    auto const child = m_children[static_cast<std::size_t>(index)];

    child->setParent(nullptr);

    imp::removeFromVector(m_children, index);

    for (auto i = 0u; i < m_children.size(); ++i)
    {
        m_children[i]->m_childIndex = static_cast<int>(i);
    }
}

void QMidiPortModel::AbstractTreeNode::destroyChildren()
{
    for (auto const& child : m_children)
    {
        child->setParent(nullptr);
    }
    m_children.clear();
}

int QMidiPortModel::AbstractTreeNode::childIndex() const
{
    return m_childIndex;
}

int QMidiPortModel::AbstractTreeNode::childrenCount() const
{
    return static_cast<int>(m_children.size());
}

auto QMidiPortModel::AbstractTreeNode::parent() const -> NodePtr
{
    return m_parent.lock();
}

auto QMidiPortModel::AbstractTreeNode::child(int const index) const -> NodePtr const&
{
    Q_ASSERT( index > -1 );
    Q_ASSERT( index < static_cast<int>(m_children.size()) );

    return m_children[static_cast<std::size_t>(index)];
}

//
// class MidiInputPortTreeNode
//

QMidiPortModel::MidiInputPortTreeNode::MidiInputPortTreeNode(const std::shared_ptr<QAbstractMidiIn> &port)
    : m_port(port)
{
    Q_ASSERT(m_port != nullptr);
}

QVariant QMidiPortModel::MidiInputPortTreeNode::data(int const column, int const role) const
{
    QVariant result;

    if (column == 0)
    {
        switch (role)
        {
        case Qt::DisplayRole:
            result = portName();
            break;
        case Qt::UserRole:
            result = m_port->portOpened();
            break;
        case Qt::CheckStateRole:
            result = m_port->isPortEnabled() ? Qt::Checked : Qt::Unchecked;
            break;
        default:
            break;
        }
    }

    return result;
}

bool QMidiPortModel::MidiInputPortTreeNode::setData(int const column, QVariant const& value, const int role)
{
    bool result = false;

    if (column == 0)
    {
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
    }

    return result;
}

void QMidiPortModel::MidiInputPortTreeNode::onChildAdded(int const childIndex)
{
    auto const nodeAdded = child(childIndex);
    auto const filterNode = std::dynamic_pointer_cast<MidiFilterTreeNode>(nodeAdded);

    if (filterNode)
    {
        m_port->addFilter(filterNode->getFilter());
    }
}

void QMidiPortModel::MidiInputPortTreeNode::onChildRemoved(int const childIndex)
{
    auto const nodeAdded = child(childIndex);
    auto const filterNode = std::dynamic_pointer_cast<MidiFilterTreeNode>(nodeAdded);

    if (filterNode)
    {
        m_port->removeFilter(filterNode->childIndex());
    }
}

//
// class MidiInputPortTreeNode
//

QVariant QMidiPortModel::MidiOutputPortTreeNode::data(int const column, int const role) const
{
    QVariant result;

    if (column == 0)
    {
        switch (role)
        {
        case Qt::DisplayRole:
            result = portName();
            break;
        case Qt::UserRole:
            result = m_port->portOpened();
            break;
        case Qt::CheckStateRole:
            result = m_port->isPortEnabled() ? Qt::Checked : Qt::Unchecked;
            break;
        default:
            break;
        }
    }

    return result;
}

bool QMidiPortModel::MidiOutputPortTreeNode::setData(int const column, QVariant const& value, int const role)
{
    bool result = false;

    if (column == 0)
    {
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
    }

    return result;
}

void QMidiPortModel::MidiOutputPortTreeNode::onChildAdded(int const childIndex)
{
    auto const nodeAdded = child(childIndex);
    auto const filterNode = std::dynamic_pointer_cast<MidiFilterTreeNode>(nodeAdded);

    if (filterNode)
    {
        m_port->addFilter(filterNode->getFilter());
    }
}

void QMidiPortModel::MidiOutputPortTreeNode::onChildRemoved(int const childIndex)
{
    auto const nodeAdded = child(childIndex);
    auto const filterNode = std::dynamic_pointer_cast<MidiFilterTreeNode>(nodeAdded);

    if (filterNode)
    {
        m_port->removeFilter(filterNode->childIndex());
    }
}

//
// class MidiFilterTreeNode
//

QMidiPortModel::MidiFilterTreeNode::MidiFilterTreeNode(std::shared_ptr<QAbstractMidiMessageFilter> const& filter)
: m_filter(filter)
{
}

Qt::ItemFlags QMidiPortModel::MidiFilterTreeNode::flags(int const column) const
{
    Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;

    if (column == 0)
    {
        flags |= Qt::ItemIsUserCheckable;
    }

    return flags;
}

QVariant QMidiPortModel::MidiFilterTreeNode::data(int const column, int const role) const
{
    QVariant result;

    if (column == 0)
    {
        switch (role)
        {
        case Qt::DisplayRole:
            result = m_filter->name();
            break;
        case Qt::CheckStateRole:
            result = m_filter->isEnabled() ? Qt::Checked : Qt::Unchecked;
            break;
        default:
            break;
        }
    }

    return result;
}

bool QMidiPortModel::MidiFilterTreeNode::setData(int const column, QVariant const& value, int const role)
{
    bool result = false;

    if (column == 0)
    {
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
    }

    return result;
}

//
// class MidiFilterPropertyTreeNode
//

auto QMidiPortModel::MidiFilterPropertyTreeNode::type() const -> Type
{
    return Type::Parameter;
}

QVariant QMidiPortModel::MidiFilterPropertyTreeNode::data(int const column, int const role) const
{
    QVariant result;

    switch (column)
    {
    case Columns::Name:
        switch (role)
        {
        case Qt::DisplayRole:
            result = m_parametrable->name(m_propertyIndex);
            break;
        case Qt::ToolTipRole:
            result = m_parametrable->tooltip(m_propertyIndex);
            break;
        }
        break;
    case Columns::Value:
        if (role == Qt::DisplayRole)
        {
            result = m_parametrable->getValue(m_propertyIndex);
        }
        break;
    default:
        break;
    }

    return result;
}

bool QMidiPortModel::MidiFilterPropertyTreeNode::setData(int const column, QVariant const& value, int const role)
{
    bool result = false;

    if (column == Columns::Value && role == Qt::EditRole)
    {
        result = m_parametrable->setValue(m_propertyIndex, value);
    }
    return result;
}

Qt::ItemFlags QMidiPortModel::MidiFilterPropertyTreeNode::flags(int const column) const
{
    Qt::ItemFlags result = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    if (column == Columns::Value)
    {
        result |= Qt::ItemIsEditable;
    }
    return result;
}

int QMidiPortModel::MidiFilterPropertyTreeNode::columnCount() const
{
    return ColumnCount;
}
