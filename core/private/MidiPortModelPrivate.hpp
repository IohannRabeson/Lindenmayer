//
// Created by Io on 09/03/2018.
//

#ifndef MIDIMONITOR_MIDIPORTMODELPRIVATE_HPP
#define MIDIMONITOR_MIDIPORTMODELPRIVATE_HPP

#include "QMidiPortModel.hpp"
#include "QAbstractMidiIn.hpp"
#include "QAbstractMidiOut.hpp"
#include "QAbstractMidiMessageFilter.hpp"
#include "VectorHelpers.hpp"

namespace imp
{
    inline Qt::ItemFlags getFirstColumnIsCheckableFlags(int const column)
    {
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | (column == 0 ? Qt::ItemIsUserCheckable : Qt::NoItemFlags);
    }
}

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

    virtual ~AbstractTreeNode();

    virtual Type type() const = 0;
    virtual QVariant data(int const column, int const role) const = 0;
    virtual bool setData(int const column, QVariant const& value, int const role) = 0;
    virtual Qt::ItemFlags flags(int const column) const = 0;
    virtual int columnCount() const = 0;
    virtual bool isRemovable() const = 0;

    int addChild(NodePtr const& node);
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
    virtual inline void onChildAdded(int const childIndex)
    {
        Q_UNUSED( childIndex );
    }

    /*!
     * \brief Method called just before a child is removed.
     * \param childIndex Index of the removed child.
     */
    virtual inline void onChildRemoved(int const childIndex)
    {
        Q_UNUSED( childIndex );
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
    QVariant data(int const /*column*/, int const /*role*/) const override { return QVariant(); }
    bool setData(int const /*column*/, QVariant const& /*value*/, int const /*role*/) override { return false; }
    Qt::ItemFlags flags(int const /*column*/) const override { return Qt::NoItemFlags; }
    int columnCount() const override { return 2; }
    bool isRemovable() const override { return false; }
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

    std::shared_ptr<QAbstractMidiIn> const& port() const { return m_port; }

    int columnCount() const override { return 2; }

    Qt::ItemFlags flags(int const column) const override { return imp::getFirstColumnIsCheckableFlags(column); }

    QVariant data(int const column, int const role) const override;
    bool setData(int const column, QVariant const& value, int const role) override;
    bool isRemovable() const override;
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

    std::shared_ptr<QAbstractMidiOut> const& port() const { return m_port; }

    int columnCount() const override { return 2; }

    Qt::ItemFlags flags(int const column) const override { return imp::getFirstColumnIsCheckableFlags(column); }

    QVariant data(int const column, int const role) const override;
    bool setData(int const column, QVariant const& value, int const role) override;
    bool isRemovable() const override;
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
    explicit MidiFilterTreeNode(std::shared_ptr<QAbstractMidiMessageFilter> const& filter);

    AbstractTreeNode::Type type() const override { return AbstractTreeNode::Type::Filter; }

    int columnCount() const override { return 2; }

    Qt::ItemFlags flags(int const column) const override;

    QVariant data(int const column, int const role) const override;
    bool setData(int const column, QVariant const& value, int const role) override;

    std::shared_ptr<QAbstractMidiMessageFilter> const& getFilter() const { return m_filter; }
    bool isRemovable() const override { return true; }
private:
    std::shared_ptr<QAbstractMidiMessageFilter> const m_filter;
};

class QMidiPortModel::MidiFilterPropertyTreeNode : public AbstractTreeNode
{
    enum Columns
    {
        Name = 0,
        Value,
        ColumnCount
    };
public:
    MidiFilterPropertyTreeNode(std::shared_ptr<Parametrable> const& parametrable, std::size_t const index);

    Type type() const override;
    QVariant data(int const column, const int role) const override;
    bool setData(int const column, const QVariant &value, const int role) override;
    Qt::ItemFlags flags(const int column) const override;
    int columnCount() const override;
    bool isRemovable() const override { return false; }
private:
    std::shared_ptr<Parametrable> const m_parametrable;
    std::size_t const m_propertyIndex;
};

#endif //MIDIMONITOR_MIDIPORTMODELPRIVATE_HPP
