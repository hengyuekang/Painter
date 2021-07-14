#ifndef FILL_H
#define FILL_H
#include "shape.h"

class Fill : public Shape
{
public:
    Fill();
    Fill(QVector<QPoint *> points, ShapeType type, QRgb rgb, QPen pen);
    ~Fill();
    void refreshData();
    //    add points of area to fill
    void addPoint(QImage *image, int x, int y);
    void paintShape(QPainter &p, QImage *image, bool isSave);
    void changeColor(QPainter &p, QImage *image, bool isSave);
private:
    QVector<QVector<QPoint *>> lines;
};

#endif // FILL_H
