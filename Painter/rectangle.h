#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "shape.h"


class Rectangle: public Shape
{
public:
    Rectangle();
    Rectangle(QVector<QPoint*> points, ShapeType type, QRgb rgb, QPen pen);
    ~Rectangle();
//    two points on the diagonal determine a rectangle
    void setPoint(QPoint a);
    QPoint* isAround(QPoint a);
    bool isInside(QPoint a);
    void refreshData();
    void paintShape(QPainter& p, QImage* image, bool isSave);
    void paintFrame(QPainter& p);
    void changeColor(QPainter &p, QImage *image, bool isSave);

    void move(int dx, int dy);
    double calculateInfo();
protected:
    int x1, y1, x2, y2, x3, y3, x4, y4;
};

#endif // RECTANGLE_H
