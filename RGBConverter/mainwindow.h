#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include "dialogoptions.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    //QByteArray Data;
    QByteArray Data;

    void StatusShow(QString text, uint32_t time);

    void saveFile ();

    void text_editor();

    QString create_header(QString text, QString filename);

    QString create_main(QString text, QString filename);

public slots:
    void slot_status_show(QString, uint32_t);

    void slot_ready_to_save(QString);

    void slot_show_font(QImage);

signals:
    void signal_show_mess(QString, uint32_t);

    void signal_start_convert(QImage, quint16, quint16, quint16, quint16, bool);

    void signal_start_convert_font(QColor, QColor, QFont, QString, quint16, quint16);

private slots:


    void on_openMenu_triggered();

    void on_exitMenu_triggered();

    void on_convertnMenu_triggered();

    void on_pushButtonOpen_clicked();

    void on_pushButtonConvert_clicked();

    void on_pushButtonOpenFont_clicked();

    void on_pushButtonOpenColor_clicked();

    void on_pushButtonOpenColorFont_clicked();

    void on_pushButtonConvertText_clicked();

    void on_checkBox_3_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
    DialogOptions *wo;
    QImage Image;
    QColor cur_color_background = QColor::fromRgb(QRgb(0xffffff));
    QColor cur_color_font = QColor::fromRgb(QRgb(Qt::black));
    QFont cur_font;
};
#endif // MAINWINDOW_H
