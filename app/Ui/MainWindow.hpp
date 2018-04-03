//
// Created by Io on 01/01/2018.
//

#ifndef MIDIMONITOR_MAINWINDOW_HPP
#define MIDIMONITOR_MAINWINDOW_HPP
#include <QMainWindow>
#include <QModelIndex>
#include <memory>

class QMidiMessageModel;
class QMidiDeviceModel;
class QMidiPortModel;
class QMidiMessageFilterFactory;
class QMidiMessage;
class QMidiIn;
class QMidiManufacturerModel;
class QMidiManager;

class MidiConsoleView;
class MidiNoteTriggerWidget;
class MidiKeyboardWidget;
class MidiConsoleArea;

namespace qool
{

    class DockWidgetManager;
    class ToolBarManager;
}

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
    void setupMIDI();
    void setupMenus();
    void setupToolbars();
    void setupTrayIcon();

    void updateActions();

    void resetMidiPorts();

    void saveSettings() const;
    void loadSettings();

    void showAbout();

    void onOutputPortAdded(QModelIndex const& parent, int first, int last);
    void onOutputPortRemoved(QModelIndex const& parent, int first, int last);
protected:
    void changeEvent(QEvent* event) override;
private:
    QSystemTrayIcon* const m_trayIcon;
    QMidiManager* const m_midiManager;
    QMidiPortModel* const m_inputPortModel;
    QMidiPortModel* const m_outputPortModel;
    qool::DockWidgetManager* const m_dockWidgets;
    qool::ToolBarManager* const m_toolbars;
    MidiConsoleArea* const m_logWidget;

    std::shared_ptr<MidiNoteTriggerWidget> const m_noteWidget;
    std::shared_ptr<MidiKeyboardWidget> const m_keyboardWidget;

    // File actions
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
