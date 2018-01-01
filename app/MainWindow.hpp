//
// Created by Io on 01/01/2018.
//

#ifndef MIDIMONITOR_MAINWINDOW_HPP
#define MIDIMONITOR_MAINWINDOW_HPP
#include <QMainWindow>

class QDeviceSchemeFactory;
class QMidiMessageModel;
class QMidiPortModel;
class QMidiMessage;
class QMidiIn;

class MidiMessageListView;
class DockWidgetManager;

class QAction;
class QComboBox;
class QWidget;
class QItemSelectionModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
public:
    void appendMessage(QMidiMessage const& message);
    void clearMessages();
    void setInputPort(int const port);
private:
    QMidiIn* const m_midiIn;
    QDeviceSchemeFactory* const m_deviceSchemeFactory;
    QMidiPortModel* const m_inputPortModel;
    QMidiMessageModel* const m_messageModel;
    QItemSelectionModel* const m_messageSelectionModel;
    MidiMessageListView* const m_messageView;
    DockWidgetManager* const m_dockWidgets;
    QComboBox* const m_inputPortSelector;

    // File actions
    QAction* const m_actionClearAll;
    QAction* const m_actionQuit;
    // View actions
    QAction* const m_actionSwitchAutoScrollToBottom;
};

#endif //MIDIMONITOR_MAINWINDOW_HPP
