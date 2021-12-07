#ifndef CONVERTRGB_H
#define CONVERTRGB_H

#include <QObject>
#include "mainwindow.h"
#include <QFile>
#include <initializer_list>
#include <QPlainTextEdit>

class ConvertRGB : public QObject
{
    Q_OBJECT

public:
    ConvertRGB();
    virtual ~ConvertRGB();

    QImage Resize(QImage image, quint16 height, quint16 width);

    QVector<QVector<quint16>> ConvertBMPTo18Bit (QImage data);

    QString export_array(QVector<QVector<quint16>> arr);

    QString export_array_font(QVector<QVector<quint16>> arr, quint16 rows, QString text);

    void ConvertFont (QFont, QColor, QColor, QString);

public slots:
    void slot_start_convert(QImage, const quint16&, const quint16&, const quint16&, const quint16&, bool);

    void slot_start_convert_font(QColor, QColor, QFont, QString, quint16, quint16);


signals:
    void signal_start_convert(QImage);

    void signal_show_mess(QString, uint32_t);

    void signal_ready_to_save(QString);

    void signal_show_font(QImage);

private:

    QVector<QVector<quint16>> data_array;

};
#endif // CONVERTRGB_H

