//
// Created by Io on 01/01/2018.
//

#ifndef MIDIMONITOR_MAINWINDOW_HPP
#define MIDIMONITOR_MAINWINDOW_HPP
#include <QMainWindow>
#include <QModelIndex>

class QMidiTranslatorFactory;
class QMidiMessageModel;
class QMidiDeviceModel;
class QMidiMessage;
class QMidiIn;
class QMidiManufacturerModel;
class QMidiManager;

class MidiMessageListView;
class MidiNoteTriggerWidget;

class DockWidgetManager;
class DeviceSchemeWidget;
class ToolBarManager;

class QAction;
class QComboBox;
class QWidget;
class QItemSelectionModel;
class QSystemTrayIcon;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
private:
    void setupUi();
    void setupActions();
    void setupSystem();
    void setupMenus();
    void setupToolbars();
    void setupTrayIcon();

    void updateActions();

    void onInputPortEnabled(int const portId, bool const enabled);
    void onOutputPortEnabled(int const portId, bool const enabled);
    void resetMidiInputs();

    void saveSettings() const;
    void loadSettings();

    void showAbout();
protected:
    void changeEvent(QEvent* event) override;

private:
    QMidiManager* const m_midiManager;
    QMidiTranslatorFactory* const m_deviceSchemeFactory;
    QMidiDeviceModel* const m_inputPortModel;
    QMidiDeviceModel* const m_outputPortModel;
    QMidiMessageModel* const m_messageModel;
    QItemSelectionModel* const m_messageSelection;
    MidiMessageListView* const m_messageView;
    DockWidgetManager* const m_dockWidgets;
    ToolBarManager* const m_toolbars;
    QMidiManufacturerModel* const m_manufacturerModel;
    MidiNoteTriggerWidget* const m_noteWidget;
    QSystemTrayIcon* const m_trayIcon;

    // File actions
    QAction* const m_actionRescanMidiPorts;
    QAction* const m_actionQuit;
    QAction* const m_actionAbout;
    // Edit actions
    QAction* const m_actionClearAll;
    // View actions
    QAction* const m_actionSwitchAutoScrollToBottom;
    // Tray icon actions
    QAction* const m_actionRestoreWindow;
};

#endif //MIDIMONITOR_MAINWINDOW_HPP
