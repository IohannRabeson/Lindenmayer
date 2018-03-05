//
// Created by Io on 19/01/2018.
//

#ifndef MIDIMONITOR_QABSTRACTMIDIIN_HPP
#define MIDIMONITOR_QABSTRACTMIDIIN_HPP
#include <functional>
#include <memory>
#include <vector>

#include "QAbstractMidiMessageFilter.hpp"

class QString;
class QMidiMessage;

class QAbstractMidiIn
{
public:
    using ErrorListener = std::function<void(QString const&)>;
    using MessageReceivedCallback = std::function<void(QMidiMessage const&)>;
    using FilterPointer = std::unique_ptr<QAbstractMidiMessageFilter>;

    virtual ~QAbstractMidiIn() = default;
    virtual bool openPort(int const portIndex) noexcept = 0;
    virtual void closePort() noexcept = 0;
    /*!
     * \brief Return the index of the opened port or -1 if the port is not open.
     */
    virtual int portOpened() const noexcept = 0;
    virtual QString portName() const noexcept = 0;
    virtual void setPortEnabled(bool const enabled) noexcept = 0;
    virtual bool isPortEnabled() const noexcept = 0;
    bool isPortOpen() const;

    void addErrorListener(ErrorListener&& listener);
    void addMessageReceivedListener(MessageReceivedCallback&& listener);

    int addFilter(FilterPointer&& filter);
    void removeFilter(int const filterIndex);
    void clearFilters();
    int filterCount() const;
protected:
    void error(QString const& error);
    virtual void messageReceived(QMidiMessage const& message);
private:
    bool acceptMessage(QMidiMessage const& message) const;
private:
    std::vector<ErrorListener> m_errorListeners;
    std::vector<MessageReceivedCallback> m_messageReceivedListeners;
    std::vector<FilterPointer> m_messageFilters;
};

#endif //MIDIMONITOR_QABSTRACTMIDIIN_HPP
