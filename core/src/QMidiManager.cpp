//
// Created by Io on 04/01/2018.
//

#include "QMidiManager.hpp"
#include "QMidiIn.hpp"
#include "QMidiOut.hpp"
#include "QMidiMessageMatrixModel.hpp"
#include "QMidiMessage.hpp"
#include "QMidiPortModel.hpp"
#include "QMidiMessageFilterFactory.hpp"
#include "QMidiManufacturerModel.hpp"

#include "VectorHelpers.hpp"
#include "RtMidiHelpers.hpp"

#include <QtDebug>
#include <QSize>
#include <QPointer>

#include <vector>

namespace
{
    template <class PortContainer>
    inline QMap<int, QString> extractPortNames(PortContainer const& ports)
    {
        QMap<int, QString> results;

        for (decltype(ports.size()) i = 0; i < ports.size(); ++i)
        {
            results.insert(i, ports[i]->portName());
        }
        return results;
    };
}

class QMidiManagerPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(QMidiManager)
public:
    explicit QMidiManagerPrivate(QMidiManager* q)
    : q_ptr(q)
    , m_inputDeviceModel(new QMidiPortModel(q))
    , m_outputDeviceModel(new QMidiPortModel(q))
    , m_matrixModel(new QMidiMessageMatrixModel(q))
    , m_messageFilterFactory(new QMidiMessageFilterFactory(q))
    , m_manufacturerModel(new QMidiManufacturerModel(q))
    {
        connect(m_inputDeviceModel, &QMidiPortModel::rowsInserted, this, &QMidiManagerPrivate::onMidiInputPortRowInserted);
        connect(m_inputDeviceModel, &QMidiPortModel::modelReset, this, &QMidiManagerPrivate::onMidiInputPortReset);
        connect(m_inputDeviceModel, &QMidiPortModel::modelAboutToBeReset, this, &QMidiManagerPrivate::onMidiInputPortAboutToReset);
        connect(m_inputDeviceModel, &QMidiPortModel::rowsAboutToBeRemoved, this, &QMidiManagerPrivate::onMidiInputPortRowRemoved);

        connect(m_outputDeviceModel, &QMidiPortModel::rowsInserted, this, &QMidiManagerPrivate::onMidiOutputPortRowInserted);
        connect(m_outputDeviceModel, &QMidiPortModel::modelReset, this, &QMidiManagerPrivate::onMidiOutputPortReset);
        connect(m_outputDeviceModel, &QMidiPortModel::modelAboutToBeReset, this, &QMidiManagerPrivate::onMidiOutputPortAboutToReset);
        connect(m_outputDeviceModel, &QMidiPortModel::rowsAboutToBeRemoved, this, &QMidiManagerPrivate::onMidiOutputPortRowRemoved);
    }

    void resetMidiInPorts();
    void resetMidiOutPorts();
    void closeOutputPorts();
    void closeInputPorts();
    void resetPhysicalMidiInPorts();
    void resetPhysicalMidiOutPorts();

    /*!
     * \brief Method called by a MIDI input port when a message is received.
     * \param message Message forwarded through the MIDI message matrix.
     */
    void forwardMidiMessage(QMidiMessage const& message);
private:
    /*! Method called just after an input port is added. */
    int addInputPort(std::shared_ptr<QAbstractMidiIn> const& midiIn);

    /*! Method called just after an output port is added. */
    int addOutputPort(std::shared_ptr<QAbstractMidiOut> const& midiOut);
private slots:
    void onMidiInputPortRowInserted(QModelIndex const& parent, int first, int last);
    void onMidiOutputPortRowInserted(QModelIndex const& parent, int first, int last);
    void onMidiInputPortAboutToReset();
    void onMidiOutputPortAboutToReset();
    void onMidiInputPortReset();
    void onMidiOutputPortReset();
    void onMidiInputPortRowRemoved(QModelIndex const& parent, int first, int last);
    void onMidiOutputPortRowRemoved(QModelIndex const& parent, int first, int last);
private:
    QMidiManager* const q_ptr;
    QPointer<QMidiPortModel> const m_inputDeviceModel;
    QPointer<QMidiPortModel> const m_outputDeviceModel;
    QMidiMessageMatrixModel* const m_matrixModel;
    QMidiMessageFilterFactory* const m_messageFilterFactory;
    QMidiManufacturerModel* const m_manufacturerModel;
    std::vector<std::shared_ptr<QAbstractMidiIn>> m_midiIns;
    std::vector<std::shared_ptr<QAbstractMidiOut>> m_midiOuts;
};

void QMidiManagerPrivate::resetPhysicalMidiInPorts()
{
    // We need one instance of QMidiIn in order to get the port count.
    // This limitation is discused here: https://github.com/thestk/rtmidi/issues/50
    auto firstMidiPort = std::make_shared<QMidiIn>();
    auto const portCount = firstMidiPort->portCount();
    std::vector<std::shared_ptr<QAbstractMidiIn>> ports;

    if (portCount > 0)
    {
        ports.emplace_back(std::move(firstMidiPort));

        // Instanciate midi other inputs
        for (int i = 0; i < portCount - 1; ++i)
        {
            ports.emplace_back(std::make_shared<QMidiIn>());
        }
    }
    m_inputDeviceModel->reset(ports);
}

void QMidiManagerPrivate::resetMidiInPorts()
{
    // Remove all inputs
    closeInputPorts();

    // Poll each physical midi in ports
    resetPhysicalMidiInPorts();

    if (m_midiIns.empty())
    {
        qWarning() << "[MidiMonitor]: No physical midi inputs detected";
    }
}

void QMidiManagerPrivate::resetMidiOutPorts()
{
    closeOutputPorts();
    resetPhysicalMidiOutPorts();

    if (m_midiOuts.empty())
    {
        qWarning() << "[MidiMonitor]: No physical midi outputs detected";
    }
}

void QMidiManagerPrivate::resetPhysicalMidiOutPorts()
{
    // We need one instance of QMidiOut in order to get the port count.
    // This limitation is discused here: https://github.com/thestk/rtmidi/issues/50
    // It could be done by using a static instance of MidiApi but it can cause several
    // drawback. The first I get in mind it can become difficult to manage several port instances into
    // different thread because ports should share the same MidiApi instance.
    auto firstMidiPort = std::make_shared<QMidiOut>();
    auto const portCount = firstMidiPort->portCount();
    std::vector<std::shared_ptr<QAbstractMidiOut>> ports;

    if (portCount > 0)
    {
        ports.emplace_back(std::move(firstMidiPort));

        // Instanciate midi other inputs
        for (int i = 0; i < portCount - 1; ++i)
        {
            ports.emplace_back(std::make_shared<QMidiOut>());
        }
    }
    m_outputDeviceModel->reset(ports);
}

void QMidiManagerPrivate::closeOutputPorts()
{
    m_midiOuts.clear();
}

void QMidiManagerPrivate::closeInputPorts()
{
    m_midiIns.clear();
}

QMidiManager::QMidiManager(QObject* parent)
: QObject(parent)
, d_ptr(new QMidiManagerPrivate(this))
{
}

QMidiManager::~QMidiManager()
{
}

QMidiPortModel* QMidiManager::getInputDeviceModel() const
{
    Q_D(const QMidiManager);

    return d->m_inputDeviceModel;
}

QMidiPortModel* QMidiManager::getOutputDeviceModel() const
{
    Q_D(const QMidiManager);

    return d->m_outputDeviceModel;
}

/*!
 * \brief Rescan physical MIDI ports
 */
void QMidiManager::rescanPorts()
{
    Q_D(QMidiManager);

    d->resetMidiInPorts();
    d->resetMidiOutPorts();
    emit portsRescanned();
}

void QMidiManager::closeAllPorts()
{
    Q_D(QMidiManager);

    d->closeOutputPorts();
    d->closeInputPorts();
    d->m_matrixModel->clear();
}

QMidiMessageMatrixModel* QMidiManager::getMessageMatrixModel() const
{
    Q_D(const QMidiManager);

    return d->m_matrixModel;
}

QMidiMessageFilterFactory* QMidiManager::getMessageFilterFactory() const
{
    Q_D(const QMidiManager);

    return d->m_messageFilterFactory;
}

QMidiManufacturerModel *QMidiManager::getManufacturerModel() const
{
    Q_D(const QMidiManager);

    return d->m_manufacturerModel;
}

void QMidiManagerPrivate::forwardMidiMessage(QMidiMessage const& message)
{
    auto const& matrix = m_matrixModel->matrix();

    matrix.forachInput(message.port(),
                         [this, &message](auto out, auto)
                         {
                             Q_ASSERT( out > -1 );
                             Q_ASSERT( static_cast<std::size_t>(out) < m_midiOuts.size() );

                             m_midiOuts[static_cast<std::size_t>(out)]->sendMessage(message);
                         });
}

int QMidiManagerPrivate::addInputPort(const std::shared_ptr<QAbstractMidiIn> &midiIn)
{
    Q_ASSERT( midiIn != nullptr );
    Q_Q(QMidiManager);

    int const newPortIndex = static_cast<int>(m_midiIns.size());
    int returnedPortIndex = -1;

    if (imp::tryToOpenPort(midiIn, newPortIndex))
    {
        midiIn->addMessageReceivedListener([this, q](QMidiMessage const& message)
        {
            emit q->messageReceived(message);
            forwardMidiMessage(message);
        });
        m_midiIns.emplace_back(midiIn);
        m_matrixModel->reset(static_cast<int>(m_midiOuts.size()),
                             static_cast<int>(m_midiIns.size()),
                             extractPortNames(m_midiOuts),
                             extractPortNames(m_midiIns));
        returnedPortIndex = newPortIndex;
    }
    return returnedPortIndex;
}

int QMidiManagerPrivate::addOutputPort(std::shared_ptr<QAbstractMidiOut> const& midiOut)
{
    Q_ASSERT( midiOut != nullptr );

    int const newPortIndex = static_cast<int>(m_midiOuts.size());
    int returnedPortIndex = -1;

    if (imp::tryToOpenPort(midiOut, newPortIndex))
    {
        m_midiOuts.emplace_back(midiOut);
        m_matrixModel->reset(static_cast<int>(m_midiOuts.size()),
                             static_cast<int>(m_midiIns.size()),
                             extractPortNames(m_midiOuts),
                             extractPortNames(m_midiIns));
        returnedPortIndex = newPortIndex;
    }

    return returnedPortIndex;
}

void QMidiManagerPrivate::onMidiInputPortRowInserted(QModelIndex const& parent, int first, int last)
{
    for (auto i = first; i <= last; ++i)
    {
        auto const index = m_inputDeviceModel->index(i, 0, parent);

        if (m_inputDeviceModel->getItemType(index) == QMidiPortModel::ItemType::InputPort)
        {
            addInputPort(m_inputDeviceModel->getInputPort(index));
        }
    }
}

void QMidiManagerPrivate::onMidiOutputPortRowInserted(QModelIndex const& parent, int first, int last)
{
    for (auto i = first; i <= last; ++i)
    {
        auto const index = m_outputDeviceModel->index(i, 0, parent);

        if (m_outputDeviceModel->getItemType(index) == QMidiPortModel::ItemType::OutputPort)
        {
            addOutputPort(m_outputDeviceModel->getOutputPort(index));
        }
    }
}

void QMidiManagerPrivate::onMidiInputPortAboutToReset()
{
    for (auto i = 0; i < m_inputDeviceModel->rowCount(); ++i)
    {
        auto midiIn = m_inputDeviceModel->getInputPort(m_inputDeviceModel->index(i, 0));

        if (midiIn->isPortOpen())
        {
            midiIn->closePort();
        }
    }
    m_midiIns.clear();
}

void QMidiManagerPrivate::onMidiOutputPortAboutToReset()
{
    for (auto i = 0; i < m_outputDeviceModel->rowCount(); ++i)
    {
        auto midiOut = m_outputDeviceModel->getOutputPort(m_outputDeviceModel->index(i, 0));

        if (midiOut->isPortOpen())
        {
            midiOut->closePort();
        }
    }
    m_midiOuts.clear();
}

void QMidiManagerPrivate::onMidiInputPortReset()
{
    m_midiIns.clear();

    for (auto i = 0; i < m_inputDeviceModel->rowCount(); ++i)
    {
        auto midiIn = m_inputDeviceModel->getInputPort(m_inputDeviceModel->index(i, 0));

        if (imp::tryToOpenPort(midiIn, i))
        {
            addInputPort(midiIn);
        }
    }

    m_matrixModel->reset(static_cast<int>(m_midiOuts.size()),
                         static_cast<int>(m_midiIns.size()),
                         extractPortNames(m_midiOuts),
                         extractPortNames(m_midiIns));
}

void QMidiManagerPrivate::onMidiOutputPortReset()
{
    m_midiOuts.clear();

    for (auto i = 0; i < m_outputDeviceModel->rowCount(); ++i)
    {
        auto midiOut = m_outputDeviceModel->getOutputPort(m_outputDeviceModel->index(i, 0));

        if (imp::tryToOpenPort(midiOut, i))
        {
            addOutputPort(midiOut);
        }
    }

    m_matrixModel->reset(static_cast<int>(m_midiOuts.size()),
                         static_cast<int>(m_midiIns.size()),
                         extractPortNames(m_midiOuts),
                         extractPortNames(m_midiIns));
}

void QMidiManagerPrivate::onMidiInputPortRowRemoved(const QModelIndex &parent, int first, int last)
{
    for (auto i = first; i <= last; ++i)
    {
        auto const index = m_inputDeviceModel->index(i, 0, parent);

        if (m_inputDeviceModel->getItemType(index) == QMidiPortModel::ItemType::InputPort)
        {
            auto const port = m_inputDeviceModel->getInputPort(index);

            imp::removeOne(m_midiIns, port);
            port->closePort();
        }
    }
    m_matrixModel->onMidiInputRemoved(parent, first, last);
}

void QMidiManagerPrivate::onMidiOutputPortRowRemoved(const QModelIndex &parent, int first, int last)
{
    for (auto i = first; i <= last; ++i)
    {
        auto const index = m_outputDeviceModel->index(i, 0, parent);

        if (m_outputDeviceModel->getItemType(index) == QMidiPortModel::ItemType::OutputPort)
        {
            auto const port = m_outputDeviceModel->getOutputPort(index);

            imp::removeOne(m_midiOuts, port);
            port->closePort();
        }
    }
    m_matrixModel->onMidiOutputRemoved(parent, first, last);
}

#include "QMidiManager.moc"
