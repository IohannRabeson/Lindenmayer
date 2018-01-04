//
// Created by Io on 02/01/2018.
//

#ifndef MIDIMONITOR_MIDIMANUFACTURERMODEL_HPP
#define MIDIMONITOR_MIDIMANUFACTURERMODEL_HPP
#include <QAbstractListModel>
#include <functional>

class QMidiMessage;

class QMidiManufacturerModel : public QAbstractListModel
{
public:
    enum Roles : int
    {
        Name = Qt::DisplayRole,
        Code = Qt::UserRole
    };

    struct Element
    {
        QString name;
        QVector<unsigned char> code;
    };

    struct LoadFromCSV
    {
        explicit LoadFromCSV(QString const& csvFilePath);

        QVector<Element> operator()() const;
    private:
        QString const m_csvFilePath;
    };

    static QVector<Element> loadFromCSV(QString const& csvFilePath);

    using QAbstractListModel::QAbstractListModel;


    void load(std::function<QVector<Element>()>&& loader);
    int rowCount(QModelIndex const& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    int findCode(QMidiMessage const& message) const;
    QString getName(int const row) const;
private:
    QVector<Element> m_elements;
};


#endif //MIDIMONITOR_MIDIMANUFACTURERMODEL_HPP
