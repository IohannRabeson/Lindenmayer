//
// Created by Io on 19/01/2018.
//

#ifndef MIDIMONITOR_QABSTRACTMIDIIN_HPP
#define MIDIMONITOR_QABSTRACTMIDIIN_HPP
#include <functional>
#include <memory>
#include <vector>

class QString;
class QMidiMessage;

class QAbstractMidiIn
{
public:
    using ErrorListener = std::function<void(QString const&)>;
    using MessageReceivedCallback = std::function<void(QMidiMessage const&)>;

    virtual ~QAbstractMidiIn() = default;
    virtual bool openPort(int const portIndex) noexcept = 0;
    virtual void closePort() noexcept = 0;
    virtual int portOpened() const noexcept = 0;
    virtual QString portName() const noexcept = 0;
    virtual void setPortEnabled(bool const enabled) noexcept = 0;

    void addErrorListener(ErrorListener&& listener);
    void addMessageReceivedListener(MessageReceivedCallback&& listener);
protected:
    void error(QString const& error);
public:
    virtual void messageReceived(QMidiMessage const& message);
private:
    std::vector<ErrorListener> m_errorListeners;
    std::vector<MessageReceivedCallback> m_messageReceivedListeners;
};

#include <QString>

class QMidiInBase : public QAbstractMidiIn
{
public:
    explicit QMidiInBase(QString const& name = QString());

    bool openPort(int const portIndex) noexcept override;
    void closePort() noexcept override;
    int portOpened() const noexcept override;
    QString portName() const noexcept override;
    void setPortEnabled(bool const enabled) noexcept override;

    void messageReceived(QMidiMessage const& message) override;
    bool isPortEnabled() const noexcept;
private:
    QString const m_name;
    int m_portIndex = -1;
    bool m_portEnabled = true;
};

#endif //MIDIMONITOR_QABSTRACTMIDIIN_HPP
