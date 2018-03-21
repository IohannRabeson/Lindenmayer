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
}

void MidiConsoleArea::add(std::shared_ptr<MidiConsoleView> const& messageView)
{
    auto* const newWindow = addSubWindow(messageView.get());

    m_logViews.push_back(messageView);
    newWindow->setWindowTitle(messageView->portName());
    newWindow->showMaximized();
    newWindow->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinMaxButtonsHint);
}

void MidiConsoleArea::remove(std::shared_ptr<MidiConsoleView> const& messageView)
{
    removeSubWindow(messageView.get());
    m_logViews.erase(std::remove(m_logViews.begin(), m_logViews.end(), messageView), m_logViews.end());
}
