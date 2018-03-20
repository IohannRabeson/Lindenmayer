//
// Created by Io on 01/01/2018.
//

#include "MainWindow.hpp"

#include "Ui/Views/MidiMessageListView.hpp"
#include "Ui/Views/MidiPortTreeView.hpp"

#include "Ui/CommonUi.hpp"
#include "Ui/Dialogs/AboutMidiMonitorDialog.hpp"
#include "Ui/SettingsUtils.hpp"

#include "Ui/Delegates/MidiDelegates.hpp"

#include "Ui/Widgets/MidiNoteTriggerWidget.hpp"
#include "Ui/Widgets/MidiKeyboardWidget.hpp"
#include "Ui/Widgets/MidiMatrixWidget.hpp"

#include <QToolBar>
#include <QMenuBar>
#include <QLineEdit>
#include <QStylePainter>
#include <QMessageBox>
#include <QSystemTrayIcon>

#include <QtDebug>

#include <QMidiIn.hpp>
#include <QMidiMessageModel.hpp>
#include <QMidiMessageMatrixModel.hpp>
#include <QMidiPortModel.hpp>
#include <QMidiManager.hpp>

#include <QMetaEnum>

#include <Qool/DockWidgetManager.hpp>
#include <Qool/ToolBarManager.hpp>

namespace
{
    QAction* createSeparator(QObject* parent)
    {
        QAction* action = new QAction(parent);

        action->setSeparator(true);
        return action;
    }
}

#include <QAbstractMidiMessageFilter.hpp>
#include <QMidiMessageFilterFactory.hpp>

class FilterByType : public QAbstractMidiMessageFilter
{
public:
    explicit FilterByType(QString const& label)
    : QAbstractMidiMessageFilter(label)
    {
        addParameter(QObject::tr("Message type"), "",
                     [this](QVariant const& value)
        {
            auto const newType = value.value<QMidiMessage::Type>();
            auto const changed = m_type != newType;

            m_type = newType;
            return changed;
        },
                     [this]() -> QVariant
        {
            return QVariant::fromValue(m_type);
        });
    }

    bool filterMessage(QMidiMessage const& message) const override
    {
        return message.type() == m_type;
    }

    void setMessageType(QMidiMessage::Type const type)
    {
        m_type = type;
    }
private:
    QMidiMessage::Type m_type = QMidiMessage::Type::NoteOn;
};

class FilterNoteOn : public QAbstractMidiMessageFilter
{
public:
    using QAbstractMidiMessageFilter::QAbstractMidiMessageFilter;

    bool filterMessage(QMidiMessage const& message) const override
    {
        return message.type() == QMidiMessage::Type::NoteOn;
    }
};

class FilterNoteOff : public QAbstractMidiMessageFilter
{
public:
    using QAbstractMidiMessageFilter::QAbstractMidiMessageFilter;

    bool filterMessage(QMidiMessage const& message) const override
    {
        return message.type() == QMidiMessage::Type::NoteOff;
    }
};

MainWindow::MainWindow(QWidget* parent)
: QMainWindow(parent)
, m_trayIcon(new QSystemTrayIcon(this))
, m_midiManager(new QMidiManager(this))
, m_inputPortModel(m_midiManager->getInputDeviceModel())
, m_outputPortModel(m_midiManager->getOutputDeviceModel())
, m_midiMessageFilterFactory(m_midiManager->getMessageFilterFactory())
, m_messageModel(new QMidiMessageModel(this))
, m_messageSelection(new QItemSelectionModel(m_messageModel, this))
, m_messageView(new MidiMessageListView(m_messageModel, this))
, m_dockWidgets(new qool::DockWidgetManager(this))
, m_toolbars(new qool::ToolBarManager(this))
, m_manufacturerModel(new QMidiManufacturerModel(this))
, m_noteWidget(new MidiNoteTriggerWidget(this))
, m_keyboardWidget(new MidiKeyboardWidget(this))

, m_actionQuit(new QAction(tr("Quit"), this))
, m_actionClearAll(new QAction(QIcon(":/Images/Resources/Icons/Clear.png"), tr("Clear all"), this))
, m_actionAbout(new QAction(tr("About..."), this))
, m_actionSwitchAutoScrollToBottom(new QAction(QIcon(":/Images/Resources/Icons/ScrollDown.png"), tr("Auto scrolling"), this))
, m_actionRestoreWindow(new QAction(tr("Show"), this))
{
    // Test - TODO: remove
    m_midiMessageFilterFactory->add<FilterNoteOn>("Note On");
    m_midiMessageFilterFactory->add<FilterNoteOff>("Note Off");
    m_midiMessageFilterFactory->add<FilterByType>("Filter by type");

    setupUi();
    setupMIDI();
    setupActions();
    setupToolbars();
    setupMenus();
    setupTrayIcon();
    loadSettings();
    updateActions();
}

MainWindow::~MainWindow()
{
    saveSettings();
    m_midiManager->closeAllPorts();
}

void MainWindow::setupMIDI()
{
    resetMidiPorts();
    connect(m_midiManager, &QMidiManager::messageReceived, m_messageModel, &QMidiMessageModel::append);
    connect(m_midiManager, &QMidiManager::messageSent, m_messageModel, &QMidiMessageModel::append);
    m_manufacturerModel->load(QMidiManufacturerModel::LoadFromCSV(":/Texts/Resources/MIDI_Manufacturers.csv"));
}

void MainWindow::resetMidiPorts()
{
    auto const* const midiInModel = m_midiManager->getInputDeviceModel();
    auto const* const midiOutModel = m_midiManager->getOutputDeviceModel();

    if (midiInModel->rowCount() > 0 || midiOutModel->rowCount() > 0)
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

    m_midiManager->rescanPorts();
    m_midiManager->getInputDeviceModel()->add(m_noteWidget);
    m_midiManager->getInputDeviceModel()->add(m_keyboardWidget);
}

void MainWindow::setupActions()
{
    // Setup actions
    m_actionSwitchAutoScrollToBottom->setCheckable(true);
    m_actionSwitchAutoScrollToBottom->setChecked(true);

    connect(m_actionClearAll, &QAction::triggered, m_messageModel, &QMidiMessageModel::clear);
    connect(m_actionQuit, &QAction::triggered, this, &QMainWindow::close);
    connect(m_actionAbout, &QAction::triggered, this, &MainWindow::showAbout);
    connect(m_actionSwitchAutoScrollToBottom, &QAction::triggered, m_messageView, &MidiMessageListView::setAutoScrollToBottomEnabled);
}

void MainWindow::setupUi()
{
    // Setup message view
    m_messageView->setModel(m_messageModel);
    m_messageView->setSelectionModel(m_messageSelection);
    m_messageView->setItemDelegateForColumn(QMidiMessageModel::Columns::Type, new MidiMessageDelegates::MidiMessageTypeDelegate(this));
    m_messageView->setItemDelegateForColumn(QMidiMessageModel::Columns::Input, new MidiMessageDelegates::MidiInPortDelegate(m_inputPortModel, this));
    m_messageView->setItemDelegateForColumn(QMidiMessageModel::Columns::Channel, new MidiMessageDelegates::MidiChannelDelegate(this));
    m_messageView->setItemDelegateForColumn(QMidiMessageModel::Columns::Timestamp, new MidiMessageDelegates::MidiTimeDelegate(this));
    m_messageView->setItemDelegateForColumn(QMidiMessageModel::Columns::Value, new MidiMessageDelegates::MidiValueDelegate(m_messageModel, m_manufacturerModel, this));
    m_messageView->setItemDelegateForColumn(QMidiMessageModel::Columns::Data, new MidiMessageDelegates::MidiDataDelegate(m_messageModel, this));
    setCentralWidget(m_messageView);

    // Setup MIDI input port view
    MidiPortTreeView* midiInputPortView = new MidiPortTreeView(MidiPortTreeView::Mode::In, m_inputPortModel, m_midiMessageFilterFactory, this);

    CommonUi::standardTreeView(midiInputPortView, true);
    m_dockWidgets->addDockWidget(midiInputPortView, tr("MIDI Inputs"), "midi_input");

    // Setup MIDI output port view
    MidiPortTreeView* midiOutputPortView = new MidiPortTreeView(MidiPortTreeView::Mode::Out, m_outputPortModel, m_midiMessageFilterFactory, this);

    CommonUi::standardTreeView(midiOutputPortView, true);
    m_dockWidgets->addDockWidget(midiOutputPortView, tr("MIDI Outputs"), "midi_output");

    // Setup note widget
    m_dockWidgets->addDockWidget(m_noteWidget.get(), tr("MIDI Note Trigger"), "midi_note_trigger");

    // Setup keyboard widget
    m_dockWidgets->addDockWidget(m_keyboardWidget.get(), tr("MIDI Keyboard"), "midi_keyboard");

    // Setup matrix view
    MidiMatrixWidget* messageMatrixView = new MidiMatrixWidget(this);

    messageMatrixView->setModel(m_midiManager->getMessageMatrixModel());
    m_dockWidgets->addDockWidget(messageMatrixView, tr("MIDI Message Matrix"), "midi_message_matrix");

    // Setup window
    setAnimated(true);
    setUnifiedTitleAndToolBarOnMac(false);
}

void MainWindow::setupToolbars()
{
    QToolBar* const mainToolbar = m_toolbars->addToolBar(tr("Main"), "main");

    mainToolbar->setIconSize(QSize(16, 16));
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

    m_trayIcon->setIcon(QIcon(":/Images/Resources/Icons/Midi.png"));
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

    fileMenu->addAction(m_actionQuit);

    editMenu->addAction(m_actionClearAll);

    view->addAction(m_actionSwitchAutoScrollToBottom);

    windowMenu->addMenu(m_dockWidgets->controlMenu());
    windowMenu->addMenu(m_toolbars->controlMenu());

    helpMenu->addAction(m_actionAbout);
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
