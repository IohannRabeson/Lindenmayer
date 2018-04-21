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
#include <QFileDialog>
#include <QStatusBar>
#include <QPlainTextEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QtGlobal>

#include <QtDebug>

MainWindow::MainWindow()
: m_dockWidgets(new qool::DockWidgetManager(this))
, m_statusBar(new QStatusBar(this))
, m_programTextEdit(new QPlainTextEdit(this))
, m_errorOutputTextEdit(new QPlainTextEdit(this))
, m_iterationSelector(new QSpinBox(this))
, m_distanceSelector(new QDoubleSpinBox(this))
, m_angleSelector(new QDoubleSpinBox(this))
, m_graphicsScene(new QGraphicsScene(this))
, m_graphicsView(new QGraphicsView(m_graphicsScene, this))
, m_turtle(m_graphicsScene)
{
    m_moduleTable.registerModule("F", [this](){ m_turtle.advance(getDistance(), true); });
    m_moduleTable.registerModule("f", [this](){ m_turtle.advance(getDistance(), false); });
    m_moduleTable.registerModule("-", [this](){ m_turtle.rotate(-getAngle()); });
    m_moduleTable.registerModule("+", [this](){ m_turtle.rotate(getAngle()); });
    m_moduleTable.registerModule("[", [this](){ m_turtle.push(); });
    m_moduleTable.registerModule("]", [this](){ m_turtle.pop(); });

    setupWidgets();
    setupActions();
    setupToolbars();
    setupMenus();
    updateActions();
}

void MainWindow::setupWidgets()
{
    auto const fontId = QFontDatabase::addApplicationFont("://Resources/Menlo-Regular.ttf");
    QString const family = QFontDatabase::applicationFontFamilies(fontId).front();
    QFont const monospaced(family);

    m_programTextEdit->setFont(monospaced);
    m_errorOutputTextEdit->setFont(monospaced);
    m_errorOutputTextEdit->setReadOnly(true);
    m_dockWidgets->addDockWidget(m_programTextEdit, tr("Program"), "program_input");
    m_dockWidgets->addDockWidget(m_errorOutputTextEdit, tr("Errors"), "errors_output");
    m_iterationSelector->setRange(1, 64);
    m_distanceSelector->setRange(1, 1024);
    m_angleSelector->setRange(-360, 360);
    setCentralWidget(m_graphicsView);
    setStatusBar(m_statusBar);
}
// Instead of using qOverload<> I prefer to use lambdas because they are
// working with MSVC. qOverload does not compile with MSVC...
void MainWindow::setupActions()
{
    connect(m_actionBuild, &QAction::triggered, this, &MainWindow::build);

    connect(m_actionDraw, &QAction::triggered, this, &MainWindow::draw);
    connect(m_actionExportImage, &QAction::triggered, [this]() { exportImage(); });
    connect(m_actionClearErrors, &QAction::triggered, [this]()
    {
        m_errorOutputTextEdit->clear();
        updateActions();
    });

    connect(m_actionSaveProgram, &QAction::triggered, this, &MainWindow::saveInput);
    connect(m_actionLoadProgram, &QAction::triggered, this, [this]() { loadProgram(); });
    connect(m_actionZoomToFit, &QAction::triggered, this, &MainWindow::zoomToFit);
    connect(m_actionZoomReset, &QAction::triggered, this, &MainWindow::zoomReset);
}

void MainWindow::setupMenus()
{
    QMenu* const fileMenu = menuBar()->addMenu(tr("File"));

    fileMenu->addAction(m_actionLoadProgram);
    fileMenu->addAction(m_actionSaveProgram);
    fileMenu->addAction(m_actionExportImage);
}

void MainWindow::setupToolbars()
{
    QToolBar* const toolbar = addToolBar(tr("Main"));

    toolbar->addWidget(m_iterationSelector);
    toolbar->addWidget(m_distanceSelector);
    toolbar->addWidget(m_angleSelector);
    toolbar->addSeparator();
    toolbar->addAction(m_actionBuild);
    toolbar->addAction(m_actionDraw);
    toolbar->addSeparator();
    toolbar->addAction(m_actionZoomReset);
    toolbar->addAction(m_actionZoomToFit);
}

bool MainWindow::saveInput()
{
    auto result = false;
    auto const filePath = QFileDialog::getSaveFileName(this, tr("Save L-Code program"), m_saveDirectory.path()).trimmed();

    if (!filePath.trimmed().isEmpty())
    {
        QFile file(filePath);

        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream stream(&file);

            stream << m_programTextEdit->toPlainText();
            m_saveDirectory = QFileInfo(filePath).dir();
            qDebug() << "Write file:" << filePath;
        }
        else
        {
            qWarning() << "MainWindow::saveInput: failed to save " << filePath << file.isOpen() << file.errorString();
        }
    }
    updateActions();
    return result;
}

bool MainWindow::loadProgram()
{
    auto const filePath = QFileDialog::getOpenFileName(this, tr("Load L-Code program"));

    return loadProgram(filePath);
}

bool MainWindow::loadProgram(QString const& filePath)
{
    bool result = false;

    if (!filePath.isEmpty())
    {
        QFile file(filePath);

        if (file.open(QFile::OpenModeFlag::ReadOnly))
        {
            QTextStream stream(&file);
            QString const text = stream.readAll();

            m_programTextEdit->setPlainText(text);
            m_turtle.reset();
            result = true;
        }
    }
    updateActions();
    return result;
}

void MainWindow::exportImage()
{
    auto filePath = QFileDialog::getSaveFileName(this, tr("Export image"), m_imageExportDirectory.path(), "Images (*.png *.jpg)");

    exportImage(filePath);
}

void MainWindow::exportImage(QString const& filePath)
{
    if (filePath.isEmpty())
    {
        return;
    }

    QRect const itemBoundingRect = getBoundingRectangle().toRect();
    QRect const targetRect(0, 0, itemBoundingRect.width(), itemBoundingRect.height());

    QImage image(targetRect.width(), targetRect.height(), QImage::Format_RGB32);
    QPainter painter(&image);

    // TODO: allow to see a preview and allow to change background -> AKA custom image export dialog
    // With a custom dialog, it's worth to implement something to choose the image format too.
    image.fill(Qt::white);

    m_graphicsScene->render(&painter, targetRect, itemBoundingRect);

    if (image.save(filePath))
    {
        m_imageExportDirectory = QFileInfo(filePath).dir();
    }
    else
    {
        m_statusBar->showMessage(tr("Unable to export image '%0'").arg(filePath));
    }

    m_imageExportDirectory = QFileInfo(filePath).dir();

    updateActions();
}

void MainWindow::draw()
{
    // Execute turtle orders
    m_graphicsScene->clear();
    m_turtle.reset();
    m_turtle.setPen(QPen(Qt::black, 2.));
    m_program.execute(getIterations());
    m_graphicsView->ensureVisible(getBoundingRectangle());
    updateActions();
}

bool MainWindow::build()
{
    // Convert text to program content using ANTLR
    auto const text = m_programTextEdit->toPlainText().toUtf8().toStdString();

    auto const errors = m_program.loadFromLCode(text, m_moduleTable);

    // Print potential errors
    m_errorOutputTextEdit->clear();

    if (!errors.empty())
    {
        for (auto const error : errors)
        {
            m_errorOutputTextEdit->appendPlainText(tr(" - Error [%0;%1]: %2").arg(error.line).arg(error.charIndex).arg(QString::fromStdString(error.message)));
        }

        return false;
    }

    // Update global variables
    auto const& content = m_program.content();

    m_iterationSelector->setValue(static_cast<int>(content.iterations.value_or(1u)));
    m_distanceSelector->setValue(content.distance.value_or(1.f));
    m_angleSelector->setValue(content.angle.value_or(90.f));
    m_graphicsView->fitInView(m_graphicsScene->itemsBoundingRect(), Qt::KeepAspectRatio);

    updateActions();

    return true;
}

void MainWindow::zoomToFit()
{
    m_graphicsView->fitInView(m_graphicsScene->itemsBoundingRect(), Qt::KeepAspectRatio);
    updateActions();
}

void MainWindow::zoomReset()
{
    m_graphicsView->resetMatrix();
    updateActions();
}

void MainWindow::updateActions()
{
    auto const haveModules = m_program.loaded();
    auto const haveProgram = m_programTextEdit->toPlainText().trimmed().size() > 0;

    m_actionBuild->setEnabled(haveProgram);
    m_actionDraw->setEnabled(haveProgram && haveModules);
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

QRectF MainWindow::getBoundingRectangle() const
{
    return m_graphicsScene->itemsBoundingRect().adjusted(-4, -4, 4, 4).toRect();
}
