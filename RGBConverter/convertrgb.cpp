#include "convertrgb.h"
#include <QTextCodec>
#include <QTextEdit>
#include <QString>
#include <iterator>
#include <QPixmap>
#include <QPainter>
#include <QChar>

ConvertRGB::ConvertRGB()
{

}

ConvertRGB::~ConvertRGB()
{

}

void ConvertRGB::slot_start_convert_font(QColor c_back, QColor c_font, QFont font, QString text, \
                                         quint16 gui, quint16 rgb)
{
    QSize size;
    if(font.pointSize() != -1)
    {
        size.setHeight(font.pointSize() + 4);
        size.setWidth(font.pointSize() + 4);
    }
    else
    {
        size.setHeight(font.pixelSize() + 4);
        size.setWidth(font.pixelSize() + 4);
    }

    QImage image(size, QImage::Format_RGB888);
    QPainter painter(&image);


    painter.setFont(font);
    painter.setBackgroundMode(Qt::OpaqueMode);
    painter.setBackground(c_back);
    painter.setPen(c_font);

    data_array.clear();
    for(auto symbol : text)
    {
        painter.fillRect(image.rect(), c_back);
        painter.drawText(image.rect(), Qt::AlignHCenter | Qt::AlignBottom, symbol);
        emit signal_show_font(image);
        this->slot_start_convert(image, image.height(), image.width(), gui, rgb, true);
    }
    emit signal_ready_to_save(this->export_array_font(this->data_array, size.width(), text));

}

// Функция масштабирования изображения
QImage ConvertRGB::Resize(QImage image, quint16 height, quint16 width)
{
     return image.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

// Функция начала конвертирования
void ConvertRGB::slot_start_convert(QImage image, const quint16 &height, const  quint16 &width, \
                                    const  quint16 &gui, const  quint16 &rgb, bool type)
{
    //std::initializer_list<quint16>::iterator arg = list_arg.begin();
    // Определения необходимости изменения размера изображения
    if(height != image.height() || width != image.width())
    {
        image = this->Resize(image, height, width);
    }
    // Выбор функции конвертирования на основании заданных параметров конвертирования
    // Если RGB656
    if(rgb == 1)
    {

    }
    // Если RGB666
    else if (rgb == 2)
    {
        switch (gui)
        {
            // Интерфейс 16 бит
            case 1:
                if(!type)
                {
                    //data_array.clear();
                    //this->ConvertBMPTo18Bit(image);
                    emit signal_ready_to_save(this->export_array(this->ConvertBMPTo18Bit(image))); //this->ConvertBMPTo18Bit(image);
                }
                else this->data_array += this->ConvertBMPTo18Bit(image);
                break;

            // Интерфейс 18 бит
            case 2:
            break;
            // Послед. интерфейс
            case 3:
            break;
        }

    }
}

// Функция преобразование изображения в 18 бит цвета
QVector<QVector<quint16>> ConvertRGB::ConvertBMPTo18Bit (QImage data){
    QVector<QVector<quint16>> data_array;

    if (data.depth() < 24){
        emit signal_show_mess("Глубина цвета менее 24 бит", 3000);
    }
    else
    {
        data_array.resize(data.width());
        for(int w = 0; w < data.width(); ++w){
            quint8 var = 0;
            for(int h = 0; h < data.height(); ++h){
                QRgb pix = data.pixel(w,h);
                QRgb blue = ((63 * (pix & 0xFF)) / 255) << 2;
                QRgb green = ((63 * (pix >> 8 & 0xFF)) / 255) << 2;
                QRgb red = ((63 * (pix >> 16 & 0xFF)) / 255) << 2;

                switch (var) {
                case 0:
                    data_array[w].append( green | (red << 8));
                    data_array[w].append(blue << 8);
                    break;
                case 1:
                    data_array[w].replace(data_array[w].size() - 1, data_array[w].last() | red);
                    data_array[w].append(blue | (green << 8));
                    break;
                }
                (!var)? ++var : var = 0;
            }
        }
        emit signal_show_mess("Преобразование завершено", 3000);
        //emit signal_ready_to_save(this->export_array(data_array));

    }
    return data_array;
}

// Функция подготовки массива для вывода в файл
QString ConvertRGB::export_array(QVector<QVector<quint16>> arr)
{

    QTextCodec::setCodecForLocale( QTextCodec::codecForName( "UTF-8" ) );
    QString text_arr;

    // Текст для основного файла
    text_arr = "/********* Файл содержит массив изображения в формате uint16 *********/\n\n\t/* Массив изображения */\n" + \
                QLatin1String("\tconstexpr uint16_t imageArray [ ") + QString::number(arr.size() * arr[0].size(), 10) + \
                QLatin1String(" ] = {\n\t\t\t");
    QVector<QVector<quint16>>::iterator row;
    QVector<quint16>::iterator col;

    for(row = arr.begin(); row != arr.end(); ++row)
    {
        quint8 subcol = 0;
        for(col = row->begin(); col != row->end(); ++col)
        {
            text_arr.append(" 0x" + QString::number(*col, 16) + QLatin1String(","));
            ++subcol;
            if(subcol == 8)
            {
                text_arr.append("\n\t\t\t");
                subcol = 0;
            }
        }
        text_arr.resize(text_arr.size()-1);
        text_arr.append("/*** end of " + QString::number(std::distance(arr.begin(), row), 10) + QLatin1String(" row ***/\n\t\t\t"));

    }
    text_arr.append(" };\n\n\t/* Длина масссива */\n\tconstexpr uint32_t length { " + QString::number(arr.size() * arr[0].size(), 10) + " };\n}");
    return text_arr;
}

// Функция подготовки массива для вывода в файл
QString ConvertRGB::export_array_font(QVector<QVector<quint16>> arr, quint16 rows, QString text)
{
    QTextCodec::setCodecForLocale( QTextCodec::codecForName( "UTF-8" ) );
    QString arrayText;

    arrayText = ("/*************** Файл содержит массив шрифтов ****************/\n" + \
            QString("/******** Каждый символ закодирован отдельным массивом *******/\n")) + \
            QString("/********* Формат: char_[код символа в UTF-16] ***************/\n"
                    "/********* Размер шрифта во высоте и ширине " + QString::number(rows, 10) + " пикселей *******/\n\n");

    QVector<QVector<quint16>>::iterator row = arr.begin();
    QVector<quint16>::iterator col;

    QChar* symbol = text.data();

    QVector<QVector<quint16>>::iterator endSubRow = arr.begin();
    QVector<QVector<quint16>>::iterator end = arr.end();

    while(endSubRow != end)
    {
        endSubRow = row + rows;

        arrayText.append("\t/* Массив символа ' " + QString(*symbol) + QString(" ' */\n"));
        arrayText.append("\tconstexpr uint16_t char_" + QString::number((*symbol).unicode(), 16) + QString(" [ ") \
                         + QString::number(rows * arr[0].size(), 10) + QString(" ] = {\n\t\t\t"));
        for( ; row != endSubRow; ++row)
        {
            quint8 subcol = 0;
            for(col = row->begin(); col != row->end(); ++col)
            {
                arrayText.append(" 0x" + QString::number(*col, 16) + QLatin1String(","));
                ++subcol;
                if(subcol == 8)
                {
                    arrayText.append("\n\t\t\t");
                    subcol = 0;
                }
            }
            arrayText.append("\n\t/*** end of " + QString::number(rows - std::distance(row, endSubRow), 10) + QLatin1String(" row ***/\n\t\t\t"));
        }
        arrayText.append(" };\n\n");
        ++symbol;
    }
    arrayText.append("\t/* Размер символа */\n\tconstexpr uint16_t size { " + QString::number(rows, 10) + \
                     " };\n\n\t/* Длина масссива */\n\tconstexpr uint16_t length { " + \
                    QString::number(rows * arr[0].size(), 10) + " };\n}");
    //arrayText.resize(arrayText.size()-2);
    //arrayText.append("};");
    return arrayText;
}
