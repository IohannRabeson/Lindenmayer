#include "MidiConsoleArea.hpp"
#include "Ui/Views/MidiConsoleView.hpp"

#include <QVBoxLayout>
#include <QMdiSubWindow>

#include <algorithm>

MidiConsoleArea::MidiConsoleArea(QWidget* parent)
: QMdiArea(parent)
{
    setViewMode(QMdiArea::ViewMode::TabbedView);
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    setTabsClosable(false);
    setDocumentMode(true);
}

void MidiConsoleArea::add(std::shared_ptr<MidiConsoleView> const& messageView)
{
    auto* const newWindow = addSubWindow(messageView.get());

    m_logViews.emplace(messageView, newWindow);
    newWindow->setWindowTitle(messageView->portName());
    newWindow->showMaximized();
    newWindow->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinMaxButtonsHint);
}

void MidiConsoleArea::remove(std::shared_ptr<MidiConsoleView> const& messageView)
{
    auto it = m_logViews.find(messageView);

    if (it != m_logViews.end())
    {
        removeSubWindow(it->second);
        m_logViews.erase(it);
    }
}
