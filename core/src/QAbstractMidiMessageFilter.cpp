//
// Created by Io on 18/02/2018.
//

#include "QAbstractMidiMessageFilter.hpp"

QAbstractMidiMessageFilter::QAbstractMidiMessageFilter(QString const& name, bool const enabled)
: m_name(name)
, m_enabled(enabled)
{
}

bool QAbstractMidiMessageFilter::acceptMessage(QMidiMessage const& message) const
{
    return (m_enabled ? !filterMessage(message) : true);
}

QString const& QAbstractMidiMessageFilter::name() const
{
    return m_name;
}

void QAbstractMidiMessageFilter::setupWidget()
{
    m_widget = instanciateWidget();
}

void QAbstractMidiMessageFilter::setEnabled(bool const enabled)
{
    m_enabled = enabled;
}

bool QAbstractMidiMessageFilter::isEnabled() const
{
    return m_enabled;
}

std::unique_ptr<QWidget> const& QAbstractMidiMessageFilter::getWidget() const
{
    return m_widget;
}

std::unique_ptr<QWidget> QAbstractMidiMessageFilter::instanciateWidget()
{
    return std::unique_ptr<QWidget>{};
}
