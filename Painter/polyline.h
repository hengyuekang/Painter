#ifndef POLYLINE_H
#define POLYLINE_H
#include "shape.h"

class PolyLine : public Shape
{
public:
    PolyLine();
    PolyLine(QVector<QPoint *> points, ShapeType type, QRgb rgb, QPen pen);
    ~PolyLine();

    void setStartPoint(QPoint a);
    //    end the polyline
    void setEndPoint(QPoint a);
    //    add a new point
    void startNewLine(QPoint a);
    //    for user to choose the shape
    bool isPolyLineEnd();
    QPoint *pointAround(QPoint a);
    //    bool isInside(QPoint a);
    void updateInfo();
    void paintShape(QPainter &p, QImage *image, bool isSave);
    void paintFrame(QPainter &p);
    void changeColor(QPainter &p, QImage *image, bool isSave);

    void move(int dx, int dy);
    double calculateInfo();

private:
    bool isEnd;
};

#endif // POLYLINE_H
