#include "ToolBarManager.hpp"

#include <QMainWindow>
#include <QMenu>
#include <QPointer>
#include <QSet>
#include <QToolBar>

/*!
 * \class ToolBarManager
 * \brief Manages the tools bars of the applications.
 */

class ToolBarManagerPrivate
{
public:
    ToolBarManagerPrivate(QMainWindow* mainWindow, QString const& menuTitle) :
        m_mainWindow(mainWindow),
        m_menu(new QMenu(menuTitle))
    {
    }

    ~ToolBarManagerPrivate()
    {
        delete m_menu;
    }

    QMainWindow* const m_mainWindow;
    QPointer<QMenu> m_menu;
    QSet<QToolBar*> m_toolBars;
    Qt::ToolBarAreas m_defaultAreas = Qt::ToolBarArea::AllToolBarAreas;
};

ToolBarManager::ToolBarManager(QMainWindow* mainWindow, QString const& menuTitle) :
    QObject(mainWindow),
    d_ptr(new ToolBarManagerPrivate(mainWindow, menuTitle))
{
}

ToolBarManager::~ToolBarManager() = default;

/*!
 * \fn QToolBar* ToolBarManager::addToolBar(QString const& title, Qt::ToolBarArea allowedAreas, bool addToMenu)
 * \brief ToolBarManager::addToolBar
 * \param title title The title of the dockwidget used in the menu.
 * The title is also used to define the object name, used when an application stores main window settings.
 * The title can't be empty (spaces are not considered).
 * \param allowedAreas
 * \param addToMenu If true, an entry is added to the menu.
 * \return
 */
QToolBar *ToolBarManager::addToolBar(const QString &title, QString const& objectName, bool addToMenu)
{
    Q_ASSERT(title.trimmed().isEmpty() == false);

    QToolBar* const newToolBar = new QToolBar(title);

    return addToolBar(newToolBar, title, objectName, addToMenu);
}

/*!
 * \fn QToolBar* ToolBarManager::addToolBar(QString const& title, QList<QAction*> const& actions, Qt::ToolBarArea allowedAreas, bool addToMenu);
 * \brief ToolBarManager::addToolBar
 * \param title title title The title of the dockwidget used in the menu.
 * The title is also used to define the object name, used when an application stores main window settings.
 * The title can't be empty (spaces are not considered).
 * \param actions Actions added to the toolbar.
 * \param allowedAreas
 * \param addToMenu If true, an entry is added to the menu.
 * \return
 */
QToolBar *ToolBarManager::addToolBar(const QString &title, QString const& objectName, const QList<QAction *> &actions, bool addToMenu)
{
    Q_ASSERT(title.trimmed().isEmpty() == false);

    auto* const newToolBar = addToolBar(title, objectName, addToMenu);

    newToolBar->addActions(actions);
    return newToolBar;
}

/*!
 * \brief QToolBar *ToolBarManager::addToolBar(QToolBar *toolBar, QString const& title, Qt::ToolBarArea allowedAreas, bool addToMenu)
 * \param toolBar Toolbar to be added.
 * \param title title title The title of the dockwidget used in the menu.
 * The title is also used to define the object name, used when an application stores main window settings.
 * The title can't be empty (spaces are not considered).
 * \param allowedAreas
 * \param addToMenu If true, an entry is added to the menu.
 * \return
 */
QToolBar *ToolBarManager::addToolBar(QToolBar *toolBar, QString const& title, QString const& objectName, bool addToMenu)
{
    Q_ASSERT(toolBar != nullptr);
    Q_ASSERT(title.trimmed().isEmpty() == false);
    Q_D(ToolBarManager);

    if (!d->m_toolBars.contains(toolBar))
    {
        auto* const action = toolBar->toggleViewAction();

        if (addToMenu && d->m_menu)
        {
            action->setText(title);
            d->m_menu->addAction(action);
        }
        toolBar->setAllowedAreas(d->m_defaultAreas);
        toolBar->setObjectName("toolbar_" + objectName);
        d->m_toolBars.insert(toolBar);
        d->m_mainWindow->addToolBar(toolBar);
    }
    return toolBar;
}

/*!
 * \brief Remove then destroy a tool bar.
 * \param toolBar
 */
void ToolBarManager::removeToolBar(QToolBar *toolBar)
{
    Q_D(ToolBarManager);

    if (toolBar && d->m_toolBars.contains(toolBar))
    {
        d->m_toolBars.remove(toolBar);
        if (d->m_menu)
        {
            d->m_menu->removeAction(toolBar->toggleViewAction());
        }
        d->m_mainWindow->removeToolBar(toolBar);
    }
}

/*!
 * \brief Remove then destroy a tool bar.
 * \param toolBar
 */
void ToolBarManager::destroyToolBar(QToolBar *toolBar)
{
    removeToolBar(toolBar);
    toolBar->deleteLater();
}

/*!
 * \brief Get the menu controling the visibility of the managed toolbars.
 */
QMenu *ToolBarManager::controlMenu() const
{
    Q_D(const ToolBarManager);

    return (d->m_menu);
}

void ToolBarManager::setDefaultAreas(Qt::ToolBarAreas const areas)
{
    Q_D(ToolBarManager);

    d->m_defaultAreas = areas;
}
