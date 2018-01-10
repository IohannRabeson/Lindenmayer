//
// Created by Io on 01/01/2018.
//

#include "MainWindow.hpp"
#include "Ui/Widgets/MidiMessageListView.hpp"
#include "Ui/DockWidgetManager.hpp"
#include "Ui/ToolBarManager.hpp"
#include "Ui/Widgets/DeviceSchemeWidget.hpp"
#include "Ui/CommonUi.hpp"
#include "Ui/Widgets/AboutMidiMonitorDialog.hpp"
#include "Ui/SettingsUtils.hpp"

#include "Plugins/Waldorf/Pulse2/Pulse2Translator.hpp"

#include "Delegates/MidiDelegates.hpp"

#include "Ui/Widgets/MidiNoteTriggerWidget.hpp"
#include "Ui/Widgets/MidiKeyboardWidget.hpp"

#include <QComboBox>
#include <QToolBar>
#include <QMenuBar>
#include <QLineEdit>
#include <QStylePainter>
#include <QMessageBox>
#include <QTableView>
#include <QSystemTrayIcon>
#include <QEvent>

#include <QtDebug>

#include <QMidiTranslatorFactory.hpp>
#include <QMidiIn.hpp>
#include <QMidiMessageModel.hpp>
#include <QMidiManager.hpp>

#include <QMetaEnum>

namespace
{
    QAction* createSeparator(QObject* parent)
    {
        QAction* action = new QAction(parent);

        action->setSeparator(true);
        return action;
    }

    class ComboBox : public QComboBox
    {
    protected:
        void paintEvent(QPaintEvent*) override
        {
            QStylePainter painter(this);
            painter.setPen(palette().color(QPalette::Text));

            QStyleOptionComboBox option;
            initStyleOption(&option);

            if (!m_label.isEmpty())
            {
                option.currentText = QString("%0 | %1").arg(m_label).arg(currentText());
            }

            painter.drawComplexControl(QStyle::CC_ComboBox, option);

            // draw the icon and text
            painter.drawControl(QStyle::CE_ComboBoxLabel, option);
        }

    public:
        using QComboBox::QComboBox;

        ComboBox(QString const& label, QWidget* parent)
                : QComboBox(parent), m_label(label)
        {
            setSizeAdjustPolicy(QComboBox::AdjustToContents);
            setMinimumContentsLength(m_label.size() + 10);
        }

    private:
        QString m_label;
    };
}

MainWindow::MainWindow(QWidget* parent)
: QMainWindow(parent)
, m_trayIcon(new QSystemTrayIcon(this))
, m_midiManager(new QMidiManager(this))
, m_deviceSchemeFactory(new QMidiTranslatorFactory(this))
, m_inputPortModel(m_midiManager->getInputDeviceModel())
, m_outputPortModel(m_midiManager->getOutputDeviceModel())
, m_messageModel(new QMidiMessageModel(this))
, m_messageSelection(new QItemSelectionModel(m_messageModel, this))
, m_messageView(new MidiMessageListView(m_messageModel, this))
, m_dockWidgets(new DockWidgetManager(this))
, m_toolbars(new ToolBarManager(this))
, m_manufacturerModel(new QMidiManufacturerModel(this))
, m_noteWidget(new MidiNoteTriggerWidget(this))
, m_keyboardWidget(new MidiKeyboardWidget(this))

, m_actionRescanMidiPorts(new QAction(tr("Rescan"), this))
, m_actionQuit(new QAction(tr("Quit"), this))
, m_actionClearAll(new QAction(tr("Clear all"), this))
, m_actionAbout(new QAction(tr("About...")))
, m_actionSwitchAutoScrollToBottom(new QAction(tr("Auto scrolling")))
, m_actionRestoreWindow(new QAction(tr("Show")))
{
    setupSystem();
    setupActions();
    setupToolbars();
    setupMenus();
    setupUi();
    setupTrayIcon();
    loadSettings();
    updateActions();
}

MainWindow::~MainWindow()
{
    saveSettings();
    m_midiManager->closeAll();
}

void MainWindow::setupSystem()
{
    // Setup scheme factory
    m_deviceSchemeFactory->add<Pulse2Translator>("Pulse 2");
    m_midiManager->resetPorts();
    connect(m_inputPortModel, &QMidiDeviceModel::checkedChanged, this, &MainWindow::onInputPortEnabled);
    connect(m_outputPortModel, &QMidiDeviceModel::checkedChanged, this, &MainWindow::onOutputPortEnabled);
    connect(m_midiManager, &QMidiManager::messageReceived, m_messageModel, &QMidiMessageModel::append);
    connect(m_midiManager, &QMidiManager::messageSent, m_messageModel, &QMidiMessageModel::append);
    m_manufacturerModel->load(QMidiManufacturerModel::LoadFromCSV(":/Texts/Resources/MIDI_Manufacturers.csv"));
}

void MainWindow::resetMidiInputs()
{
    auto const* const midiInModel = m_midiManager->getInputDeviceModel();

    if (midiInModel->rowCount() > 0)
    {
        if (QMessageBox::warning(this, tr("Clear messages"),
                                 tr("Messages can lost their input port information.\nAre you sure you want to do that?"),
                                 QMessageBox::StandardButton::Yes,
                                 QMessageBox::StandardButton::No) == QMessageBox::StandardButton::No)
        {
            // Abort action.
            return;
        }
    }

    // The message with input port information will have their indexes
    // updated when possible and set to -1 removed indexes.
    QMap<int, int> inputPortRemappings;

    m_midiManager->resetPorts(inputPortRemappings);
    m_messageModel->remapInputPorts(inputPortRemappings);
}

void MainWindow::setupActions()
{
    // Setup actions
    m_actionSwitchAutoScrollToBottom->setCheckable(true);
    m_actionSwitchAutoScrollToBottom->setChecked(true);

    connect(m_actionClearAll, &QAction::triggered, m_messageModel, &QMidiMessageModel::clear);
    connect(m_actionQuit, &QAction::triggered, this, &QMainWindow::close);
    connect(m_actionRescanMidiPorts, &QAction::triggered, this, &MainWindow::resetMidiInputs);
    connect(m_actionAbout, &QAction::triggered, this, &MainWindow::showAbout);
    connect(m_actionSwitchAutoScrollToBottom, &QAction::triggered, m_messageView, &MidiMessageListView::setAutoScrollToBottomEnabled);
}

void MainWindow::setupUi()
{
    // Setup message view
    m_messageView->setModel(m_messageModel);
    m_messageView->setSelectionModel(m_messageSelection);
    m_messageView->setItemDelegateForColumn(QMidiMessageModel::Columns::Type, new MidiMessageTypeDelegate(this));
    m_messageView->setItemDelegateForColumn(QMidiMessageModel::Columns::Input, new MidiPortDelegate(m_inputPortModel, this));
    m_messageView->setItemDelegateForColumn(QMidiMessageModel::Columns::Channel, new MidiChannelDelegate(this));
    m_messageView->setItemDelegateForColumn(QMidiMessageModel::Columns::Timestamp, new MidiTimeDelegate(this));
    m_messageView->setItemDelegateForColumn(QMidiMessageModel::Columns::Value, new MidiValueDelegate(m_messageModel, m_manufacturerModel, this));
    m_messageView->setItemDelegateForColumn(QMidiMessageModel::Columns::Data, new MidiDataDelegate(m_messageModel, this));
    setCentralWidget(m_messageView);

    // Setup MIDI input port view
    QTableView* midiInputPortView = new QTableView(this);

    CommonUi::standardTableView(midiInputPortView, false);
    midiInputPortView->setModel(m_inputPortModel);
    m_dockWidgets->addDockWidget(midiInputPortView, tr("MIDI Inputs"));

    // Setup MIDI output port view
    QTableView* midiOutputPortView = new QTableView(this);

    CommonUi::standardTableView(midiOutputPortView, false);
    midiOutputPortView->setModel(m_midiManager->getOutputDeviceModel());
    m_dockWidgets->addDockWidget(midiOutputPortView, tr("MIDI Outputs"));

    // Setup note widget
    m_dockWidgets->addDockWidget(m_noteWidget, tr("MIDI Note Trigger"));
    connect(m_noteWidget, &MidiNoteTriggerWidget::sendMessage, m_midiManager, &QMidiManager::sendMessage);

    // Setup keyboard widget
    m_dockWidgets->addDockWidget(m_keyboardWidget, tr("MIDI keyboard"));
    connect(m_keyboardWidget, &MidiKeyboardWidget::sendMessage, m_midiManager, &QMidiManager::sendMessage);

    // Setup window
    setAnimated(true);
    setUnifiedTitleAndToolBarOnMac(false);
}

void MainWindow::setupToolbars()
{
    QToolBar* const mainToolbar = m_toolbars->addToolBar(tr("Main"));

    mainToolbar->setIconSize(QSize(16, 16));
    mainToolbar->addAction(m_actionRescanMidiPorts);
    mainToolbar->addAction(m_actionClearAll);
}

void MainWindow::setupTrayIcon()
{
    QMenu* const contextMenu = new QMenu(this);

    contextMenu->addAction(m_actionRestoreWindow);
    contextMenu->addAction(createSeparator(this));
    contextMenu->addAction(m_actionAbout);
    contextMenu->addAction(m_actionQuit);

    connect(m_actionRestoreWindow, &QAction::triggered, [this]()
    {
        if (isMinimized())
        {
            showNormal();
        }
    });

    m_trayIcon->setIcon(QIcon(":/Images/Resources/Midi.png"));
    m_trayIcon->setContextMenu(contextMenu);
    m_trayIcon->setVisible(true);
}

void MainWindow::setupMenus()
{
    QMenuBar* const bar = menuBar();
    QMenu* const fileMenu = bar->addMenu(tr("File"));
    QMenu* const editMenu = bar->addMenu(tr("Edit"));
    QMenu* const view = bar->addMenu(tr("View"));
    QMenu* const windowMenu = bar->addMenu(tr("Window"));
    QMenu* const helpMenu = bar->addMenu(tr("?"));

    fileMenu->addAction(m_actionRescanMidiPorts);
    fileMenu->addAction(m_actionQuit);

    editMenu->addAction(m_actionClearAll);

    view->addAction(m_actionSwitchAutoScrollToBottom);

    windowMenu->addMenu(m_dockWidgets->controlMenu());
    windowMenu->addMenu(m_toolbars->controlMenu());

    helpMenu->addAction(m_actionAbout);
}

void MainWindow::onInputPortEnabled(int const portId, bool const enabled)
{
    m_midiManager->setInputPortEnabled(portId, enabled);
}

void MainWindow::onOutputPortEnabled(int const portId, bool const enabled)
{
    m_midiManager->setOutputPortEnabled(portId, enabled);
}

void MainWindow::saveSettings() const
{
    QSettings settings;

    qDebug() << "[MainWindow] Save settings:" << settings.fileName();
    settings.beginGroup("main_window");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
    settings.endGroup();

    m_messageView->saveSettings(settings);
    m_noteWidget->saveSettings(settings);
    m_keyboardWidget->saveSettings(settings);
}

void MainWindow::loadSettings()
{
    QSettings settings;

    qDebug() << "[MainWindow] Load settings:" << settings.fileName();
    settings.beginGroup("main_window");
    if (settings.contains("geometry") == false)
    {
        showMaximized();
    }
    else
    {
        restoreGeometry(restoreFrom<QByteArray>(settings, "geometry"));
        restoreState(restoreFrom<QByteArray>(settings, "state"));
    }
    settings.endGroup();

    m_messageView->loadSettings(settings);
    m_noteWidget->loadSettings(settings);
    m_keyboardWidget->loadSettings(settings);
}

void MainWindow::showAbout()
{
    if (isMinimized())
    {
        showNormal();
    }
    AboutMidiMonitorDialog dialog(this);

    dialog.exec();
}

void MainWindow::updateActions()
{
    auto const isWindowMinimized = isMinimized();

    m_actionRestoreWindow->setEnabled(isWindowMinimized);
}

void MainWindow::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::WindowStateChange)
    {
        updateActions();
    }
    QWidget::changeEvent(event);
}
