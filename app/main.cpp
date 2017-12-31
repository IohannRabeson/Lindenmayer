//
// Created by Io on 30/12/2017.
//
// TODO:
// - port selector
// - bytes viewer
// - schemes integration
//

#include <QApplication>
#include <QTableView>

#include "MidiInputWidget.hpp"

static void setupApplication()
{
    QApplication::setApplicationDisplayName("Midi Monitor");
    QApplication::setApplicationName("MidiMonitor");
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    MidiInputWidget widget;

    setupApplication();
    widget.show();
    return app.exec();
}
