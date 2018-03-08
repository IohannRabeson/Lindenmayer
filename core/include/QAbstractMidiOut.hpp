//
// Created by Io on 23/01/2018.
//

#ifndef MIDIMONITOR_QABSTRACTMIDIOUT_HPP
#define MIDIMONITOR_QABSTRACTMIDIOUT_HPP
#include <vector>
#include <functional>

#include <QString>
#include "QAbstractMidiMessageFilter.hpp"

class QMidiMessage;

class QAbstractMidiOut
{
public:
    using ErrorListener = std::function<void(QString const&)>;
    using MessageReceivedCallback = std::function<void(QMidiMessage const&)>;
    using FilterPointer = std::unique_ptr<QAbstractMidiMessageFilter>;

    virtual bool openPort(int const portIndex) = 0;
    virtual void closePort() = 0;
    virtual int portOpened() const = 0;
    virtual QString portName() const = 0;
    virtual void setPortEnabled(bool const enabled) = 0;
    virtual bool isPortEnabled() const = 0;

    void sendMessage(QMidiMessage const& message);
    bool isPortOpen() const;

    void addErrorListener(ErrorListener&& listener);
    void addMessageReceivedListener(MessageReceivedCallback&& listener);

    int addFilter(FilterPointer&& filter);
    void removeFilter(int const filterIndex);
    void clearFilters();
    int filterCount() const;
protected:
    void error(QString const& error);
    void messageReceived(QMidiMessage const& message);
private:
    bool acceptMessage(QMidiMessage const& message) const;
    virtual void outputMessage(QMidiMessage const& message) = 0;
private:
    std::vector<ErrorListener> m_errorListeners;
    std::vector<MessageReceivedCallback> m_messageReceivedListeners;
    std::vector<FilterPointer> m_messageFilters;
};

#endif //MIDIMONITOR_QABSTRACTMIDIOUT_HPP
