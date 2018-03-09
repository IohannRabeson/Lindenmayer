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

#include <QtDebug>
#include <QSize>

#include <vector>

class QMidiManagerPrivate
{
    Q_DECLARE_PUBLIC(QMidiManager)
public:
    explicit QMidiManagerPrivate(QMidiManager* q)
    : q_ptr(q)
    , m_inputDeviceModel(new QMidiPortModel(q))
    , m_outputDeviceModel(new QMidiPortModel(q))
    , m_matrixModel(new QMidiMessageMatrixModel(q))
    , m_messageFilterFactory(new QMidiMessageFilterFactory(q))
    {
    }

    void resetMidiInPorts();
    void resetMidiOutPorts();
    void closeOutputPorts();
    void closeInputPorts();
    void resetPhysicalMidiInPorts();
    void resetPhysicalMidiOutPorts();
    void forwardMidiMessage(QMidiMessage const& message);
private:
    QMidiManager* const q_ptr;
    QMidiPortModel* const m_inputDeviceModel;
    QMidiPortModel* const m_outputDeviceModel;
    QMidiMessageMatrixModel* const m_matrixModel;
    QMidiMessageFilterFactory* const m_messageFilterFactory;
    std::vector<std::shared_ptr<QAbstractMidiIn>> m_midiIns;
    std::vector<std::shared_ptr<QAbstractMidiOut>> m_midiOuts;
};

void QMidiManagerPrivate::resetPhysicalMidiInPorts()
{
    // We need one instance of QMidiIn in order to get the port count.
    // This limitation is discused here: https://github.com/thestk/rtmidi/issues/50
    auto firstMidiPort = std::make_shared<QMidiIn>();
    auto const portCount = firstMidiPort->portCount();

    if (portCount > 0u)
    {
        m_midiIns.emplace_back(std::move(firstMidiPort));

        // Instanciate midi other inputs
        for (int i = 0; i < portCount - 1; ++i)
        {
            m_midiIns.emplace_back(std::make_shared<QMidiIn>());
        }
        for (int i = 0; i < m_midiIns.size(); ++i)
        {
            m_midiIns[i]->openPort(i);
        }
    }
    m_inputDeviceModel->reset(m_midiIns);
}

void QMidiManagerPrivate::resetMidiInPorts()
{
    Q_Q(QMidiManager);

    // Remove all inputs
    closeInputPorts();

    // Poll each physical midi in ports
    resetPhysicalMidiInPorts();

    if (m_midiIns.empty())
    {
        qWarning() << "[MidiMonitor]: No physical midi inputs";
    }
    else
    {
        // Open midi inputs
        for (int i = 0; i < m_midiIns.size(); ++i)
        {
            auto const& midiIn = m_midiIns[i];

            midiIn->addMessageReceivedListener([this, q](QMidiMessage const& message)
                                               {
                                                   emit q->messageReceived(message);
                                                   forwardMidiMessage(message);
                                               });
        }
    }
}

void QMidiManagerPrivate::resetMidiOutPorts()
{
    closeOutputPorts();
    resetPhysicalMidiOutPorts();

    if (m_midiOuts.empty())
    {
        qWarning() << "[MidiMonitor]: No physical midi outputs";
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

    if (portCount > 0u)
    {
        m_midiOuts.emplace_back(std::move(firstMidiPort));

        // Instanciate midi other inputs
        for (int i = 0; i < portCount - 1; ++i)
        {
            auto midiOut = std::make_shared<QMidiOut>();

            m_midiOuts.emplace_back(std::move(midiOut));
        }
        for (int i = 0; i < m_midiOuts.size(); ++i)
        {
            m_midiOuts[i]->openPort(i);
        }
    }
    m_outputDeviceModel->reset(m_midiOuts);
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

/*!
 * \brief Rescan physical MIDI ports
 */
void QMidiManager::rescanPorts()
{
    Q_D(QMidiManager);

    d->resetMidiInPorts();
    d->resetMidiOutPorts();
    d->m_matrixModel->reset(d->m_midiOuts.size(), d->m_midiIns.size(), extractPortNames(d->m_midiOuts), extractPortNames(d->m_midiIns));
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

/*!
 * \brief Method called by a MIDI input port when a message is received.
 * \param message Message forwarded through the MIDI message matrix.
 */
void QMidiManagerPrivate::forwardMidiMessage(QMidiMessage const& message)
{
    auto const& matrix = m_matrixModel->matrix();

    matrix.forachInput(message.port(),
                         [this, &message](auto out, auto in)
                         {
                             m_midiOuts.at(out)->sendMessage(message);
                         });
}

int QMidiManager::addInputPort(std::shared_ptr<QAbstractMidiIn>&& midiIn)
{
    Q_ASSERT( midiIn != nullptr );
    Q_D(QMidiManager);

    int const newPortIndex = d->m_midiIns.size();
    int returnedPortIndex = -1;

    if (midiIn->isPortOpen() || midiIn->openPort(newPortIndex))
    {
        midiIn->addMessageReceivedListener([this, d](QMidiMessage const& message)
                                           {
                                               emit messageReceived(message);
                                               d->forwardMidiMessage(message);
                                           });
        d->m_midiIns.emplace_back(std::move(midiIn));
        d->m_inputDeviceModel->add(d->m_midiIns.back());
        d->m_matrixModel->reset(d->m_midiOuts.size(), d->m_midiIns.size(), extractPortNames(d->m_midiOuts), extractPortNames(d->m_midiIns));
        returnedPortIndex = newPortIndex;
    }
    return returnedPortIndex;
}

int QMidiManager::addOutputPort(std::shared_ptr<QAbstractMidiOut>&& midiOut)
{
    Q_ASSERT( midiOut != nullptr );
    Q_D(QMidiManager);

    int const newPortIndex = d->m_midiIns.size();
    int returnedPortIndex = -1;

    if (midiOut->isPortOpen() || midiOut->openPort(newPortIndex))
    {
        d->m_midiOuts.emplace_back(std::move(midiOut));
        d->m_outputDeviceModel->add(d->m_midiOuts.back());
        d->m_matrixModel->reset(d->m_midiOuts.size(), d->m_midiIns.size(), extractPortNames(d->m_midiOuts), extractPortNames(d->m_midiIns));
        returnedPortIndex = newPortIndex;
    }
    return returnedPortIndex;
}
