//
// Created by Io on 01/01/2018.
//

#ifndef MIDIMONITOR_MAINWINDOW_HPP
#define MIDIMONITOR_MAINWINDOW_HPP
#include <QMainWindow>
#include <QModelIndex>

class QDeviceSchemeFactory;
class QMidiMessageModel;
class QMidiDeviceModel;
class QMidiMessage;
class QMidiIn;
class QMidiManufacturerModel;
class QMidiManager;

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
//    void appendMessage(QMidiMessage const& message);
//    void clearMessages();
    void closeAllPorts();
private:
    void setupUi();
    void setupActions();
    void setupSystem();
    void setupMenus();
    void setupToolbars();

    void onInputPortEnabled(int const portId, bool const enabled);

    void resetMidiInputs();

    void saveSettings() const;
    void loadSettings();

    void showAbout();
private:
    QMidiManager* const m_midiManager;
    QDeviceSchemeFactory* const m_deviceSchemeFactory;
    QMidiDeviceModel* const m_inputPortModel;
    QMidiMessageModel* const m_messageModel;
    QItemSelectionModel* const m_messageSelection;
    MidiMessageListView* const m_messageView;
    DockWidgetManager* const m_dockWidgets;
    ToolBarManager* const m_toolbars;
    QMidiManufacturerModel* const m_manufacturerModel;

    // File actions
    QAction* const m_actionRescanMidiPorts;
    QAction* const m_actionQuit;
    QAction* const m_actionAbout;
    // Edit actions
    QAction* const m_actionClearAll;
    // View actions
    QAction* const m_actionSwitchAutoScrollToBottom;
};

#endif //MIDIMONITOR_MAINWINDOW_HPP
