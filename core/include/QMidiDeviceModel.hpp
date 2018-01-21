//
// Created by Io on 31/12/2017.
//

#ifndef MIDIMONITOR_QMIDIPORTMODEL_HPP
#define MIDIMONITOR_QMIDIPORTMODEL_HPP
#include "QAbstractMidiIn.hpp"

#include <QAbstractListModel>

#include <memory>

class QAbstractMidiOut;

/*!
 * \brief Model of midi ports
 *
 */
class QMidiDeviceModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles
    {
        Name = Qt::DisplayRole,
        Index = Qt::UserRole,
        Checked = Qt::CheckStateRole
    };

    struct MidiPort
    {
        QString name;
        int index = -1;
        bool checked = true;
        bool enabled = true;
    };

    using Ports = QVector<MidiPort>;
    using Loader = std::function<Ports()>;

    explicit QMidiDeviceModel(QObject* parent);

    void reset(Loader&& loader);
    int append(QString const& name, bool defaultPort = false);
    void clear();

    void reset(std::vector<std::unique_ptr<QAbstractMidiIn>> const& midiIns);
    void reset(QVector<QAbstractMidiOut*> const& midiOuts);

    void setChecked(int const row, bool checked);

    int rowCount(QModelIndex const& parent = QModelIndex()) const override;
    int defaultPort() const;
    QString name(int const row) const;

    QVariant data(const QModelIndex& index, int role) const override;

    bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    Qt::ItemFlags flags(QModelIndex const& index) const override;
signals:
    void checkedChanged(int const row, bool const checked);
private:
    Ports m_ports;
    int m_defaultPortIndex = -1;
};

#endif //MIDIMONITOR_QMIDIPORTMODEL_HPP
