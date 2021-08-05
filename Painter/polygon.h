#ifndef POLYGON_H
#define POLYGON_H
#include "shape.h"

class Polygon : public Shape
{
public:
    Polygon();
    Polygon(QVector<QPoint *> points, ShapeType type, QRgb rgb, QPen pen);
    ~Polygon();

    void setStartPoint(QPoint a);
    void setEndPoint(QPoint a);
    void startNewLine(QPoint a);

    bool isPolyEnd();
    QPoint *pointAround(QPoint a);
    //        bool isInside(QPoint a);
    void updateInfo();

    void paintShape(QPainter &p, QImage *image, bool isSave);
    void paintFrame(QPainter &p);
    void changeColor(QPainter &p, QImage *image, bool isSave);
    void move(int dx, int dy);
    double calculateInfo();

private:
    bool isEnd;
};

#endif // POLYGON_H
