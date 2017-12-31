#ifndef QMIDIMESSAGEMODEL_HPP
#define QMIDIMESSAGEMODEL_HPP

#include <QAbstractTableModel>
#include <QMidiMessage.hpp>
#include "QAbstractMidiScheme.hpp"

class QMidiMessageModel : public QAbstractTableModel
{
    Q_OBJECT

    static QMap<int, QString> const s_header;
    static QMap<QMidiMessage::Type, QString> const s_messageTypes;
public:
    explicit QMidiMessageModel(QObject *parent = nullptr);

    enum Columns
    {
        Timestamp = 0,
        Type,
        Data,
        ColumnCount
    };

    void resetScheme(QAbstractMidiScheme* scheme);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QMidiMessage getMessage(int const row) const;
public slots:
    int append(QMidiMessage const& message);
    void clear();
private:
    QString getText(int const column, QMidiMessage const& message) const;
private:
    QVector<QMidiMessage> m_messages;
    std::unique_ptr<QAbstractMidiScheme> m_scheme;
};

#endif // QMIDIMESSAGEMODEL_HPP
