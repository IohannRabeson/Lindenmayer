//
// Created by Io on 31/12/2017.
//

#ifndef SYSEXMONITOR_MIDIINPUTWIDGET_HPP
#define SYSEXMONITOR_MIDIINPUTWIDGET_HPP
#include "MidiWidget.hpp"

class QMidiIn;
class QMidiPortModel;

class MidiInputWidget : public MidiWidget
{
public:
    explicit MidiInputWidget(QWidget* parent = nullptr);
private:
    void onMidiPortChanged(int const portIndex) override;
private:
    QMidiIn* const m_midiIn;
    QMidiPortModel* const m_midiInPortModel;
};

#endif //SYSEXMONITOR_MIDIINPUTWIDGET_HPP
