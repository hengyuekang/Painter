#include "shape.h"

Shape::Shape()
{
}
Shape::Shape(QVector<QPoint *> points, ShapeType type, QRgb rgb, QPen pen)
{
    this->points = points;
    this->type = type;
    this->rgb = rgb;
    this->pen = pen;
//    updateInfo();
}
 void Shape::updateInfo() {}
Shape::~Shape() {}
ShapeType Shape::getType() const { return type; }
 void Shape::setStartPoint(QPoint a) {}
void Shape::setPoint(QPoint *point, QPoint a)
{
    if (point == NULL)
        point = new QPoint;
    point->setX(a.x());
    point->setY(a.y());
//    updateInfo();
}
bool Shape::isAroundPoint(QPoint *point, QPoint a)
{
    if (fabs(point->x() - a.x()) < ERROR && fabs(point->y() - a.y()) < ERROR)
        return true;
    return false;
}
 QPoint *Shape::pointAround(QPoint a) { return NULL; }
bool Shape::isInside(QPoint a)
{
    if (a.x() > xmin + 1 && a.x() < xmax - 1 && a.y() > ymin + 1 && a.y() < ymax - 1)
        return true;
    return false;
}
void Shape::highlightPoint(QPainter &p, int x, int y)
{
    p.drawPoint(x - 1, y - 1);
    p.drawPoint(x - 1, y);
    p.drawPoint(x - 1, y + 1);
    p.drawPoint(x, y - 1);
    p.drawPoint(x, y);
    p.drawPoint(x, y + 1);
    p.drawPoint(x + 1, y - 1);
    p.drawPoint(x + 1, y);
    p.drawPoint(x + 1, y + 1);
}
 void Shape::paintFrame(QPainter &p) {}
void Shape::movePoint(QPoint *point, int dx, int dy)
{
    point->setX(point->x() + dx);
    point->setY(point->y() + dy);
}
 void Shape::move(int dx, int dy) {}
 double Shape::calculateInfo()
 {
     return 0.0;
 }
 void Shape::paintShape(QPainter &p, QImage *image, bool isSave){}
 void Shape::changeColor(QPainter &p, QImage *image, bool isSave){}
 void Shape::startNewLine(QPoint a){}
