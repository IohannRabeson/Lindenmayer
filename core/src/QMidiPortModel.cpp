//
// Created by Io on 31/12/2017.
//

#include "QMidiPortModel.hpp"
#include "QMidiIn.hpp"

QMidiPortModel::QMidiPortModel(QObject* parent)
: QAbstractListModel(parent)
{
}

int QMidiPortModel::rowCount(QModelIndex const& parent) const
{
    return parent.isValid() ? 0 : m_ports.size();
}

QVariant QMidiPortModel::data(QModelIndex const& index, int role) const
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
                result = m_ports[index.row()].used ? Qt::Checked : Qt::Unchecked;
                break;
            default:
                break;
        }
    }
    return result;
}

void QMidiPortModel::append(QString const& name, int const index, bool const defaultPort)
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

void QMidiPortModel::clear()
{
    beginResetModel();
    m_ports.clear();
    endResetModel();
}

void QMidiPortModel::reset(Loader&& loader)
{
    beginResetModel();
    m_ports = loader();
    endResetModel();
}

void QMidiPortModel::rescan(QMidiIn* midiIn)
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

Qt::ItemFlags QMidiPortModel::flags(QModelIndex const& index) const
{
    Qt::ItemFlags result = Qt::NoItemFlags;

    result = QAbstractListModel::flags(index);
    if (!m_ports[index.row()].used)
    {
        result |= Qt::ItemIsSelectable;
        result |= Qt::ItemIsEnabled;
    }
    return result;
}

void QMidiPortModel::setUsed(int const row, bool used)
{
    auto const currentIndex = index(row, 0);

    if (m_ports[row].used != used)
    {
        m_ports[row].used = used;
        emit dataChanged(currentIndex, currentIndex);
    }
}

int QMidiPortModel::defaultPort() const
{
    int result = m_defaultPortIndex;

    if (m_defaultPortIndex == -1 && !m_ports.isEmpty())
    {
        result = 0;
    }
    return result;
}
