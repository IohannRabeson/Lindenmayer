//
// Created by Io on 31/12/2017.
//

#ifndef MIDIMONITOR_MIDICONSOLEVIEW_HPP
#define MIDIMONITOR_MIDICONSOLEVIEW_HPP
#include <QTreeView>

#include <QMidiOutBase.hpp>

class QMidiManager;
class QMidiMessageModel;
class QMidiMessage;

class QSettings;

class MidiConsoleView : public QTreeView, public QMidiOutBase
{
    Q_OBJECT

    static unsigned int s_midiConsoleInstances;
public:
    explicit MidiConsoleView(QMidiManager* const midiManager, QWidget* parent = nullptr);

    void setAutoScrollToBottomEnabled(bool const enabled);
    void loadSettings(QSettings& settings);
    void saveSettings(QSettings& settings) const;
    bool isPortRemovable() const override;
private:
    void outputMessage(const QMidiMessage &message) override;

    void onNewMessage();
private:
    QMidiManager* const m_midiManager;
    QMidiMessageModel* const m_messageModel;
    bool m_autoScroll;
};

#endif //MIDIMONITOR_MIDICONSOLEVIEW_HPP
