//
// Created by Io on 02/01/2018.
//

#ifndef MIDIMONITOR_MIDIMANUFACTURERMODEL_HPP
#define MIDIMONITOR_MIDIMANUFACTURERMODEL_HPP
#include <QAbstractListModel>
#include <functional>

class QMidiManufacturerModelPrivate;
class QMidiMessage;

class QMidiManufacturerModel : public QAbstractListModel
{
    Q_DECLARE_PRIVATE(QMidiManufacturerModel)
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

    using Loader = std::function<QVector<QMidiManufacturerModel::Element>()>;

    struct LoadFromCSV
    {
        explicit LoadFromCSV(QString const& csvFilePath);

        QVector<Element> operator()() const;
    private:
        QString const m_csvFilePath;
    };

    explicit QMidiManufacturerModel(QObject* parent = nullptr);
    ~QMidiManufacturerModel();

    void load(Loader&& loader);
    int rowCount(QModelIndex const& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    int findCode(QMidiMessage const& message) const;
    QString getName(int const row) const;
private:
    QScopedPointer<QMidiManufacturerModelPrivate> d_ptr;
};

#endif //MIDIMONITOR_MIDIMANUFACTURERMODEL_HPP
