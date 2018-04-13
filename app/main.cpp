//
// Created by Io on 04/04/2018.
//

#include <QApplication>

#include "MainWindow.hpp"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    MainWindow window;

    window.show();
    return app.exec();
}