//
// Created by Io on 31/12/2017.
//

#include "QMidiDeviceModel.hpp"
#include "QMidiIn.hpp"
#include "QMidiOut.hpp"

QMidiDeviceModel::QMidiDeviceModel(QObject* parent)
: QAbstractListModel(parent)
{
}

int QMidiDeviceModel::rowCount(QModelIndex const& parent) const
{
    return parent.isValid() ? 0 : m_ports.size();
}

QVariant QMidiDeviceModel::data(QModelIndex const& index, int role) const
{
    QVariant result;

    if (index.isValid())
    {
        switch (role)
        {
            case Roles::Name:
                result = m_ports[index.row()].name;
                break;
            case Roles::Index:
                result = m_ports[index.row()].index;
                break;
            case Roles::Checked:
                result = m_ports[index.row()].checked ? Qt::Checked : Qt::Unchecked;
                break;
            default:
                break;
        }
    }
    return result;
}

void QMidiDeviceModel::append(QString const& name, int const index, bool const defaultPort)
{
    auto const newRow = m_ports.size();

    beginInsertRows(QModelIndex(), newRow, newRow);
    m_ports.append(MidiPort{name, index});
    endInsertRows();
    if (defaultPort)
    {
        m_defaultPortIndex = newRow;
    }
}

void QMidiDeviceModel::clear()
{
    beginResetModel();
    m_ports.clear();
    endResetModel();
}

void QMidiDeviceModel::reset(Loader&& loader)
{
    beginResetModel();
    m_ports = loader();
    endResetModel();
}

void QMidiDeviceModel::rescan(QMidiIn* const midiIn)
{
    reset([midiIn]() -> Ports
    {
        Ports ports;

        for (auto i = 0; i < midiIn->portCount(); ++i)
        {
            ports.append(MidiPort{midiIn->portName(i), i});
        }
        return ports;
    });
}

void QMidiDeviceModel::rescan(QMidiOut* midiOut)
{
    reset([midiOut]() -> Ports
          {
              Ports ports;

              for (auto i = 0; i < midiOut->portCount(); ++i)
              {
                  ports.append(MidiPort{midiOut->portName(i), i});
              }
              return ports;
          });
}

Qt::ItemFlags QMidiDeviceModel::flags(QModelIndex const& index) const
{
    Qt::ItemFlags result = Qt::NoItemFlags;

    if (index.isValid())
    {
        result = Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
        if (m_ports[index.row()].enabled)
        {
            result |= Qt::ItemIsEnabled;
        }
    }
    return result;
}

void QMidiDeviceModel::setChecked(int const row, bool checked)
{
    auto const currentIndex = index(row, 0);

    if (m_ports[row].checked != checked)
    {
        m_ports[row].checked = checked;
        emit dataChanged(currentIndex, currentIndex);
        emit checkedChanged(row, checked);
    }
}

int QMidiDeviceModel::defaultPort() const
{
    int result = m_defaultPortIndex;

    if (m_defaultPortIndex == -1 && !m_ports.isEmpty())
    {
        result = 0;
    }
    return result;
}

QString QMidiDeviceModel::name(int const row) const
{
    return m_ports.value(row).name;
}

bool QMidiDeviceModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    bool result = false;

    if (index.isValid() && index.column() == 0)
    {
        auto const& port = m_ports[index.row()];

        switch (role)
        {
            case Qt::CheckStateRole:
            {
                auto const checked = value.value<Qt::CheckState>() == Qt::Checked;

                if (checked != port.checked)
                {
                    m_ports[index.row()].checked = checked;
                    result = true;
                    emit dataChanged(index, index, QVector<int>{Qt::CheckStateRole});
                    emit checkedChanged(index.row(), checked);
                }
            }
            default:
                break;
        }
    }
    return result;
}
