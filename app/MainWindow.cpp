//
// Created by Io on 01/01/2018.
//

#include "MainWindow.hpp"
#include "MidiMessageListView.hpp"
#include "DockWidgetManager.hpp"
#include "DeviceSchemeWidget.hpp"

#include "Plugins/Waldorf/Pulse2/Pulse2Scheme.hpp"

#include <QComboBox>
#include <QToolBar>
#include <QMenuBar>
#include <QLineEdit>
#include <QStylePainter>

#include <QDeviceSchemeFactory.hpp>
#include <QMidiIn.hpp>
#include <QMidiPortModel.hpp>
#include <QMidiMessageModel.hpp>

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
, m_midiIn(new QMidiIn(this))
, m_deviceSchemeFactory(new QDeviceSchemeFactory(this))
, m_inputPortModel(new QMidiPortModel(this))
, m_messageModel(new QMidiMessageModel(this))
, m_messageSelection(new QItemSelectionModel(m_messageModel, this))
, m_messageView(new MidiMessageListView(m_messageModel, this))
, m_dockWidgets(new DockWidgetManager(this))
, m_inputPortSelector(new ComboBox(tr("MIDI port"), this))
, m_schemeSelector(new ComboBox(tr("Scheme"), this))

, m_actionClearAll(new QAction(tr("Clear all"), this))
, m_actionQuit(new QAction(tr("Quit"), this))
, m_actionSwitchAutoScrollToBottom(new QAction(tr("Auto scrolling")))
{
    setupSystem();
    setupActions();
    setupToolbars();
    setupMenus();
    setupUi();

    // Default setup
    setScheme(m_deviceSchemeFactory->defaultScheme());
    setInputPort(m_inputPortModel->defaultPort());
}

void MainWindow::setupSystem()
{
    // Setup scheme factory
    m_deviceSchemeFactory->add<Pulse2Scheme>("Pulse 2");

    // Setup midi ports
    m_inputPortModel->rescan(m_midiIn);
}

void MainWindow::setupActions()
{
    // Setup actions
    m_actionSwitchAutoScrollToBottom->setCheckable(true);
    m_actionSwitchAutoScrollToBottom->setChecked(true);

    connect(m_actionClearAll, &QAction::triggered, m_messageModel, &QMidiMessageModel::clear);
    connect(m_actionQuit, &QAction::triggered, this, &QMainWindow::close);
    connect(m_actionSwitchAutoScrollToBottom, &QAction::triggered, m_messageView, &MidiMessageListView::setAutoScrollToBottomEnabled);
}

void MainWindow::setupUi()
{
    // Setup port selector
    m_inputPortSelector->setModel(m_inputPortModel);
    m_schemeSelector->setModel(m_deviceSchemeFactory);

    connect(m_inputPortSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, &MainWindow::setInputPort);
    connect(m_schemeSelector, qOverload<int>(&QComboBox::currentIndexChanged), [this](int const row)
    {
        setScheme(m_deviceSchemeFactory->index(row));
    });

    // Setup message view
    connect(m_midiIn, &QMidiIn::messageReceived, m_messageModel, &QMidiMessageModel::append);

    m_messageView->setModel(m_messageModel);
    m_messageView->setSelectionModel(m_messageSelection);
    setCentralWidget(m_messageView);

    // Setup window
    setAnimated(true);
    setUnifiedTitleAndToolBarOnMac(false);
}

void MainWindow::setupToolbars()
{
    QToolBar* const mainToolbar = addToolBar(tr("Main"));

    mainToolbar->addWidget(m_inputPortSelector);
    mainToolbar->addWidget(m_schemeSelector);
}

void MainWindow::setupMenus()
{
    QMenuBar* const bar = menuBar();
    QMenu* const fileMenu = bar->addMenu(tr("File"));
    QMenu* const view = bar->addMenu(tr("View"));
    QMenu* const windowMenu = bar->addMenu(tr("Window"));

    fileMenu->addAction(m_actionClearAll);
    fileMenu->addAction(m_actionQuit);

    view->addAction(m_actionSwitchAutoScrollToBottom);

    windowMenu->addMenu(m_dockWidgets->controlMenu());
}

void MainWindow::appendMessage(QMidiMessage const& message)
{
    m_messageModel->append(message);
}

void MainWindow::clearMessages()
{
    m_messageModel->clear();
}

void MainWindow::setInputPort(int const port)
{
    if (port == -1)
    {
        close();
    }
    else if (m_midiIn->portOpened() != port)
    {
        m_midiIn->openPort(port);
        m_inputPortSelector->setCurrentIndex(port);
    }
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
