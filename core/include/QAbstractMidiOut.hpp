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
    using FilterPointer = std::shared_ptr<QAbstractMidiMessageFilter>;

    virtual ~QAbstractMidiOut() = default;
    virtual bool openPort(int const portIndex) = 0;
    virtual void closePort() = 0;
    virtual int portOpened() const = 0;
    virtual QString portName() const = 0;
    virtual void setPortEnabled(bool const enabled) = 0;
    virtual bool isPortEnabled() const = 0;
    virtual bool isPortRemovable() const = 0;

    void sendMessage(QMidiMessage const& message);
    bool isPortOpen() const;

    void addErrorListener(ErrorListener&& listener);

    int addFilter(FilterPointer const& filter);
    void removeFilter(int const filterIndex);
    void clearFilters();
    int filterCount() const;
protected:
    void error(QString const& error);
private:
    virtual void outputMessage(QMidiMessage const& message) = 0;
private:
    std::vector<ErrorListener> m_errorListeners;
    std::vector<FilterPointer> m_messageFilters;
};

#endif //MIDIMONITOR_QABSTRACTMIDIOUT_HPP
