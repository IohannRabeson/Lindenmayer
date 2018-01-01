#include "DockWidgetManager.hpp"

#include <QDockWidget>
#include <QMainWindow>
#include <QMenu>
#include <QPointer>
#include <QSet>
#include <QSignalMapper>

/*!
 * \class DockWidgetManager
 * \brief Manage the dockable widgets of the application.
 */

class DockWidgetManagerPrivate
{
public:
    DockWidgetManagerPrivate(QMainWindow* mainWindow, QString const& menuTitle) :
        m_mainWindow(mainWindow),
        m_menu(new QMenu(menuTitle))
    {
    }

    ~DockWidgetManagerPrivate()
    {
        if (m_menu)
        {
            delete m_menu;
        }
    }

    QMainWindow* m_mainWindow;
    QPointer<QMenu> m_menu;
    QSet<QDockWidget*> m_dockWidgets;
};

DockWidgetManager::DockWidgetManager(QMainWindow *parent, QString const& menuTitle) :
    QObject(parent),
    d_ptr(new DockWidgetManagerPrivate(parent, menuTitle))
{
}

DockWidgetManager::~DockWidgetManager() = default;

/*!
 * \fn QDockWidget *DockWidgetManager::addDockWidget(QWidget *widget, QString const& title, bool addToMenu, QDockWidget::DockWidgetFeatures features, Qt::DockWidgetAreas allowedAreas)
 * \brief DockWidgetManager::addDockWidget
 * \param widget The widget used to create the new dock widget.
 * \param dockTitle The title of the dockwidget used in the menu.
 * The title is also used to define the object name, used when an application stores main window settings.
 * The title can't be empty (spaces are not considered).
 * \param addToMenu If true, an entry is added to the menu.
 * \param features
 * \param allowedAreas
 * \return
 */
QDockWidget *DockWidgetManager::addDockWidget(QWidget *widget, QString const& title, bool addToMenu, QDockWidget::DockWidgetFeatures features, Qt::DockWidgetAreas allowedAreas)
{
    Q_ASSERT(widget != nullptr);
    Q_ASSERT(title.trimmed().isEmpty() == false);
    Q_D(DockWidgetManager);
    QDockWidget* const dockWidget = new QDockWidget(title, d->m_mainWindow);

    dockWidget->setWidget(widget);
    dockWidget->hide();
    addDockWidget(dockWidget, title, addToMenu, features, allowedAreas);
    return (dockWidget);
}

/*!
 * \fn void DockWidgetManager::addDockWidget(QDockWidget *dockWidget, QString const& title, bool addToMenu, QDockWidget::DockWidgetFeatures features, Qt::DockWidgetAreas allowedAreas)
 * \brief DockWidgetManager::addDockWidget
 * \param dockWidget The dockwidget added
 * \param title The title of the dockwidget used in the menu.
 * The title is also used to define the object name, used when an application stores main window settings.
 * The title can't be empty (spaces are not considered).
 * \param addToMenu If true, an entry is added to the menu.
 * \param features
 * \param allowedAreas
 */
void DockWidgetManager::addDockWidget(QDockWidget *dockWidget, QString const& title, bool addToMenu, QDockWidget::DockWidgetFeatures features, Qt::DockWidgetAreas allowedAreas)
{
    Q_ASSERT(dockWidget != nullptr);
    Q_ASSERT(title.trimmed().isEmpty() == false);
    Q_D(DockWidgetManager);
    QAction* action = nullptr;

    if (d->m_dockWidgets.contains(dockWidget) == false)
    {
        dockWidget->setFeatures(features);
        dockWidget->setAllowedAreas(allowedAreas);
        action = dockWidget->toggleViewAction();
        if (addToMenu && d->m_menu)
        {
            action->setText(title);
            d->m_menu->addAction(action);
        }
        dockWidget->setObjectName("dockwidget_" + title.toLower());
        d->m_dockWidgets.insert(dockWidget);
        d->m_mainWindow->addDockWidget(Qt::RightDockWidgetArea, dockWidget);
    }
}

/*!
 * \brief DockWidgetManager::removeDockWidget
 * \param dockwidget Remove the dock widget from the main window.
 *
 * The dockable widget is not destroyed.
 */
void DockWidgetManager::removeDockWidget(QDockWidget *dockwidget)
{
    Q_D(DockWidgetManager);

    if (dockwidget && d->m_dockWidgets.contains(dockwidget))
    {
        d->m_dockWidgets.remove(dockwidget);
        if (d->m_menu)
        {
            d->m_menu->removeAction(dockwidget->toggleViewAction());
        }
        d->m_mainWindow->removeDockWidget(dockwidget);
    }
}

/*!
 * \brief Remove then destroy a dock widget.
 * \param dockwidget
 */
void DockWidgetManager::destroyDockWidget(QDockWidget *dockwidget)
{
    removeDockWidget(dockwidget);
    dockwidget->deleteLater();
}

/*!
 * \brief Get the menu controling the visibility of the managed dockable widgets.
 */
QMenu *DockWidgetManager::controlMenu() const
{
    Q_D(const DockWidgetManager);

    return (d->m_menu);
}
