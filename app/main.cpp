//
// Created by Io on 30/12/2017.
//

#include <QApplication>
#include <QTableView>
#include <QtDebug>

#include <QMidiIn.hpp>
#include <QMidiMessageModel.hpp>

static void setupApplication()
{
    QApplication::setApplicationDisplayName("Midi Monitor");
    QApplication::setApplicationName("MidiMonitor");
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QTableView widget;
    QMidiIn midiIn;
    QMidiMessageModel model;

    setupApplication();

    QObject::connect(&midiIn, &QMidiIn::messageReceived, &model, &QMidiMessageModel::append);
    QObject::connect(&midiIn, &QMidiIn::error, [](QString const& error)
    {
        qWarning() << error;
    });
    widget.setModel(&model);
    midiIn.openPort(0);
    widget.show();
    return app.exec();
}
