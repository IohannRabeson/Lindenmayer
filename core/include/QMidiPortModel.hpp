//
// Created by Io on 31/12/2017.
//

#ifndef MIDIMONITOR_QMIDIPORTMODEL_HPP
#define MIDIMONITOR_QMIDIPORTMODEL_HPP
#include <QAbstractListModel>

class QMidiIn;

/*!
 * \brief Model of midi ports
 *
 * // TODO: void rescan(QMidiOut* midiOut);
 */
class QMidiPortModel : public QAbstractListModel
{
public:
    enum Roles
    {
        Name = Qt::DisplayRole,
        Index = Qt::UserRole,
        Checked = Qt::CheckStateRole
    };

    explicit QMidiPortModel(QObject* parent);

    void rescan(QMidiIn* midiIn);
    void setUsed(int const row, bool used);
    int rowCount(QModelIndex const& parent) const override;
    int defaultPort() const;
    QVariant data(const QModelIndex& index, int role) const override;
    Qt::ItemFlags flags(QModelIndex const& index) const override;
private:
    struct MidiPort
    {
        QString name;
        int index = -1;
        bool used = false;
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
