#ifndef MIDILOGWIDGET_HPP
#define MIDILOGWIDGET_HPP
#include <QWidget>
#include <QMdiArea>

class MidiConsoleView;

class MidiConsoleArea : public QMdiArea
{
public:
    explicit MidiConsoleArea(QWidget* parent);

    void add(std::shared_ptr<MidiConsoleView> const& messageView);
    void remove(std::shared_ptr<MidiConsoleView> const& messageView);
private:
    std::vector<std::shared_ptr<MidiConsoleView>> m_logViews;
};

#endif // MIDILOGWIDGET_HPP
