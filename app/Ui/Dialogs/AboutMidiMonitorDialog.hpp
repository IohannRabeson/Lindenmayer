#ifndef ABOUTMIDIMONITORDIALOG_HPP
#define ABOUTMIDIMONITORDIALOG_HPP

#include <QDialog>

namespace Ui {
    class AboutMidiMonitorDialog;
}

class AboutMidiMonitorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutMidiMonitorDialog(QWidget *parent = 0);
    ~AboutMidiMonitorDialog();

private:
    Ui::AboutMidiMonitorDialog *ui;
};

#endif // ABOUTMIDIMONITORDIALOG_HPP
