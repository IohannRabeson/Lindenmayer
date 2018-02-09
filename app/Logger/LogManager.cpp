//
// Created by Io on 04/02/2018.
//

#include "Logger/LogManager.hpp"

#include <QMessageLogger>
#include <QLatin1String>
#include <QMap>

#if defined(APPLICATION_RETAIL)
#include <QMessageBox>
#include <QApplication>
#endif

namespace
{
    static char const* getPrefix(QtMsgType const type)
    {
        static QMap<QtMsgType, char const*> const Prefixes
        {
            {QtDebugMsg, "[Debug]"},
            {QtInfoMsg, "[Info]"},
            {QtWarningMsg, "[Warning]"},
            {QtCriticalMsg, "[Critical]"},
            {QtFatalMsg, "[Fatal]"}
        };

        return Prefixes.value(type, "");
    }
}

class LogManagerPrivate
{
public:
    static void messageHandler(QtMsgType type, QMessageLogContext const& context, QString const& msg)
    {
        auto const localMsg = msg.toLocal8Bit();
        auto const messageTypeText = getPrefix(type);

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

#if defined(APPLICATION_RETAIL)
        if (type == QtFatalMsg)
        {
            std::fprintf(stderr, "[Fatal]: %s (%s:%u, %s)\n", localMsg.toStdString().c_str(), context.file, context.line, context.function);
            QMessageBox::warning(QApplication::activeWindow(), QApplication::applicationDisplayName(), QObject::tr("Fatal error: %0\nThe application will be closed.").arg(msg));
            abort();
        }
#else
    Q_UNUSED (context);
#endif
    }
};

void LogManager::setupLoggers()
{
    qInstallMessageHandler(&LogManagerPrivate::messageHandler);
}