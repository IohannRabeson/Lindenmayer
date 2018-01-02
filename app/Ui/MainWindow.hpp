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
class ToolBarManager;

class QAction;
class QComboBox;
class QWidget;
class QItemSelectionModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
public:
    void appendMessage(QMidiMessage const& message);
    void clearMessages();
    void setScheme(QModelIndex const& index);
    void closeAllPorts();
private:
    void onPortEnabled(int const portId, bool const enabled);

    void saveSettings() const;
    void loadSettings();

    void showAbout();
private:
    QVector<QMidiIn*> m_midiIns;
    QDeviceSchemeFactory* const m_deviceSchemeFactory;
    QMidiPortModel* const m_inputPortModel;
    QMidiMessageModel* const m_messageModel;
    QItemSelectionModel* const m_messageSelection;
    MidiMessageListView* const m_messageView;
    DockWidgetManager* const m_dockWidgets;
    ToolBarManager* const m_toolbars;
    QComboBox* const m_schemeSelector;

    QModelIndex m_currentSchemeIndex;

    // File actions
    QAction* const m_actionClearAll;
    QAction* const m_actionQuit;
    QAction* const m_aboutApplication;
    // View actions
    QAction* const m_actionSwitchAutoScrollToBottom;

    void setupUi();
    void setupActions();
    void setupSystem();
    void setupMenus();
    void setupToolbars();
};

#endif //MIDIMONITOR_MAINWINDOW_HPP
