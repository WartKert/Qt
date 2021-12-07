#include "mainwindow.h"

#include <QApplication>
#include "convertrgb.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    ConvertRGB cRGB;

    QObject::connect(&cRGB, &ConvertRGB::signal_show_mess, &w, &MainWindow::slot_status_show);
    QObject::connect(&w, &MainWindow::signal_start_convert, &cRGB, &ConvertRGB::slot_start_convert);
    QObject::connect(&cRGB, &ConvertRGB::signal_ready_to_save, &w, &MainWindow::slot_ready_to_save);
    QObject::connect(&w, &MainWindow::signal_start_convert_font, &cRGB, &ConvertRGB::slot_start_convert_font);
    QObject::connect(&cRGB, &ConvertRGB::signal_show_font, &w, &MainWindow::slot_show_font);
    w.show();
    return a.exec();
}









