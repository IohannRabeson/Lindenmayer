//
// Created by Io on 01/01/2018.
//

#include "MainWindow.hpp"
#include "Ui/MidiMessageListView.hpp"
#include "Ui/DockWidgetManager.hpp"
#include "Ui/ToolBarManager.hpp"
#include "Ui/DeviceSchemeWidget.hpp"
#include "Ui/CommonUi.hpp"
#include "Ui/AboutMidiMonitorDialog.hpp"
#include "Ui/SettingsUtils.hpp"

#include "Plugins/Waldorf/Pulse2/Pulse2Scheme.hpp"

#include "Delegates/MidiDelegates.hpp"

#include <QComboBox>
#include <QToolBar>
#include <QMenuBar>
#include <QLineEdit>
#include <QStylePainter>
#include <QMessageBox>
#include <QTableView>
#include <QHeaderView>
#include <QtDebug>

#include <QDeviceSchemeFactory.hpp>
#include <QMidiIn.hpp>
#include <QMidiMessageModel.hpp>
#include <QMidiManager.hpp>

#include <QMetaEnum>

namespace
{
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
, m_midiManager(new QMidiManager(this))
, m_deviceSchemeFactory(new QDeviceSchemeFactory(this))
, m_inputPortModel(m_midiManager->getInputDeviceModel())
, m_messageModel(new QMidiMessageModel(this))
, m_messageSelection(new QItemSelectionModel(m_messageModel, this))
, m_messageView(new MidiMessageListView(m_messageModel, this))
, m_dockWidgets(new DockWidgetManager(this))
, m_toolbars(new ToolBarManager(this))
, m_manufacturerModel(new QMidiManufacturerModel(this))

, m_actionRescanMidiPorts(new QAction(tr("Rescan midi ports"), this))
, m_actionQuit(new QAction(tr("Quit"), this))
, m_actionClearAll(new QAction(tr("Clear all"), this))
, m_actionAbout(new QAction(tr("About...")))
, m_actionSwitchAutoScrollToBottom(new QAction(tr("Auto scrolling")))
{
    setupSystem();
    setupActions();
    setupToolbars();
    setupMenus();
    setupUi();
    loadSettings();
}

MainWindow::~MainWindow()
{
    closeAllPorts();
    saveSettings();
}

void MainWindow::setupSystem()
{
    // Setup scheme factory
    m_deviceSchemeFactory->add<Pulse2Scheme>("Pulse 2");
    m_midiManager->resetMidiInPorts();
    connect(m_inputPortModel, &QMidiDeviceModel::checkedChanged, this, &MainWindow::onInputPortEnabled);
    connect(m_midiManager, &QMidiManager::messageReceived, m_messageModel, &QMidiMessageModel::append);
    m_manufacturerModel->load(QMidiManufacturerModel::LoadFromCSV(":/Texts/Resources/MIDI_Manufacturers.csv"));
}

void MainWindow::resetMidiInputs()
{
    auto const* const midiInModel = m_midiManager->getInputDeviceModel();

    if (midiInModel->rowCount() > 0)
    {
        if (QMessageBox::warning(this, tr("Clear messages"),
                                 tr("Messages will be deleted.\nAre you sure you want to do that?"),
                                 QMessageBox::StandardButton::Yes,
                                 QMessageBox::StandardButton::No) == QMessageBox::StandardButton::No)
        {
            // Abort action.
            return;
        }
    }
    m_midiManager->resetMidiInPorts();
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

    // Setup window
    setAnimated(true);
    setUnifiedTitleAndToolBarOnMac(false);
}

void MainWindow::setupToolbars()
{
    QToolBar* const mainToolbar = m_toolbars->addToolBar(tr("Main"));
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

void MainWindow::closeAllPorts()
{
    m_midiManager->closeAll();
}

void MainWindow::onInputPortEnabled(int const portId, bool const enabled)
{
    m_midiManager->setInputPortEnabled(portId, enabled);
}

void MainWindow::saveSettings() const
{
    QSettings settings;

    qDebug() << "[MainWindow] Save settings:" << settings.fileName();
    settings.beginGroup("main_window");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
    settings.endGroup();

    settings.beginGroup("message_view");
    settings.setValue("geometry", m_messageView->saveGeometry());
    settings.beginGroup("header_view");
    settings.setValue("geometry", m_messageView->header()->saveGeometry());
    settings.setValue("state", m_messageView->header()->saveState());
    settings.endGroup();
    settings.endGroup();
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
}

void MainWindow::showAbout()
{
    AboutMidiMonitorDialog dialog(this);

    dialog.exec();
}
