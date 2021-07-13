#ifndef POLYGON_H
#define POLYGON_H
#include "shape.h"


class Polygon : public Shape
{
public:
    Polygon();
    Polygon(QVector<QPoint*> points, ShapeType type, QRgb rgb, QPen pen);
        ~Polygon();

        void setPoint(QPoint a);
        void setEndPoint(QPoint a);
        void startNewLine(QPoint a);

        bool isPolyEnd();
        QPoint* isAround(QPoint a);
        bool isInside(QPoint a);
        void refreshData();

        void paintShape(QPainter& p, QImage* image, bool isSave);
        void paintFrame(QPainter& p);

        void move(int dx, int dy);

private:
    bool isEnd ;
};

#endif // POLYGON_H
