#ifndef DIALOGOPTIONS_H
#define DIALOGOPTIONS_H

#include <QDialog>
#include <QMainWindow>
#include <QAbstractButton>

namespace Ui {
class DialogOptions;
}

class DialogOptions : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOptions(QWidget *parent = nullptr);
    ~DialogOptions();

    quint16 setHeight;
    quint16 setWidth;
    quint16 setGUIWidtn;
    quint16 setRGB;

private slots:

    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::DialogOptions *ui;
};

#endif // DIALOGOPTIONS_H
