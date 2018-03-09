//
// Created by Io on 09/03/2018.
//

#ifndef MIDIMONITOR_MIDIPORTMODELPRIVATE_HPP
#define MIDIMONITOR_MIDIPORTMODELPRIVATE_HPP

#include "QMidiPortModel.hpp"
#include "VectorHelpers.hpp"

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

    void setParent(NodePtr const& parent);
    void removeChild(int const index);
    void destroyChildren();

    int childIndex() const;
    int childrenCount() const;

    NodePtr parent() const;
    NodePtr const& child(int const index) const;
private:
    /*!
     * \brief Method called just after a new child is added.
     * \param childIndex Index of the new child added.
     */
    virtual inline void onChildAdded(int const /*childIndex*/) { }

    /*!
     * \brief Method called just before a child is removed.
     * \param childIndex Index of the removed child.
     */
    virtual inline void onChildRemoved(int const /*childIndex*/) { }
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
    QVariant data(int const /*role*/) const override { return QVariant(); }
    bool setData(QVariant const& /*value*/, int const /*role*/) override { return false; }
    Qt::ItemFlags flags(int const /*column*/) const override { return Qt::NoItemFlags; }
    int columnCount() const override { return 1; }
};

/*!
 * \class QMidiInListModel::MidiPortTreeNode
 * \brief Midi in port node
 */
class QMidiPortModel::MidiInputPortTreeNode : public AbstractTreeNode
{
public:
    explicit MidiInputPortTreeNode(std::shared_ptr<QAbstractMidiIn> const& port);

    AbstractTreeNode::Type type() const override { return AbstractTreeNode::Type::InputPort; }

    QString portName() const { return m_port->portName(); }

    int columnCount() const override { return 1; }

    Qt::ItemFlags flags(int const) const override
    {
        return Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
    }

    QVariant data(int const role) const override;
    bool setData(QVariant const& value, int const role) override;
private:
    virtual void onChildAdded(int const childIndex) override;
    virtual void onChildRemoved(int const childIndex) override;
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

    QVariant data(int const role) const override;
    bool setData(QVariant const& value, int const role) override;
private:
    virtual void onChildAdded(int const childIndex) override;
    virtual void onChildRemoved(int const childIndex) override;
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

    QVariant data(int const role) const override;
    bool setData(QVariant const& value, int const role) override;

    std::shared_ptr<QAbstractMidiMessageFilter> const& getFilter() const { return m_filter; }
private:
    std::shared_ptr<QAbstractMidiMessageFilter> const m_filter;
};

#endif //MIDIMONITOR_MIDIPORTMODELPRIVATE_HPP
