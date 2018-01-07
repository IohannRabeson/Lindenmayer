//
// Created by Io on 30/12/2017.
//
// TODO:
// - DONE: port selector
// - DONE: schemes integration
// - DONE: Scheme selector
// - DONE: ability to listen multiple ports at the same time
// - icon tray!
// - use dockwidget and toolbar manager to build a layout system
// - virtual keyboard (QML???)
// - input message filters
//   - message filters list view
//   - filter function
//   - model to store filters states
// - A data transit LED
// - Replace scheme by device translator:
// A device translator can format informations:
//      - message type
//      - channel number
//      - control change
//      - program change
//      - sysex
// - Save/load settings of inputs / outputs ports enabled value
// - Commands for context menu in Midi Inputs dock widget and Midi Output dock widget:
//      - enable only (double click)
//      - enable all / disable all (depending of the selection state of the item under the cursor)
// - data details with QML:
// Display raw byte as hexadecimal with text highlighting and tooltips for very explicative
// and detailed informations.
// - Message recorder / player to be able to record messages then replays them.

#include <QApplication>

#include "Ui/MainWindow.hpp"

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

    setupApplication();

    MainWindow widget;

    widget.show();
    return app.exec();
}
