//
// Created by Io on 01/01/2018.
//

#include "MainWindow.hpp"
#include "MidiMessageListView.hpp"
#include "DockWidgetManager.hpp"

#include <QComboBox>
#include <QToolBar>
#include <QMenuBar>
#include <QMenu>

#include <QDeviceSchemeFactory.hpp>
#include <QMidiIn.hpp>
#include <QMidiPortModel.hpp>
#include <QMidiMessageModel.hpp>

MainWindow::MainWindow(QWidget* parent)
: QMainWindow(parent)
, m_midiIn(new QMidiIn(this))
, m_deviceSchemeFactory(new QDeviceSchemeFactory(this))
, m_inputPortModel(new QMidiPortModel(this))
, m_messageModel(new QMidiMessageModel(this))
, m_messageSelectionModel(new QItemSelectionModel(m_messageModel))
, m_messageView(new MidiMessageListView(m_messageModel, this))
, m_dockWidgets(new DockWidgetManager(this))
, m_inputPortSelector(new QComboBox(this))

, m_actionClearAll(new QAction(tr("Clear all"), this))
, m_actionQuit(new QAction(tr("Quit"), this))
, m_actionSwitchAutoScrollToBottom(new QAction(tr("Auto scrolling")))
{
    // Setup actions
    m_actionSwitchAutoScrollToBottom->setCheckable(true);
    m_actionSwitchAutoScrollToBottom->setChecked(true);

    connect(m_actionClearAll, &QAction::triggered, this, &MainWindow::clearMessages);
    connect(m_actionQuit, &QAction::triggered, this, &MainWindow::close);
    connect(m_actionSwitchAutoScrollToBottom, &QAction::triggered, m_messageView, &MidiMessageListView::setAutoScrollToBottomEnabled);

    // Setup midi
    m_inputPortModel->rescan(m_midiIn);

    // Setup toolbar
    QToolBar* const mainToolbar = addToolBar(tr("Main"));

    mainToolbar->addWidget(m_inputPortSelector);

    // Setup menus
    QMenuBar* const bar = menuBar();
    QMenu* const fileMenu = bar->addMenu(tr("File"));
    QMenu* const view = bar->addMenu(tr("View"));
    QMenu* const windowMenu = bar->addMenu(tr("Window"));

    fileMenu->addAction(m_actionClearAll);
    fileMenu->addAction(m_actionQuit);

    view->addAction(m_actionSwitchAutoScrollToBottom);

    windowMenu->addMenu(m_dockWidgets->controlMenu());

    // Setup port selector
    m_inputPortSelector->setModel(m_inputPortModel);

    connect(m_inputPortSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, &MainWindow::setInputPort);

    // Setup message view
    connect(m_midiIn, &QMidiIn::messageReceived, m_messageModel, &QMidiMessageModel::append);

    m_messageView->setModel(m_messageModel);
    m_messageView->setSelectionModel(m_messageSelectionModel);
    setCentralWidget(m_messageView);

    // Default setup
    m_messageModel->setScheme(m_deviceSchemeFactory->createDefault());
    setInputPort(0);
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
    if (m_midiIn->portOpened() != port)
    {
        m_midiIn->openPort(port);
        m_inputPortSelector->setCurrentIndex(port);
    }
}
