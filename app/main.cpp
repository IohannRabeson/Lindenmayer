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

#include <DarkStyle.h>

#if defined(APPLICATION_RETAIL)
#include <QMessageBox>
#endif

QCommandLineOption const DarkThemeOption("dark", QCoreApplication::translate("main", "Enable dark theme (experimental)"));
QCommandLineOption const TargetDirectoryOption(QStringList() << "language" << "selected-language",
                                         QCoreApplication::translate("main", "Select language"),
                                         QCoreApplication::translate("main", "language name"));

namespace Log
{
    static char const* formatMessageType(QtMsgType const type)
    {
        char const* typeText = nullptr;

        switch (type)
        {
            case QtDebugMsg:
                typeText = "[Debug]";
                break;
            case QtInfoMsg:
                typeText = "[Info]";
                break;
            case QtWarningMsg:
                typeText = "[Warning]";
                break;
            case QtCriticalMsg:
                typeText = "[Critical]";
                break;
            case QtFatalMsg:
                typeText = "[Fatal]";
                std::abort();
        }
        return typeText;
    }
}

static void messageHandler(QtMsgType type, QMessageLogContext const& context, QString const& msg)
{
    QByteArray const localMsg = msg.toLocal8Bit();
    auto const messageTypeText = Log::formatMessageType(type);

#if defined(APPLICATION_RETAIL)
    if (type == QtFatalMsg)
    {
        std::fprintf(stderr, "[Fatal]: %s (%s:%u, %s)\n", localMsg.toStdString().c_str(), context.file, context.line, context.function);
        QMessageBox::warning(QApplication::activeWindow(), QApplication::applicationDisplayName(), QObject::tr("Fatal error: %0\nThe application will be closed.").arg(msg));
        abort();
    }
    else if (type > QtDebugMsg)
    {
        std::fprintf(stdout, messageTypeText + ": %s (%s:%u, %s)\n", localMsg.toStdString().c_str(), context.file, context.line, context.function);
    }
#else
    switch (type)
    {
        case QtDebugMsg:
            std::fprintf(stdout, "%s: %s\n", messageTypeText, localMsg.constData());
            break;
        case QtInfoMsg:
            std::fprintf(stdout, "%s: %s\n", messageTypeText, localMsg.constData());
            break;
        case QtWarningMsg:
            std::fprintf(stderr, "%s: %s\n", messageTypeText, localMsg.constData());
            break;
        case QtCriticalMsg:
            std::fprintf(stderr, "%s: %s\n", messageTypeText, localMsg.constData());
            break;
        case QtFatalMsg:
            std::fprintf(stderr, "%s: %s\n", messageTypeText, localMsg.constData());
            std::abort();
    }
#endif
}

static void setupApplication()
{
    QApplication::setApplicationDisplayName(APPLICATION_DISPLAY_NAME);
    QApplication::setApplicationName(APPLICATION_NAME);
    QApplication::setApplicationVersion(APPLICATION_VERSION);
    QApplication::setOrganizationName(APPLICATION_ORGANIZATION);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    qInstallMessageHandler(&messageHandler);
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QCommandLineParser commandLineParser;

    commandLineParser.addOption(DarkThemeOption);
    commandLineParser.addOption(TargetDirectoryOption);
    commandLineParser.process(app);

    TranslatorManager translatorManager;

    setupApplication();

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
