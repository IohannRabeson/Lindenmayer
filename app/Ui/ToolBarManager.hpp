#ifndef TOOLBARMANAGER_HPP
#define TOOLBARMANAGER_HPP

#include <QObject>
#include <QList>
#include <QScopedPointer>

class QMainWindow;
class QToolBar;
class QAction;
class QMenu;

class ToolBarManagerPrivate;

class ToolBarManager : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ToolBarManager)
public:
    explicit ToolBarManager(QMainWindow* mainWindow, QString const& menuTitle = tr("Toolbars"));
    ~ToolBarManager();

    void setDefaultAreas(Qt::ToolBarAreas const areas);

    QToolBar* addToolBar(QString const& title, QString const& objectName, bool addToMenu = true);
    QToolBar* addToolBar(QString const& title, QString const& objectName, QList<QAction*> const& actions, bool addToMenu);
    QToolBar* addToolBar(QToolBar* toolBar, QString const& title, QString const& objectName, bool addToMenu);
    void removeToolBar(QToolBar* toolBar);
    void destroyToolBar(QToolBar* toolBar);
    QMenu *controlMenu() const;
private:
    QScopedPointer<ToolBarManagerPrivate> d_ptr;
};

#endif // TOOLBARMANAGER_HPP
