#ifndef FILL_H
#define FILL_H
#include "shape.h"


class Fill:public Shape
{
public:
    Fill();
    Fill(QVector<QPoint*> points, ShapeType type, QRgb rgb, QPen pen);
    ~Fill();
    void refreshData();
//    add points of area to fill
    void addPoint(QImage* image, int x, int y);

};

#endif // FILL_H
