#include "dialogoptions.h"
#include "ui_dialogoptions.h"
#include <QPushButton>
#include <QWindow>
#include <QDialogButtonBox>
//#include <QAbstractButton>

DialogOptions::DialogOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOptions)
{
    ui->setupUi(this);
}

DialogOptions::~DialogOptions()
{
    delete ui;
}

void DialogOptions::on_buttonBox_clicked(QAbstractButton *button)
{
    if(button == ui->buttonBox->button(QDialogButtonBox::Ok))//            ->button(QDialogButtonBox::Ok)-ac)
    {
        setHeight = ui->spinBoxHeight->value();
        setWidth = ui->spinBoxWidth->value();
        setGUIWidtn = (ui->radioButton_16bits->isChecked())? 1 : (ui->radioButton_18bits->isChecked())? 2 :\
                        (ui->radioButton_SI->isChecked())? 3 : 0;
        setRGB = (ui->radioButton_RGB656->isChecked())? 1 : (ui->radioButton_RGB666->isChecked())? 2 : 0;
        this->accept();

    }
    if (button == ui->buttonBox->button(QDialogButtonBox::Cancel))
    {
        this->accept();
    }

}
