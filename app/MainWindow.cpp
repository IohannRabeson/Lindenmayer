//
// Created by Io on 13/04/2018.
//

#include "MainWindow.hpp"

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
#include <QMessageBox>

#include <QtDebug>

MainWindow::MainWindow()
: m_dockWidgets(new qool::DockWidgetManager(this))
, m_statusBar(new QStatusBar(this))
, m_programTextEdit(new QPlainTextEdit(this))
, m_errorOutputTextEdit(new QPlainTextEdit(this))
, m_graphicsScene(new QGraphicsScene(this))
, m_graphicsView(new QGraphicsView(m_graphicsScene, this))
, m_turtle(m_graphicsScene)
, m_documentOnDisk(tr("Untitled %0"))
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

    connect(&m_documentOnDisk, &qool::DocumentOnDisk::filePathChanged, this, &MainWindow::setWindowFilePath);
    connect(&m_documentOnDisk, &qool::DocumentOnDisk::modifiedChanged, this, &MainWindow::setWindowModified);
    connect(&m_documentOnDisk, &qool::DocumentOnDisk::modifiedChanged, this, &MainWindow::onDocumentModified);

    connect(m_programTextEdit, &QPlainTextEdit::textChanged, &m_documentOnDisk, &qool::DocumentOnDisk::modified);
    newProgram();
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
    setCentralWidget(m_graphicsView);
    setStatusBar(m_statusBar);
}
// I'm using lambdas here Instead of using qOverload<>
// because otherwise MSVC does not support empty template parameters?.
void MainWindow::setupActions()
{
    connect(m_actionDraw, &QAction::triggered, this, &MainWindow::draw);
    connect(m_actionExportImage, &QAction::triggered, [this]() { exportImage(); });
    connect(m_actionClearErrors, &QAction::triggered, [this]()
    {
        m_errorOutputTextEdit->clear();
        updateActions();
    });

    m_actionNewProgram->setShortcut(QKeySequence::New);
    m_actionSaveProgram->setShortcut(QKeySequence::Save);
    m_actionSaveProgramAs->setShortcut(QKeySequence::SaveAs);
    m_actionLoadProgram->setShortcut(QKeySequence::Open);

    connect(m_actionNewProgram, &QAction::triggered, this, &MainWindow::newProgram);
    connect(m_actionSaveProgram, &QAction::triggered, this, &MainWindow::saveProgram);
    connect(m_actionSaveProgramAs, &QAction::triggered, this, [this]() { saveProgramAs(); });
    connect(m_actionLoadProgram, &QAction::triggered, this, [this]() { loadProgram(); });
    connect(m_actionZoomToFit, &QAction::triggered, this, &MainWindow::zoomToFit);
    connect(m_actionZoomReset, &QAction::triggered, this, &MainWindow::zoomReset);
}

void MainWindow::setupMenus()
{
    QMenu* const fileMenu = menuBar()->addMenu(tr("File"));

    fileMenu->addAction(m_actionNewProgram);
    fileMenu->addAction(m_actionLoadProgram);
    fileMenu->addAction(m_actionSaveProgram);
    fileMenu->addAction(m_actionSaveProgramAs);
    fileMenu->addAction(m_actionExportImage);
}

void MainWindow::setupToolbars()
{
    QToolBar* const toolbar = addToolBar(tr("Main"));

    toolbar->addAction(m_actionDraw);
    toolbar->addSeparator();
    toolbar->addAction(m_actionZoomReset);
    toolbar->addAction(m_actionZoomToFit);
}

void MainWindow::newProgram()
{
    if (maybeSave())
    {
        m_programTextEdit->clear();
        m_documentOnDisk.newDocument();
        m_needToBeBuilded = true;
        updateActions();
    }
}

bool MainWindow::loadProgram()
{
    bool result = false;

    if (maybeSave())
    {
        auto const filePath = QFileDialog::getOpenFileName(this, tr("Load L-Code program"));

        result = loadProgram(filePath);
    }
    return result;
}

bool MainWindow::loadProgram(QString const& filePath)
{
    bool result = false;

    if (maybeSave() && !filePath.isEmpty())
    {
        QFile file(filePath);

        if (file.open(QFile::OpenModeFlag::ReadOnly))
        {
            QTextStream stream(&file);
            QString const text = stream.readAll();

            m_programTextEdit->setPlainText(text);
            m_turtle.reset();
            m_documentOnDisk.opened(file.fileName());
            m_needToBeBuilded = true;
            updateActions();
            result = true;
        }
    }
    return result;
}

bool MainWindow::saveProgram()
{
    bool result = false;

    if (m_documentOnDisk.isOnDisk())
    {
        result = writeProgram(m_documentOnDisk.absoluteFilePath());
    }
    else
    {
        result = saveProgramAs();
    }

    return result;
}

bool MainWindow::saveProgramAs()
{
    auto const filePath = QFileDialog::getSaveFileName(this, tr("Save L-Code program"), m_documentOnDisk.directory().path());

    return saveProgramAs(filePath);
}

bool MainWindow::saveProgramAs(QString const& filePath)
{
    auto result = false;

    if (!filePath.trimmed().isEmpty())
    {
        if (writeProgram(filePath))
        {
            m_documentOnDisk.savedAs(filePath);
            result = true;
        }
    }
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
    build();
    // Execute turtle orders
    m_graphicsScene->clear();
    m_turtle.reset();
    m_turtle.setPen(QPen(Qt::black, 2.));
    m_program.execute();
    m_graphicsView->ensureVisible(getBoundingRectangle());
    updateActions();
}

bool MainWindow::build()
{
    if (!m_needToBeBuilded)
    {
        return true;
    }

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

        // If they are an error m_needToBeBuilded is set to false so
        // the next call to build will not trigger a build again.
        m_needToBeBuilded = false;
        return false;
    }

    m_program.rewrite();

    updateActions();

    m_needToBeBuilded = false;

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
    auto const haveModules = m_documentOnDisk.isLoaded();
    auto const haveProgram = m_programTextEdit->toPlainText().trimmed().size() > 0;

    m_actionDraw->setEnabled(haveProgram && haveModules);
    m_actionSaveProgram->setEnabled(m_documentOnDisk.isModified() && m_documentOnDisk.isOnDisk());
    m_actionSaveProgramAs->setEnabled(m_documentOnDisk.isModified() && !m_documentOnDisk.isOnDisk());
}

unsigned int MainWindow::getIterations() const
{
    return m_program.content().iterations.get_value_or(0u);
}

qreal MainWindow::getDistance() const
{
    return m_program.content().distance.get_value_or(0.);
}

qreal MainWindow::getAngle() const
{
    return m_program.content().angle.get_value_or(0.);
}

QRectF MainWindow::getBoundingRectangle() const
{
    return m_graphicsScene->itemsBoundingRect().adjusted(-4, -4, 4, 4).toRect();
}

bool MainWindow::writeProgram(QString const& filePath)
{
    bool result = false;

    if (!filePath.trimmed().isEmpty())
    {
        QFile file(filePath);

        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream stream(&file);

            stream << m_programTextEdit->toPlainText();
            qDebug() << "Write file:" << filePath;
            result = true;
        }
        else
        {
            qWarning() << "MainWindow::saveProgram: failed to save " << filePath << file.isOpen() << file.errorString();
        }
        updateActions();
    }

    return result;
}

bool MainWindow::maybeSave()
{
    bool result = false;

    if (m_documentOnDisk.isModified())
    {
        QMessageBox message;

        message.setText(QObject::tr("The document '%0' has been modified.\n"
                                     "Do you want to save your changes?").arg(m_documentOnDisk.documentName()));
        message.setStandardButtons(QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);
        switch (message.exec())
        {
            case QMessageBox::Save:
                result = saveProgram();
                break;
            case QMessageBox::No:
                result = true;
                break;
            case QMessageBox::Cancel:
                result = false;
                break;
            default:
                break;
        }
    }
    else
    {
        result = true;
    }

    return result;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    event->setAccepted(maybeSave());
}

void MainWindow::onDocumentModified(bool const modified)
{
    if (modified)
    {
        m_needToBeBuilded = true;
    }
}
