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
    QAbstractMidiMessageFilter& operator = (QAbstractMidiMessageFilter const&) = delete;
    QAbstractMidiMessageFilter(QAbstractMidiMessageFilter&&) = default;
    QAbstractMidiMessageFilter& operator = (QAbstractMidiMessageFilter&&) = default;

    /*!
     * \brief Test if a message should be filtered or not.
     * \param message Message to test.
     * \return True if the message should'nt discarded by this filter, otherwise false.
     *
     * \see QMidiInListModel
     */
    bool acceptMessage(QMidiMessage const& message) const;

    QString const& name() const;
    void setEnabled(bool const enabled);
    bool isEnabled() const;

    void setupWidget();

    std::unique_ptr<QWidget> const& getWidget() const;
private:
    /*!
     * \brief Implement this method to define which messages are filtered.
     * \param message Message to test.
     * \return True to filter a message, false to accept a message.
     */
    virtual bool filterMessage(QMidiMessage const& message) const = 0;
    virtual std::unique_ptr<QWidget> instanciateWidget();
private:
    QString const m_name;
    std::unique_ptr<QWidget> m_widget;
    bool m_enabled = true;
};

#endif //MIDIMONITOR_QABSTRACTMIDIMESSAGEFILTER_HPP
