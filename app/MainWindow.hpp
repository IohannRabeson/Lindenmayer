//
// Created by Io on 13/04/2018.
//

#ifndef LINDENMAYER_MAINWINDOW_HPP
#define LINDENMAYER_MAINWINDOW_HPP
#include <QMainWindow>
#include <QAction>
#include <QDir>

#include <Qool/DockWidgetManager.hpp>
#include <Qool/DocumentOnDisk.hpp>
#include <Qool/RecentFileMenu.hpp>
#include <Qool/ToolBarManager.hpp>

#include <ModuleTable.hpp>
#include <Program.hpp>

#include "GraphicsSceneTurtle2D.hpp"

class QStatusBar;
class QAction;
class QPlainTextEdit;
class QGraphicsScene;
class QGraphicsView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    static constexpr int const StateVersionNumber = 0u;
public:
    MainWindow();

    void newProgram();
    bool saveProgram();
    bool saveProgramAs(QString const& filePath);
    bool saveProgramAs();
    bool loadProgram();
    bool loadProgram(QString const& filePath);

    void exportImage();
    void exportImage(QString const& filePath);

    void draw();
    bool build();
    void zoomToFit();
    void zoomReset();
private:
    void setupWidgets();
    void setupActions();
    void setupToolbars();
    void setupMenus();

    bool writeProgram(QString const& filePath);
    bool maybeSave();

    void updateActions();

    qreal getDistance() const;
    qreal getAngle() const;

    QRectF getBoundingRectangle() const;

    void onDocumentModified(bool const modified);
protected:
    void closeEvent(QCloseEvent* event) override;
private:
    void saveSettings() const;
    void loadSettings();
private:
    qool::DockWidgetManager* const m_dockWidgets;
    QStatusBar* const m_statusBar;
    QPlainTextEdit* const m_programTextEdit;
    QPlainTextEdit* const m_errorOutputTextEdit;
    QGraphicsScene* const m_graphicsScene;
    QGraphicsView* const m_graphicsView;
    qool::RecentFileMenu* const m_recentFileMenu;
    qool::ToolBarManager* const m_toolbarManager;

    QAction* const m_actionNewProgram = new QAction(tr("New"), this);
    QAction* const m_actionSaveProgram = new QAction(tr("Save"), this);
    QAction* const m_actionSaveProgramAs = new QAction(tr("Save as..."), this);
    QAction* const m_actionLoadProgram = new QAction(tr("Open..."), this);
    QAction* const m_actionExportImage = new QAction(tr("Export..."), this);
    QAction* const m_actionDraw = new QAction(tr("Draw"), this);
    QAction* const m_actionClearErrors = new QAction(tr("Clear errors"), this);
    QAction* const m_actionZoomToFit = new QAction(tr("Zoom to fit"), this);
    QAction* const m_actionZoomReset = new QAction(tr("Zoom reset"), this);

    lcode::ModuleTable m_moduleTable;
    GraphicsSceneTurtle2D m_turtle;
    lcode::Program m_program;
    qool::DocumentOnDisk m_documentOnDisk;
    bool m_needToBeBuilded = false;

    QDir m_imageExportDirectory;
};


#endif //LINDENMAYER_MAINWINDOW_HPP
