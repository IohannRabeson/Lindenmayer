//
// Created by Io on 30/12/2017.
//
// TODO:
// - DONE: port selector
// - DONE: schemes integration
// - ability to listen multiple ports at the same time
// - bytes viewer
// - bytes line viewer
// - message details viewer
// - message filters
//   - message filters list view
//   - filter function
//   - model to store filters states
// - Scheme selector
//

#include <QApplication>

#include "MainWindow.hpp"

static void setupApplication()
{
    QApplication::setApplicationDisplayName("Midi Monitor");
    QApplication::setApplicationName("MidiMonitor");
    QApplication::setApplicationVersion("dev");
    QApplication::setOrganizationName("Dead devs society");
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    MainWindow widget;

    setupApplication();
    widget.show();
    return app.exec();
}
