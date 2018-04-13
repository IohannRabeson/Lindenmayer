//
// Created by Io on 13/04/2018.
//

#include "MainWindow.hpp"

#include <Program.hpp>

#include <QToolBar>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMenuBar>
#include <QMenu>

MainWindow::MainWindow()
: m_turtle(m_graphicsScene)
{
    m_moduleTable.registerModule("F", [this](){ m_turtle.advance(getDistance(), true); });
    m_moduleTable.registerModule("f", [this](){ m_turtle.advance(getDistance(), false); });
    m_moduleTable.registerModule("L", [this](){ m_turtle.rotate(-getAngle()); });
    m_moduleTable.registerModule("R", [this](){ m_turtle.rotate(getAngle()); });
    m_moduleTable.registerModule("[", [this](){ m_turtle.push(); });
    m_moduleTable.registerModule("]", [this](){ m_turtle.pop(); });

    setupWidgets();
    setupActions();
    setupToolbars();
    setupMenus();
}

bool MainWindow::saveInput()
{
    auto result = false;
    auto const filePath = QFileDialog::getSaveFileName(this, tr("Save L-Code program"));

    if (!filePath.trimmed().isEmpty())
    {
        QFile file(filePath);

        if (file.open(QFile::OpenModeFlag::WriteOnly | QFile::OpenModeFlag::Text | QFile::OpenModeFlag::Truncate))
        {
            QTextStream stream(&file);

            stream << m_programTextEdit->toPlainText();
        }
    }
    return result;
}

bool MainWindow::loadInput()
{
    auto result = false;
    auto const filePath = QFileDialog::getOpenFileName(this, tr("Load L-Code program"));

    if (!filePath.trimmed().isEmpty())
    {
        QFile file(filePath);

        if (file.open(QFile::OpenModeFlag::ReadOnly))
        {
            QTextStream stream(&file);
            QString const text = stream.readAll();

            m_programTextEdit->setPlainText(text);
            result = true;
        }
    }
    return result;
}

void MainWindow::setupWidgets()
{
    m_errorOutputTextEdit->setReadOnly(true);
    m_dockWidgets->addDockWidget(m_programTextEdit, tr("Program"), "program_input");
    m_dockWidgets->addDockWidget(m_errorOutputTextEdit, tr("Errors"), "errors_output");
    m_iterationSelector->setRange(1, 64);
    m_distanceSelector->setRange(1, 1024);
    m_angleSelector->setRange(-360, 360);
    setCentralWidget(m_graphicsView);
}

void MainWindow::setupActions()
{
    connect(m_actionBuild, &QAction::triggered, [this]()
    {
        auto const text = m_programTextEdit->toPlainText().toUtf8().toStdString();

        lcode::Program program;

        auto const errors = program.loadFromLCode(text, m_moduleTable);

        m_errorOutputTextEdit->clear();

        for (auto const error : errors)
        {
            m_errorOutputTextEdit->appendPlainText(tr(" - Error [%0;%1]: %2").arg(error.line).arg(error.charIndex).arg(QString::fromStdString(error.message)));
        }

        m_graphicsScene->clear();
        m_turtle.reset();

        if (errors.empty())
        {
            auto const modules = program.rewrite(getIterations());

            m_moduleTable.execute(modules);

            m_graphicsView->ensureVisible(m_graphicsScene->itemsBoundingRect().adjusted(-4, -4, 4, 4));
        }
    });

    connect(m_actionClearErrors, &QAction::triggered, [this]()
    {
        m_errorOutputTextEdit->clear();
    });

    connect(m_saveProgram, &QAction::triggered, this, &MainWindow::saveInput);
    connect(m_loadProgram, &QAction::triggered, this, &MainWindow::loadInput);
}

void MainWindow::setupMenus()
{
    QMenu* const fileMenu = menuBar()->addMenu(tr("File"));

    fileMenu->addAction(m_loadProgram);
    fileMenu->addAction(m_saveProgram);
}

void MainWindow::setupToolbars()
{
    QToolBar* const toolbar = addToolBar(tr("Main"));

    toolbar->addWidget(m_iterationSelector);
    toolbar->addWidget(m_distanceSelector);
    toolbar->addWidget(m_angleSelector);
    toolbar->addSeparator();
    toolbar->addAction(m_actionBuild);
}

unsigned int MainWindow::getIterations() const
{
    return static_cast<unsigned int>(m_iterationSelector->value());
}

qreal MainWindow::getDistance() const
{
    return m_distanceSelector->value();
}

qreal MainWindow::getAngle() const
{
    return m_angleSelector->value();
}