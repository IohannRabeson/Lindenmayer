//
// Created by Io on 13/04/2018.
//

#ifndef LINDENMAYER_MAINWINDOW_HPP
#define LINDENMAYER_MAINWINDOW_HPP
#include <QMainWindow>
#include <QAction>
#include <QDir>

#include <Qool/DockWidgetManager.hpp>

#include <Program.hpp>

#include "GraphicsSceneTurtle2D.hpp"

class QStatusBar;
class QAction;
class QPlainTextEdit;
class QSpinBox;
class QDoubleSpinBox;
class QGraphicsScene;
class QGraphicsView;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();

    bool saveInput();
    bool loadInput();
    void exportImage();
    void draw();
    void build();
    void zoomToFit();
    void zoomReset();
private:
    void setupWidgets();
    void setupActions();
    void setupToolbars();
    void setupMenus();

    void updateActions();

    unsigned int getIterations() const;
    qreal getDistance() const;
    qreal getAngle() const;
private:
    qool::DockWidgetManager* const m_dockWidgets;
    QStatusBar* const m_statusBar;
    QPlainTextEdit* const m_programTextEdit;
    QPlainTextEdit* const m_errorOutputTextEdit;
    QSpinBox* const m_iterationSelector;
    QDoubleSpinBox* const m_distanceSelector;
    QDoubleSpinBox* const m_angleSelector;
    QGraphicsScene* const m_graphicsScene;
    QGraphicsView* const m_graphicsView;

    QAction* const m_actionSaveProgram = new QAction(tr("Save..."), this);
    QAction* const m_actionLoadProgram = new QAction(tr("Open..."), this);
    QAction* const m_actionExportImage = new QAction(tr("Export..."), this);
    QAction* const m_actionBuild = new QAction(tr("Build"), this);
    QAction* const m_actionDraw = new QAction(tr("Draw"), this);
    QAction* const m_actionClearErrors = new QAction(tr("Clear errors"), this);
    QAction* const m_actionZoomToFit = new QAction(tr("Zoom to fit"), this);
    QAction* const m_actionZoomReset = new QAction(tr("Zoor reset"), this);

    lcode::ModuleTable m_moduleTable;
    GraphicsSceneTurtle2D m_turtle;
    lcode::Program m_program;

    QDir m_imageExportDirectory;
    QDir m_saveDirectory;
};


#endif //LINDENMAYER_MAINWINDOW_HPP
