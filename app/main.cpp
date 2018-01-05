//
// Created by Io on 30/12/2017.
//
// TODO:
// - DONE: port selector
// - DONE: schemes integration
// - DONE: Scheme selector
// - DONE: ability to listen multiple ports at the same time
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

// - Commands for context menu in Midi Inputs dock widget:
//      - enable only (double click)
//      - enable all / disable all (depending of the selection state of the item under the cursor)
// - data details with QML:
// Display raw byte as hexadecimal with text highlighting and tooltips for very explicative
// and detailed informations.
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
