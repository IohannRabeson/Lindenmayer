//
// Created by Io on 18/02/2018.
//

#ifndef MIDIMONITOR_QABSTRACTMIDIMESSAGEFILTER_HPP
#define MIDIMONITOR_QABSTRACTMIDIMESSAGEFILTER_HPP
#include <memory>

#include <QWidget>
#include <QVariant>

class Parametrable
{
public:   
    using Setter = std::function<bool(QVariant const&)>;
    using Getter = std::function<QVariant()>;
    using Index = std::size_t;

    struct ParameterInfo
    {
        QString const name;
        QString const tooltip;
        Setter set;
        Getter get;
    };

    virtual ~Parametrable() = default;

    std::size_t parameterCount() const
    {
        return m_parameters.size();
    }

    QString name(Index const index) const
    {
        return m_parameters[index].name;
    }

    QString tooltip(Index const index) const
    {
        return m_parameters[index].tooltip;
    }

    bool setValue(Index const index, QVariant const& value)
    {
        return m_parameters[index].set(value);
    }

    QVariant getValue(Index const index) const
    {
        return m_parameters[index].get();
    }
protected:
    void addParameter(QString const& name,
                      QString const& tooltip,
                      Setter&& setter,
                      Getter&& getter)
    {
        Q_ASSERT( setter );
        Q_ASSERT( getter );

        m_parameters.emplace_back(ParameterInfo{name, tooltip, std::move(setter), std::move(getter)});
    }
private:
    std::vector<ParameterInfo> m_parameters;
};

class QMidiMessage;

class QAbstractMidiMessageFilter : public Parametrable
{
    Q_GADGET
public:
    explicit QAbstractMidiMessageFilter(QString const& name);
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
private:
    /*!
     * \brief Implement this method to define which messages are filtered.
     * \param message Message to test.
     * \return True to filter a message, false to accept a message.
     */
    virtual bool filterMessage(QMidiMessage const& message) const = 0;
private:
    QString const m_name;
    std::unique_ptr<QWidget> m_widget;
    bool m_enabled = true;
};

#endif //MIDIMONITOR_QABSTRACTMIDIMESSAGEFILTER_HPP
