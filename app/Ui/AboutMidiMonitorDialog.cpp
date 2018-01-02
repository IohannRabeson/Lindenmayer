#include "AboutMidiMonitorDialog.hpp"
#include "ui_AboutMidiMonitorDialog.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>

namespace
{
    static QString loadTextFile(QString const& path)
    {
        QFile file(path);
        QString result;

        if (file.open(QFile::ReadOnly))
        {
            QTextStream stream(&file);

            result = stream.readAll();
        }
        return result;
    }
}

AboutMidiMonitorDialog::AboutMidiMonitorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutMidiMonitorDialog)
{
    ui->setupUi(this);
    ui->applicationNameLabel->setText(QApplication::applicationDisplayName());
    ui->applicationVersionLabel->setText(QApplication::applicationVersion());
    ui->creditPlainTextEdit->setPlainText(loadTextFile(":/Texts/Resources/Credits.txt"));
    ui->aboutTextEdit->setHtml(loadTextFile(":/Texts/Resources/About.txt"));
}

AboutMidiMonitorDialog::~AboutMidiMonitorDialog()
{
    delete ui;
}
