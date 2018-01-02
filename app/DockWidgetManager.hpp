#ifndef DOCKWIDGETMANAGER_HPP
#define DOCKWIDGETMANAGER_HPP
#include <QObject>
#include <QDockWidget>
#include <QScopedPointer>

class QMenu;
class QDockWidget;
class QMainWindow;
class QSettings;

class DockWidgetManagerPrivate;

class DockWidgetManager : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(DockWidgetManager)
public:
    explicit DockWidgetManager(QMainWindow* parent, QString const& menuTitle = tr("Dockable widgets"));
    ~DockWidgetManager();

    QDockWidget* addDockWidget(QWidget* widget, QString const& dockTitle, bool addToMenu = true, QDockWidget::DockWidgetFeatures features = QDockWidget::AllDockWidgetFeatures, Qt::DockWidgetAreas allowedAreas = Qt::AllDockWidgetAreas, bool show = true);
    void addDockWidget(QDockWidget* dockWidget, QString const& title, bool addToMenu = true, QDockWidget::DockWidgetFeatures features = QDockWidget::AllDockWidgetFeatures, Qt::DockWidgetAreas allowedAreas = Qt::AllDockWidgetAreas, bool show = true);
    void removeDockWidget(QDockWidget* dockwidget);
    void destroyDockWidget(QDockWidget* dockwidget);

    QMenu* controlMenu()const;
private:
    QScopedPointer<DockWidgetManagerPrivate> d_ptr;
};

#endif // DOCKWIDGETMANAGER_HPP
