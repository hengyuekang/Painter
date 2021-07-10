#ifndef SHAPE_H
#define SHAPE_H
enum ShapeType
{
    BLANK,
    LINE,
    POLYLINE,
    RECTANGLE,
    OVAL,
    POLYGON,
    PEN,
    FILL,
    FILLRECT,
    FILLOVAL,
    FILLPOLY
};
#include <QVector>
#include <QPoint>
#include <QPainter>
#include <cmath>
#include <QDebug>

class Shape
{
public:
    Shape();
    Shape(QVector<QPoint *> points, ShapeType type, QRgb rgb, QPen pen);
    ~Shape();
    //    update info
    virtual void refreshData();
    ShapeType getType() const;
    //    change the point for control using point a
    virtual void setPoint(QPoint a);
    void setPoint(QPoint *point, QPoint a);
    //    whether a is on one of points(<=1 error)
    bool isAroundPoint(QPoint *point, QPoint a);
    //    find a Point around a
    virtual QPoint *isAround(QPoint a);
    //    whether a is inside the shape according to min and max
    virtual bool isInside(QPoint a);
    //    paint the shape
    virtual void paintShape(QPainter &p, QImage *image, bool isSave);
    //    paint a squre for mouse to control the shape
    void paintVertex(QPainter &p, int x, int y);
    //    paint a rectangle for the whole shape making it easier to know the frame
    virtual void paintFrame(QPainter &p);
    //    change the position of one point using dx and dy
    void movePoint(QPoint *point, int dx, int dy);
    //    move the whole shape
    virtual void move(int dx, int dy);

protected:
    ShapeType type;
    QVector<QPoint *> points;
    //    center=(min+max)/2
    int centerX, centerY;
    int minX, maxX, minY, maxY;
    //     color of pen
    QRgb rgb;
    QPen pen;
};

#endif // SHAPE_H
