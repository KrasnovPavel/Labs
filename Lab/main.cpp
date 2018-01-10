#include "mainwindow.h"
#include "settingswindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    SettingsWindow sw;

    QObject::connect(&sw, &SettingsWindow::startWithPos, &w, &MainWindow::startWithPos);
    QObject::connect(&sw, &SettingsWindow::start, &w, &MainWindow::start);
    QObject::connect(&sw, &SettingsWindow::pause, &w, &MainWindow::pause);
    QObject::connect(&w, &MainWindow::stepResult, &sw, &SettingsWindow::step);
    QObject::connect(&sw, &SettingsWindow::setPosition, &w, &MainWindow::setPosition);

    w.show();
    sw.show();

    return a.exec();
}
