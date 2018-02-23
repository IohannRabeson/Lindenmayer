//
// Created by Io on 18/02/2018.
//

#ifndef MIDIMONITOR_QABSTRACTMIDIMESSAGEFILTERMODEL_HPP
#define MIDIMONITOR_QABSTRACTMIDIMESSAGEFILTERMODEL_HPP
#include "QAbstractMidiMessageFilter.hpp"

#include <QScopedPointer>
#include <QAbstractListModel>

class QMidiMessageFilterModelPrivate;
class QAbstractMidiMessageFilter;

class QMidiMessageFilterModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QMidiMessageFilterModel)
public:
    using FilterPointer = std::unique_ptr<QAbstractMidiMessageFilter>;

    explicit QMidiMessageFilterModel(QObject* parent = nullptr);
    ~QMidiMessageFilterModel();

    Qt::ItemFlags flags(QModelIndex const& index) const override;
    QModelIndex addFilter(FilterPointer&& filter);
    void removeFilter(QModelIndex const& index);
    void clear();

    int rowCount(QModelIndex const& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    bool acceptMessage(QMidiMessage const& message) const;
    std::unique_ptr<QWidget> const& editorWidget(QModelIndex const& index) const;
    void setFilterEnabled(QModelIndex const& index, bool const enabled);
private:
    QScopedPointer<QMidiMessageFilterModelPrivate> d_ptr;
};

#endif //MIDIMONITOR_QABSTRACTMIDIMESSAGEFILTERMODEL_HPP
