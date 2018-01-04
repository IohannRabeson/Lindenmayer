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

#include "Plugins/Waldorf/Pulse2/Pulse2Scheme.hpp"

#include "Delegates/MidiDelegates.hpp"

#include <QComboBox>
#include <QToolBar>
#include <QMenuBar>
#include <QLineEdit>
#include <QStylePainter>
#include <QSettings>
#include <QtDebug>

#include <QDeviceSchemeFactory.hpp>
#include <QMidiIn.hpp>
#include <QMidiDeviceModel.hpp>
#include <QMidiMessageModel.hpp>
#include <QMidiManufacturerModel.hpp>
#include <Ui/Format.hpp>

#include <QStyledItemDelegate>
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
, m_deviceSchemeFactory(new QDeviceSchemeFactory(this))
, m_inputPortModel(new QMidiDeviceModel(this))
, m_messageModel(new QMidiMessageModel(this))
, m_messageSelection(new QItemSelectionModel(m_messageModel, this))
, m_messageView(new MidiMessageListView(m_messageModel, this))
, m_dockWidgets(new DockWidgetManager(this))
, m_toolbars(new ToolBarManager(this))
, m_schemeSelector(new ComboBox(tr("Scheme"), this))
, m_manufacturerModel(new QMidiManufacturerModel(this))

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

    // Default setup
    setScheme(m_deviceSchemeFactory->defaultScheme());
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

    resetMidiInputs();

    connect(m_inputPortModel, &QMidiDeviceModel::checkedChanged, this, &MainWindow::onPortEnabled);

    m_manufacturerModel->load(QMidiManufacturerModel::LoadFromCSV(":/Texts/Resources/MIDI_Manufacturers.csv"));
}

void MainWindow::resetMidiInputs()
{
    qDeleteAll(m_midiIns);
    m_midiIns.clear();

    // Instanciate the first MIDI port, then scans available ports.
    // Wierd but seem to be mandatory.
    // TODO: looking for an alternative to RtMidi?
    m_midiIns.append(new QMidiIn(this));
    m_inputPortModel->rescan(m_midiIns.front());

    // Instanciate midi other inputs
    for (int i = 0; i < m_inputPortModel->rowCount() - 1; ++i)
    {
        auto* midiIn = new QMidiIn(this);

        m_midiIns.append(midiIn);
    }

    if (m_midiIns.isEmpty())
    {
        qWarning() << "[MidiMonitor]: No midi inputs";
    }
    else
    {
        // Open midi inputs
        for (int i = 0; i < m_midiIns.size(); ++i)
        {
            auto* const midiIn = m_midiIns[i];

            connect(midiIn, &QMidiIn::messageReceived, m_messageModel, &QMidiMessageModel::append);

            midiIn->openPort(i);
        }
    }
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
    // Setup scheme selector
    m_schemeSelector->setModel(m_deviceSchemeFactory);

    connect(m_schemeSelector, qOverload<int>(&QComboBox::currentIndexChanged), [this](int const row)
    {
        setScheme(m_deviceSchemeFactory->index(row));
    });

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

    mainToolbar->addWidget(m_schemeSelector);
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

void MainWindow::appendMessage(QMidiMessage const& message)
{
    m_messageModel->append(message);
}

void MainWindow::clearMessages()
{
    m_messageModel->clear();
}

void MainWindow::setScheme(QModelIndex const& index)
{
    if (index.isValid() && (!m_currentSchemeIndex.isValid() || index != m_currentSchemeIndex))
    {
        m_currentSchemeIndex = index;
        m_messageModel->setScheme(m_deviceSchemeFactory->create(index));
        m_schemeSelector->setCurrentIndex(index.row());
    }
}

void MainWindow::closeAllPorts()
{
    for (auto* midiIn : m_midiIns)
    {
        midiIn->closePort();
    }
}

void MainWindow::onPortEnabled(int const portId, bool const enabled)
{
    for (auto* midiIn : m_midiIns)
    {
        if (midiIn->portOpened() == portId)
        {
            midiIn->setEnabled(enabled);
            return;
        }
    }
}

void MainWindow::saveSettings() const
{
    QSettings settings;

    qDebug() << "[MainWindow] Save settings:" << settings.fileName();
    settings.beginGroup("main_window");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
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
        restoreGeometry(settings.value("geometry").value<QByteArray>());
        restoreState(settings.value("state").value<QByteArray>());
    }
    settings.endGroup();
}

void MainWindow::showAbout()
{
    AboutMidiMonitorDialog dialog(this);

    dialog.exec();
}
