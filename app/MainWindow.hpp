//
// Created by Io on 13/04/2018.
//

#ifndef LINDENMAYER_MAINWINDOW_HPP
#define LINDENMAYER_MAINWINDOW_HPP
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QAction>

#include <Qool/DockWidgetManager.hpp>

#include <ModuleTable.hpp>

#include "GraphicsSceneTurtle2D.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();

    bool saveInput();
    bool loadInput();
private:
    void setupWidgets();
    void setupActions();
    void setupToolbars();
    void setupMenus();

    unsigned int getIterations() const;
    qreal getDistance() const;
    qreal getAngle() const;
private:
    qool::DockWidgetManager* const m_dockWidgets = new qool::DockWidgetManager(this);
    QPlainTextEdit* const m_programTextEdit = new QPlainTextEdit(this);
    QPlainTextEdit* const m_errorOutputTextEdit = new QPlainTextEdit(this);
    QSpinBox* const m_iterationSelector = new QSpinBox(this);
    QDoubleSpinBox* const m_distanceSelector = new QDoubleSpinBox(this);
    QDoubleSpinBox* const m_angleSelector = new QDoubleSpinBox(this);

    QGraphicsScene* const m_graphicsScene = new QGraphicsScene(this);
    QGraphicsView* const m_graphicsView = new QGraphicsView(m_graphicsScene, this);

    QAction* const m_saveProgram = new QAction(tr("Save..."), this);
    QAction* const m_loadProgram = new QAction(tr("Open..."), this);
    QAction* const m_actionBuild = new QAction(tr("Build"), this);
    QAction* const m_actionClearErrors = new QAction(tr("Clear errors"), this);

    lcode::ModuleTable m_moduleTable;
    GraphicsSceneTurtle2D m_turtle;
};


#endif //LINDENMAYER_MAINWINDOW_HPP
