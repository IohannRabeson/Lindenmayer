//
// Created by Io on 30/12/2017.
//
// TODO:
// - DONE: port selector
// - DONE: schemes integration
// - DONE: TranslatorCreatorItem selector
// - DONE: ability to listen multiple ports at the same time
// - DONE: icon tray!
// - DONE virtual keyboard

// - use dockwidget and toolbar manager to build a layout system

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

#if defined(APPLICATION_RETAIL)
// Noop message handler, used in RETAIL mode
static void messageHandler(QtMsgType type, QMessageLogContext const& context, QString const& msg)
{
    if (type == QtFatalMsg)
    {
        QByteArray const localMsg = msg.toLocal8Bit();

        std::fprintf(stderr, "[Fatal]: %s (%s:%u, %s)\n", localMsg.toStdString().c_str(), context.file, context.line, context.function);
        abort();
    }
}
#else
//#elif defined(NDEBUG)
// Without details message handler, used in RELEASE mode
static void messageHandler(QtMsgType type, QMessageLogContext const&, QString const& msg)
{
    QByteArray const localMsg = msg.toLocal8Bit();

    switch (type)
    {
        case QtDebugMsg:
            std::fprintf(stderr, "[Debug]: %s\n", localMsg.constData());
            break;
        case QtInfoMsg:
            std::fprintf(stderr, "[Info]: %s\n", localMsg.constData());
            break;
        case QtWarningMsg:
            std::fprintf(stderr, "[Warning]: %s\n", localMsg.constData());
            break;
        case QtCriticalMsg:
            std::fprintf(stderr, "[Critical]: %s\n", localMsg.constData());
            break;
        case QtFatalMsg:
            std::fprintf(stderr, "[Fatal]: %s\n", localMsg.constData());
            std::abort();
    }
}
//#else
//// Detailed message handler, used in DEBUG mode
//static void messageHandler(QtMsgType type, QMessageLogContext const& context, QString const& msg)
//{
//    QByteArray const localMsg = msg.toLocal8Bit();
//
//    switch (type)
//    {
//        case QtDebugMsg:
//            std::fprintf(stderr, "[Debug]: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
//            break;
//        case QtInfoMsg:
//            std::fprintf(stderr, "[Info]: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
//            break;
//        case QtWarningMsg:
//            std::fprintf(stderr, "[Warning]: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
//            break;
//        case QtCriticalMsg:
//            std::fprintf(stderr, "[Critical]: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
//            break;
//        case QtFatalMsg:
//            std::fprintf(stderr, "[Fatal]: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
//            std::abort();
//    }
//}
#endif

static void setupApplication()
{
    QApplication::setApplicationDisplayName(APPLICATION_DISPLAY_NAME);
    QApplication::setApplicationName(APPLICATION_NAME);
    QApplication::setApplicationVersion(APPLICATION_VERSION);
    QApplication::setOrganizationName(APPLICATION_ORGANIZATION);

    qInstallMessageHandler(&messageHandler);
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    setupApplication();

    MainWindow widget;

    widget.show();
    return app.exec();
}
