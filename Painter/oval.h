#ifndef OVAL_H
#define OVAL_H
#include "shape.h"


class Oval: public Shape
{
public:
    Oval();
    Oval(QVector<QPoint*> points, ShapeType type, QRgb rgb, QPen pen);
        ~Oval();
//    store the 4 points of frame
    void setPoint(QPoint a);
    QPoint* isAround(QPoint a);
    bool isInside(QPoint a);
        void refreshData();
        void paintShape(QPainter& p, QImage* image, bool isSave);
        void paintFrame(QPainter& p);
        void move(int dx, int dy);
        void changeColor(QPainter &p, QImage *image, bool isSave);
        double calculateInfo();
protected:
    int x1, y1, x2, y2, x3, y3, x4, y4, ra, rb;

};

#endif // OVAL_H
