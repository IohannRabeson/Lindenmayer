//
// Created by Io on 18/02/2018.
//

#ifndef MIDIMONITOR_QABSTRACTMIDIMESSAGEFILTER_HPP
#define MIDIMONITOR_QABSTRACTMIDIMESSAGEFILTER_HPP
#include <memory>
#include <QWidget>

class QMidiMessage;

class QAbstractMidiMessageFilter
{
public:
    explicit QAbstractMidiMessageFilter(QString const& name, bool const enabled = true);
    virtual ~QAbstractMidiMessageFilter() = default;

    QAbstractMidiMessageFilter(QAbstractMidiMessageFilter const&) = delete;
    QAbstractMidiMessageFilter(QAbstractMidiMessageFilter&&) = default;
    QAbstractMidiMessageFilter& operator = (QAbstractMidiMessageFilter&&) = default;

    void setupWidget();

    bool acceptMessage(QMidiMessage const& message) const;

    QString const& name() const;
    void setName(QString const& name);
    void setEnabled(bool const enabled);
    bool isEnabled() const;

    std::unique_ptr<QWidget> const& getWidget() const;
private:
    virtual bool filterMessage(QMidiMessage const& message) const = 0;
    virtual std::unique_ptr<QWidget> instanciateWidget();
private:
    QString m_name;
    std::unique_ptr<QWidget> m_widget;
    bool m_enabled = true;
};

#endif //MIDIMONITOR_QABSTRACTMIDIMESSAGEFILTER_HPP
