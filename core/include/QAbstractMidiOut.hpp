//
// Created by Io on 23/01/2018.
//

#ifndef MIDIMONITOR_QABSTRACTMIDIOUT_HPP
#define MIDIMONITOR_QABSTRACTMIDIOUT_HPP
#include <vector>
#include <functional>

#include <QString>

class QMidiMessage;


class QAbstractMidiOut
{
public:
    using ErrorListener = std::function<void(QString const&)>;
    using MessageReceivedCallback = std::function<void(QMidiMessage const&)>;

    virtual bool openPort(int const portIndex) noexcept = 0;
    virtual void closePort() noexcept = 0;
    virtual int portOpened() const noexcept = 0;
    virtual QString portName() const noexcept = 0;
    virtual int portCount() const noexcept = 0;
    virtual void setEnabled(bool const enabled) = 0;
    virtual void sendMessage(QMidiMessage const& message) = 0;

    void error(QString const& error);
    void messageReceived(QMidiMessage const& message);

    void addErrorListener(ErrorListener&& listener);
    void addMessageReceivedListener(MessageReceivedCallback&& listener);
private:
    std::vector<ErrorListener> m_errorListeners;
    std::vector<MessageReceivedCallback> m_messageReceivedListeners;
};

#endif //MIDIMONITOR_QABSTRACTMIDIOUT_HPP
