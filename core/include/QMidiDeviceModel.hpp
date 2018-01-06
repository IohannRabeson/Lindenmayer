//
// Created by Io on 31/12/2017.
//

#ifndef MIDIMONITOR_QMIDIPORTMODEL_HPP
#define MIDIMONITOR_QMIDIPORTMODEL_HPP
#include <QAbstractListModel>

class QMidiIn;
class QMidiOut;

/*!
 * \brief Model of midi ports
 *
 * // TODO: void rescan(QMidiOut* midiOut);
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

    explicit QMidiDeviceModel(QObject* parent);

    void rescan(QMidiIn* midiIn);
    void rescan(QMidiOut* midiOut);
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
    struct MidiPort
    {
        QString name;
        int index = -1;
        bool checked = true;
    };

    using Ports = QVector<MidiPort>;
    using Loader = std::function<Ports()>;

    void reset(Loader&& loader);
    void append(QString const& name, int const index, bool defaultPort = false);
    void clear();
private:
    Ports m_ports;
    int m_defaultPortIndex = -1;
};

#endif //MIDIMONITOR_QMIDIPORTMODEL_HPP
