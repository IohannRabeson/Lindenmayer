//
// Created by Io on 04/04/2018.
//

#include <QApplication>
#include <QCommandLineParser>
#include <QStandardPaths>

#include "MainWindow.hpp"

#include <QtDebug>
#include <Optional.hpp>

#include <memory>

QCommandLineOption const optionAutoTest("auto-test", "run in automatic test mode", "auto test program directory path");

class AutoTest : public QObject
{
public:
    AutoTest(QFileInfoList const& files, MainWindow* parent)
    : QObject(parent)
    , m_window(parent)
    , m_files(files)
    , m_timerId(-1)
    , m_currentFile(0)
    {
        m_timerId = startTimer(100);
    }

    int resultCode() const
    {
        return m_resultCode.value_or(0);
    }
protected:
    void timerEvent(QTimerEvent* event) override
    {
        if (event->timerId() == m_timerId && m_currentFile < m_files.size())
        {
            auto const fileInfo = m_files[m_currentFile];

            QString result;

            if (!m_window->loadProgram(fileInfo.absoluteFilePath()))
            {
                if (!m_resultCode)
                {
                    m_resultCode = 1;
                }
                result = QObject::tr("Loading failed");
            }
            else if (!m_window->build())
            {
                if (!m_resultCode)
                {
                    m_resultCode = 2;
                }
                result = QObject::tr("Build failed");
            }
            else
            {
                result = QObject::tr("Success");
            }

            qDebug().nospace().noquote() << " - Load \"" << fileInfo.fileName() << "\" -> " << result;

            m_window->draw();
            m_window->update();
            m_window->zoomToFit();
            m_window->show();
            QApplication::processEvents();
            ++m_currentFile;
        }

        if (m_currentFile >= m_files.size())
        {
            qApp->quit();
        }
    }
private:
    MainWindow* const m_window;
    QFileInfoList m_files;
    lcode::Optional<int> m_resultCode;
    int m_timerId;
    int m_currentFile;
};

std::unique_ptr<AutoTest> performAutoTests(QCommandLineParser const& commandLineParser, MainWindow& window)
{
    QDir dir(commandLineParser.value(optionAutoTest));
    std::unique_ptr<AutoTest> autoTest;

    if (dir.exists())
    {
        qDebug() << "Running loading and building auto tests with directory" << dir.path();

        auto const entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Files | QDir::Readable);

        autoTest.reset(new AutoTest(entries, &window));

        qApp->exec();
    }
    else
    {
        qDebug() << "Unable to open directory:" << dir.absolutePath();
    }

    return autoTest;
}

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
        auto tests = performAutoTests(commandLineParser, window);

        return tests ? tests->resultCode() : 3;
    }

    return app.exec();
}