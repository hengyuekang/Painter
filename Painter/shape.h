#ifndef SHAPE_H
#define SHAPE_H
#include <QVector>
#include <QPoint>
#include <QPainter>
#include <cmath>
#include <QDebug>
#define ERROR 5
enum ShapeType
{
    BLANK,
    LINE,
    POLYLINE,
    RECTANGLE,
    OVAL,
    POLYGON,
    FILL,

};
class Shape
{
public:
    Shape();
    Shape(QVector<QPoint *> points, ShapeType type, QRgb color, QPen pen);
    ~Shape();
    virtual void updateInfo();
    ShapeType getType() const;
    virtual void setStartPoint(QPoint a);
    //    *point==a
    void setPoint(QPoint *point, QPoint a);
    //  to end painting
    bool isAroundPoint(QPoint *point, QPoint a);
    //    find a Point around a
    virtual QPoint *pointAround(QPoint a);
    //    whether inside of frame
    bool isInside(QPoint a);
    //    paint the shape
    virtual void paintShape(QPainter &p, QImage *image, bool isSave);
    //    make the point more obvious
    void highlightPoint(QPainter &p, int x, int y);
    //    frame of shape
    virtual void paintFrame(QPainter &p);
    //    change the position of one point using dx and dy
    void movePoint(QPoint *point, int dx, int dy);
    virtual void move(int dx, int dy);
    virtual void changeColor(QPainter &p, QImage *image, bool isSave);
    virtual double calculateInfo();
//    for POLYGON and polyline
    virtual void startNewLine(QPoint a);
    QPoint *getCenter();
    int getMinx()
    {
        return xmin;
    }
    int getMaxx()
    {
        return xmax;
    }
    int getMiny()
    {
        return ymin;
    }
    int getMaxy()
    {
        return ymax;
    }
//    friend QDataStream& operator<<(QDataStream&,Shape*);
//    friend QDataStream& operator>>(QDataStream&,Shape*);


protected:
    ShapeType type;
    QVector<QPoint *> points;
    //    center=(min+max)/2
    int center_x, center_y;
    int xmin, xmax, ymin, ymax;
    //     color of pen
    QRgb rgb;
    QPen pen;
};

#endif // SHAPE_H
