//
// Created by Io on 01/01/2018.
//

#ifndef MIDIMONITOR_MAINWINDOW_HPP
#define MIDIMONITOR_MAINWINDOW_HPP
#include <QMainWindow>
#include <QModelIndex>

class QDeviceSchemeFactory;
class QMidiMessageModel;
class QMidiPortModel;
class QMidiMessage;
class QMidiIn;
class QAbstractDeviceScheme;

class MidiMessageListView;
class DockWidgetManager;
class DeviceSchemeWidget;

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
    void setScheme(QModelIndex const& index);
private:
    QMidiIn* const m_midiIn;
    QDeviceSchemeFactory* const m_deviceSchemeFactory;
    QMidiPortModel* const m_inputPortModel;
    QMidiMessageModel* const m_messageModel;
    QItemSelectionModel* const m_messageSelection;
    MidiMessageListView* const m_messageView;
    DockWidgetManager* const m_dockWidgets;
    QComboBox* const m_inputPortSelector;
    QComboBox* const m_schemeSelector;

    QModelIndex m_currentSchemeIndex;

    // File actions
    QAction* const m_actionClearAll;
    QAction* const m_actionQuit;

    // View actions
    QAction* const m_actionSwitchAutoScrollToBottom;

    void setupUi();
    void setupActions();
    void setupSystem();
    void setupMenus();
    void setupToolbars();
};

#endif //MIDIMONITOR_MAINWINDOW_HPP
