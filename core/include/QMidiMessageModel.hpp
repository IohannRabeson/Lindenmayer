#ifndef QMIDIMESSAGEMODEL_HPP
#define QMIDIMESSAGEMODEL_HPP

#include <QAbstractTableModel>

#include <QMidiMessage.hpp>

#include "QAbstractDeviceScheme.hpp"

class QMidiMessageModelPrivate;

class QMidiMessageModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QMidiMessageModel)
public:
    explicit QMidiMessageModel(QObject *parent = nullptr);
    ~QMidiMessageModel();

    enum Columns
    {
        Type = 0,
        Input,
        Timestamp,
        Channel,
        Value,
        Data,
        ColumnCount
    };

    void setScheme(QAbstractDeviceScheme* scheme);
    QAbstractDeviceScheme* getScheme() const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QMidiMessage getMessage(int const row) const;
public slots:
    int append(QMidiMessage const& message);
    void clear();
private:
    QScopedPointer<QMidiMessageModelPrivate> d_ptr;
};

#endif // QMIDIMESSAGEMODEL_HPP
