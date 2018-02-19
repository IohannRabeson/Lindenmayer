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
#include <Qool/StyleFactory.hpp>

#include <DarkStyle.h>

QCommandLineOption const ThemeOption("theme", QCoreApplication::translate("main", "Select theme"), "theme key");
QCommandLineOption const TargetDirectoryOption(QStringList() << "language" << "selected-language"
                                              , QCoreApplication::translate("main", "Select language")
                                              , QCoreApplication::translate("main", "language name"));

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

static void setupStyleFactory(qool::StyleFactory& styleFactory)
{
    styleFactory.addCreator("dark", []() -> QStyle* { return new DarkStyle; });
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    qool::StyleFactory styleFactory;
    TranslatorManager translatorManager;

    setupApplication();
    setupStyleFactory(styleFactory);

    QCommandLineParser commandLineParser;

    qDebug() << "[CommandLineParser]: arguments:" << QApplication::arguments();

    commandLineParser.addOption(ThemeOption);
    commandLineParser.addOption(TargetDirectoryOption);

    if (!commandLineParser.parse(app.arguments()))
    {
        qDebug() << commandLineParser.errorText();
        return 1;
    }

    if (commandLineParser.isSet(ThemeOption))
    {
        QString const themeKey = commandLineParser.value(ThemeOption);

        qInfo() << "[CommandLineParser]: theme selected:" << themeKey;

        QStyle* const style = styleFactory.create(themeKey);

        if (style)
        {
            QApplication::setStyle(new DarkStyle);
        }
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
