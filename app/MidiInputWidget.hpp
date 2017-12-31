//
// Created by Io on 31/12/2017.
//

#ifndef SYSEXMONITOR_MIDIINPUTWIDGET_HPP
#define SYSEXMONITOR_MIDIINPUTWIDGET_HPP
#include "MidiWidget.hpp"

class QMidiIn;

class MidiInputWidget : public MidiWidget
{
public:
    explicit MidiInputWidget(QWidget* parent = nullptr);
private:
    QMidiIn* const m_midiIn;
};

#endif //SYSEXMONITOR_MIDIINPUTWIDGET_HPP
