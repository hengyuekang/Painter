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
    refreshData();
}
void Shape::refreshData() {}
Shape::~Shape() {}
ShapeType Shape::getType() const { return type; }
void Shape::setPoint(QPoint a) {}
void Shape::setPoint(QPoint *point, QPoint a)
{
    if (point == NULL)
        point = new QPoint;
    point->setX(a.x());
    point->setY(a.y());
    refreshData();
}
bool Shape::isAroundPoint(QPoint *point, QPoint a)
{
    if (fabs(point->x() - a.x()) < 2 && fabs(point->y() - a.y()) < 2)
        return true;
    return false;
}
QPoint *Shape::isAround(QPoint a) { return NULL; }
bool Shape::isInside(QPoint a) { return false; }
void Shape::paintShape(QPainter &p, QImage *image, bool isSave)
{
    QColor currcolor=p.pen().color();
    rgb=qRgb(currcolor.red(),currcolor.green(),currcolor.blue());
}
void Shape::paintVertex(QPainter &p, int x, int y)
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
void Shape::changeColor(QPainter &p, QImage *image, bool isSave) {}
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
