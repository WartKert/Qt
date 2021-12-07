#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QStatusBar>
#include <QImage>
#include <QTextOption>
#include <QTextStream>
#include <QFontDialog>
#include <QColorDialog>
#include <QStyle>
#include <QFileInfo>
#include <QRegExp>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::slot_status_show(QString text, uint32_t time)
{
    ui->statusbar->showMessage(text, time);
    //emit signal_show_mess(text, time);

}
// Слот для отображение картинки шрифтов
void MainWindow::slot_show_font(QImage image) {

    QPixmap pixmap(QPixmap::fromImage(image));
    //ui->dispImageFont->sets;
    ui->dispImageFont->setScaledContents(false);


    ui->dispImageFont->setPixmap(pixmap);

            //ui->ImageToDisplay->setPixmap(pixmap.scaled(w, h, Qt::KeepAspectRatio));


}

//
void MainWindow::slot_ready_to_save(QString string)
{
    QFile fileName(QFileDialog::getSaveFileName(this, tr("Сохранить как"), "/Array", tr("Text (*.cpp);;Text (*.c)")));
    QFileInfo fileNameInfo(fileName);

    // Создание и запись в заголовочный файл
    QFile fileHeader(fileNameInfo.path() + "/" + fileNameInfo.baseName() + ".h");
    fileHeader.open(QIODevice::WriteOnly);
    QTextStream out2(&fileHeader);
    out2 << this->create_header(string, fileNameInfo.baseName());
    fileHeader.close();

    // Запись в основной файл
    QTextStream out(&fileName);
    fileName.open(QIODevice::WriteOnly | QIODevice::Text);
    out << this->create_main(string, fileNameInfo.baseName());
    fileName.close();
}

// Функция заполнения основного файла
QString MainWindow::create_main(QString text, QString filename)
{

    return text.insert(text.indexOf("\t/* ", 0), "#include \"" + filename + ".h\n\nnnamespace " + \
                       filename.toLower().replace(0, 1, filename[0].toUpper()) + " {\n");
}

// Функция заполнения заголовочного файла
QString MainWindow::create_header(QString text, QString filename)
{
    int pos {0};
    filename = filename.toLower();
    filename.replace(0, 1, filename[0].toUpper());
    // Текст для заголовочного файла
    QRegExp search("\\b(?:image).+\\b");
    search.setMinimal(true);
    QString text_header = "/********* Заголовочный файл для массива изображения в формате uint16 *********/\n"
                "/********* На конце файла находится структура с указателями на массивы ********/\n" + \
                QLatin1String("#ifndef ") + filename.toUpper() + "_H\n#define " + filename.toUpper() + \
                "_H\n\n#include <stdint.h>\n#include " + ((search.indexIn(text, 0) > -1)? "\"Iimage\"" : "\"Ifont\"") + \
                "\n\nnamespace " + filename + " {\n";


    QRegExp rx("(?:\\/[\\*]\\s.*)(constexpr)(?:.*\\s+(\\]|\\}))");
    rx.setMinimal(true);

    while ((pos = rx.indexIn(text, pos)) > -1) {
        pos += rx.matchedLength();
        text_header.append("\t" + rx.cap(0).replace(rx.cap(1), "\textern const") + ";\n\n");
    }

    text_header.append("}\nclass point" + filename + " : public " + ((search.indexIn(text, 0) > -1)? "Iimage" : "Ifont") + " {\npublic:\n");

    QRegExp ry("\\b(uint\\d{2}_t)(?:\\s)(char_.+|size|length|image\\w+)((?=\\s\\[)|(?=\\s\\{))\\b");           //("\\b(?:((?:char_(?![\\s+\\[])|image).+|(size|length)))\\b");
    ry.setMinimal(true);
    QRegExp rz("(?:plength\\(|psize\\()(?![&])(.+\\))");
    rz.setMinimal(true);
    pos = 0;
    QString temp("point" + filename + "() :\n\t\t");
    int cnt = 0;
    while ((pos = ry.indexIn(text, pos)) > -1) {
        pos += ry.matchedLength();
        text_header.append("\t/* Указатель на ' " + ry.cap(2) + " ' */\n\tconst " + ry.cap(1) + " *p" + ry.cap(2) + ";\n\n");
        if(cnt == 3) {
            cnt = 0;
            temp.append("\n\t\t");
        }
        ++cnt;
        temp.append("p" + ry.cap(2) + "(" + filename + "::" + (ry.cap(2)) + "),");
        if (rz.indexIn(temp, 0) > -1) temp.replace(rz.cap(1), "&" + rz.cap(1));
    }

    temp.resize(temp.size()-1);
    text_header.append("\n\tvirtual ~point" + filename + "() {};\n");

    if (search.indexIn(text, 0) > -1) {
        return text_header.append("\t" + temp + \
                "\n\t\t{}\n\n\t// Метод возвращает длину массива для одного символа символа\n\t" + \
                "virtual uint16_t GetLength() {\n\t\treturn *(this -> length);\n\t}\n" + \
                "};\n#endif /* " + filename.toUpper() + "_H */");
    }
    else {
        return text_header.append("\t" + temp + \
                "\n\t\t{}\n\n\t// Метод возвращает размер символа\n\tvirtual uint16_t GetSize() {\n\t\treturn *(this -> size);\n\t}\n\n\t" + \
                "// Метод возвращает длину массива для одного символа символа\n\tvirtual uint16_t GetLength() {\n\t\treturn *(this -> length);\n\t}\n\n\t" + \
                "// Метод поиска указанного символа (UTF-16) и возврат его указателя\n\tvirtual uint16_t* GetFont(const char16_t &sym) {\n\t\t" + \
                "return const_cast<uint16_t*>((sym == 0x401)? pchar_401 : (sym == 0x451)? pchar_451 : \\\n\t\t\t\t(sym < 127)? " + \
                "this ->pchar_20 + static_cast<uint32_t>((sym - 32) * *(this -> length)) : \\\n\t\t\t\tthis ->pchar_410 +" + \
                "static_cast<uint32_t>((sym - 1040) * *(this -> length)));\n\t}" + "\n};\n#endif /* " + filename.toUpper() + "_H */");
    }
}

//
void MainWindow::on_openMenu_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть файл"), "", "Images (*.bmp *.jpg)");
    QFile file(fileName);

    if (file.open(QIODevice::ReadOnly)) {

        Image = QImage(fileName);
        ui->statusbar->showMessage("Файл успешно прочитан", 2000);

        // Вывод открытой картинки на центральный виджет
        QPixmap pixmap(QPixmap::fromImage(Image));
        quint16 w = ui->ImageToDisplay->width();
        quint16 h = ui->ImageToDisplay->height();
        ui->ImageToDisplay->setPixmap(pixmap.scaled(w, h, Qt::KeepAspectRatio));
        // Вывод информации об открытой картинки
        QString textLabel;
        textLabel.append("Фаил = " + fileName + "\n"\
                         + "Высота = " + QString::number(Image.height(), 10) + "\n"\
                         + "Ширина = " + QString::number(Image.width(), 10) + "\n"\
                         + "Глубина цвета = " + QString::number(Image.depth(), 10) + "\n");
        ui->textToDisplay->setText(textLabel);
        file.close();
    }
    else {
        ui->statusbar->showMessage("Ошибка открытия файла", 2000);
    }
}

void MainWindow::on_exitMenu_triggered()
{
    QApplication::exit();
}

void MainWindow::on_convertnMenu_triggered()
{
    // Создание окна для установки настроек конвертирования
    wo = new DialogOptions(this);
    wo->setModal(true);
    wo->exec();
    // Вызов функции конвертирования изображения
    //std::initializer_list<quint16> list_arg{wo->setHeight, wo->setWidth, wo->setGUIWidtn, wo->setRGB, 0};
    MainWindow::signal_start_convert(this->Image, wo->setHeight, wo->setWidth, wo->setGUIWidtn, wo->setRGB, false);
    delete wo;
}


void MainWindow::on_pushButtonOpen_clicked()
{
    this->on_openMenu_triggered();
}

void MainWindow::on_pushButtonConvert_clicked()
{
    this->on_convertnMenu_triggered();
}

// Реакция на нажатие кнопки Шрифт
void MainWindow::on_pushButtonOpenFont_clicked()
{
    bool ok;
    cur_font = QFontDialog::getFont(&ok, QFont("Helvetica [Cronyx]", 10), this);
    if (ok)
    {
        ui->plainTextEdit->setFont(cur_font);
    }
}

// Реакция на нажатие кнопки Цвет фона
void MainWindow::on_pushButtonOpenColor_clicked()
{
    QColor temp = QColorDialog::getColor(Qt::white, this, "Цвет фона", QColorDialog::DontUseNativeDialog );
    if(temp.isValid())
    {
        cur_color_background = temp;
        ui->plainTextEdit->setStyleSheet(QString("background-color:%1;\n color:%2;").arg(cur_color_background.name()).arg(cur_color_font.name()));
    }
}

// Реакция на нажатие кнопки Цвет шрифта
void MainWindow::on_pushButtonOpenColorFont_clicked()
{
    QColor temp = QColorDialog::getColor(Qt::white, this, "Цвет шрифта", QColorDialog::DontUseNativeDialog );
    if(temp.isValid())
    {
        cur_color_font = temp;
        ui->plainTextEdit->setStyleSheet(QString("background-color:%1;\n color:%2;").arg(cur_color_background.name()).arg(cur_color_font.name()));
    }
}
// Реакция на нажатие кнопки Конвертирования шрифта
void MainWindow::on_pushButtonConvertText_clicked()
{
    wo = new DialogOptions(this);
    wo->setModal(true);
    wo->exec();

    QString text;

    // Если необходимо добавить кирилические символы
    if(ui->checkBox->isChecked())
    {
        for (int un = 0x0410; un <= 0x044F; ++un) {
            text.append(QChar(un));
        }
    }
    // Если необходимо добавить латинские символы
    if(ui->checkBox_2->isChecked())
    {
        for (int un = 0x0020; un <= 0x007E; ++un) {
            text.append(QChar(un));
        }
    }
    // Если необходимо добавить дополнительные символы
    if(ui->checkBox_3->isChecked())
    {
        text.append(ui->plainTextEdit->toPlainText());
    }

    // Вызов функции конвертирования текста
    emit signal_start_convert_font(cur_color_background, cur_color_font, cur_font, \
                                   text, wo->setGUIWidtn, wo->setRGB);
    delete wo;
}

// Блокировка тестового поля при снятом чекбоксе 3
void MainWindow::on_checkBox_3_stateChanged(int arg1)
{
    if(arg1) ui->plainTextEdit->setReadOnly(false);
    else ui->plainTextEdit->setReadOnly(true);
}
