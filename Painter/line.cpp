#include "line.h"

Line::Line()
{
    type = LINE;
}
Line::Line(QVector<QPoint *> points, ShapeType type, QRgb rgb, QPen pen)
{
    this->points = points;
    this->type = type;
    this->rgb = rgb;
    this->pen = pen;
    refreshData();
}
Line::Line(int x1, int y1, int x2, int y2, ShapeType type, QRgb rgb, QPen pen)
{
    QPoint *p1 = new QPoint(x1, y1);
    QPoint *p2 = new QPoint(x2, y2);
    points.push_back(p1);
    points.push_back(p2);
    this->type = type;
    this->rgb = rgb;
    this->pen = pen;
    refreshData();
}
Line::~Line() {}
void Line::setPoint(QPoint a)
{
    while (points.size() < 2)
    {
        QPoint *newP = new QPoint;
        points.push_back(newP);
    }
    points[1]->setX(a.x());
    points[1]->setY(a.y());
    refreshData();
}
QPoint *Line::isAround(QPoint a)
{
    if (isAroundPoint(points[0], a))
        return points[0];
    if (isAroundPoint(points[1], a))
        return points[1];
    return NULL;
}
bool Line::isInside(QPoint a)
{
    if (a.x() > minX + 1 && a.x() < maxX - 1 && a.y() > minY + 1 && a.y() < maxY - 1)
        return true;
    return false;
}
void Line::refreshData()
{
    if (points.size() == 0)
        return;
    x1 = points[0]->x();
    y1 = points[0]->y();
    x2 = points.last()->x();
    y2 = points.last()->y();
    if (x1 > x2)
    {
        minX = x2;
        maxX = x1;
    }
    else
    {
        minX = x1;
        maxX = x2;
    }
    if (y1 > y2)
    {
        minY = y2;
        maxY = y1;
    }
    else
    {
        minY = y1;
        maxY = y2;
    }
    centerX = (x1 + x2) / 2;
    centerY = (y1 + y2) / 2;
}
void Line::paintShape(QPainter &p, QImage *image, bool isSave)
{
    p.setPen(pen);
    //DDA
    //     double dx, dy, e, x, y;
    //     dx = x2 - x1;
    //     dy = y2 - y1;
    //     e =( fabs(dx) > fabs(dy) ) ? fabs(dx) : fabs(dy);
    //     dx /= e;
    //     dy /= e;
    //     x = x1;
    //     y = y1;
    //     for (int i = 1; i <= e; i++)
    //     {
    //         int px = (int)(x + 0.5);
    //         int py = (int)(y + 0.5);
    //         p.drawPoint(px, py);
    //         if (isSave)
    //             image->setPixel(px, py, rgb);
    //         x += dx;
    //         y += dy;
    //     }
    p.drawLine(x1, y1, x2, y2);
}
void Line::paintFrame(QPainter &p)
{
    QPen curPen = p.pen();
    QPen framePen(Qt::blue, 1, Qt::DashDotLine, Qt::RoundCap);
    p.setPen(framePen);
    p.drawLine(x1, y1, x1, y2);
    p.drawLine(x1, y1, x2, y1);
    p.drawLine(x2, y1, x2, y2);
    p.drawLine(x1, y2, x2, y2);
    paintVertex(p, x1, y1);
    paintVertex(p, x2, y2);
    p.setPen(curPen);
}
void Line::move(int dx, int dy)
{
    for (int i = 0; i < points.size(); i++)
        movePoint(points[i], dx, dy);
    refreshData();
}
