//
// Created by Io on 04/04/2018.
//

#include <QApplication>
#include <QCommandLineParser>
#include <QStandardPaths>

#include "MainWindow.hpp"

#include <QtDebug>

QCommandLineOption const optionAutoTest("auto-test", "run in automatic test mode", "auto test program directory path");

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("Lindenmayer");
    QApplication::setApplicationVersion("0.0.0");
    QApplication::setOrganizationName("Dead Devs Society");
    QCommandLineParser commandLineParser;

    commandLineParser.addOption(optionAutoTest);
    commandLineParser.process(app);

    MainWindow window;

    window.show();

    if (commandLineParser.isSet(optionAutoTest))
    {
        qDebug() << "Directory:" << commandLineParser.value(optionAutoTest);
        QDir dir(commandLineParser.value(optionAutoTest));
        auto const entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Files | QDir::Readable);

        qDebug() << entries;
        for (auto const& fileInfo : entries)
        {
            qDebug() << " - Load" << fileInfo.absoluteFilePath();
            if (!window.loadProgram(fileInfo.absoluteFilePath()))
            {
                return 1;
            }
            if (!window.build())
            {
                return 2;
            }
            window.draw();
            window.update();
            window.zoomToFit();
            window.show();
            window.exportImage(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/Lindenmayer-Auto-Tests/" + fileInfo.completeBaseName() + ".png");
        }
        return 0;
    }

    return app.exec();
}