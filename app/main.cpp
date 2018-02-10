//
// Created by Io on 30/12/2017.
//
// TODO:
// - Replace scheme by device translator:
// A device translator can format informations:
//      - message type
//      - channel number
//      - control change
//      - program change
//      - sysex
// - data details with QML:
// Display raw byte as hexadecimal with text highlighting and tooltips for very explicative
// and detailed informations.

#include <QApplication>
#include <QCommandLineParser>
#include <QtDebug>

#include "MainWindow.hpp"
#include "Translators/TranslatorManager.hpp"
#include <Qool/LogManager.hpp>

#include <DarkStyle.h>

QCommandLineOption const DarkThemeOption("dark", QCoreApplication::translate("main", "Enable dark theme (experimental)"));
QCommandLineOption const TargetDirectoryOption(QStringList() << "language" << "selected-language",
                                         QCoreApplication::translate("main", "Select language"),
                                         QCoreApplication::translate("main", "language name"));

static void setupApplication()
{
    QApplication::setApplicationDisplayName(APPLICATION_DISPLAY_NAME);
    QApplication::setApplicationName(APPLICATION_NAME);
    QApplication::setApplicationVersion(APPLICATION_VERSION);
    QApplication::setOrganizationName(APPLICATION_ORGANIZATION);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    qool::LogManager::setupLoggers();
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    setupApplication();

    QCommandLineParser commandLineParser;

    commandLineParser.addOption(DarkThemeOption);
    commandLineParser.addOption(TargetDirectoryOption);
    commandLineParser.process(app);

    TranslatorManager translatorManager;

    if (commandLineParser.isSet(DarkThemeOption))
    {
        qInfo() << "[CommandLineParser]: Dark theme enabled";
        QApplication::setStyle(new DarkStyle);
    }

    if (!commandLineParser.isSet(TargetDirectoryOption)
        || !translatorManager.loadTranslator(QLocale(commandLineParser.value(TargetDirectoryOption))))
    {
        translatorManager.loadSystemTranslator();
    }

    MainWindow widget;

    widget.show();
    return app.exec();
}
