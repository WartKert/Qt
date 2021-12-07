#ifndef SCALEDPIXMAP_H
#define SCALEDPIXMAP_H

#include <QMainWindow>
#include <QObject>
#include <QtWidgets/QLabel>

class ScaledPixmap : public QLabel
{
    Q_OBJECT
public:
    explicit ScaledPixmap(QWidget* parent = Q_NULLPTR);

protected:
    void scaledImageForWindow(QImage*);

public slots:
};

#endif // SCALEDPIXMAP_H
