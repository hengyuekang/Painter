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
    updateInfo();
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
    updateInfo();
}
Line::~Line() {}
void Line::setStartPoint(QPoint a)
{
    while (points.size() < 2)
    {
        QPoint *newP = new QPoint;
        points.push_back(newP);
    }
    points[1]->setX(a.x());
    points[1]->setY(a.y());
    updateInfo();
}
QPoint *Line::pointAround(QPoint a)
{
    if (isAroundPoint(points[0], a))
        return points[0];
    if (isAroundPoint(points[1], a))
        return points[1];
    return NULL;
}
//bool Line::isInside(QPoint a)
//{
//    if (a.x() > minX + 1 && a.x() < maxX - 1 && a.y() > minY + 1 && a.y() < maxY - 1)
//        return true;
//    return false;
//}
void Line::updateInfo()
{
    if (points.size() == 0)
        return;
    x1 = points[0]->x();
    y1 = points[0]->y();
    x2 = points.last()->x();
    y2 = points.last()->y();
    if (x1 > x2)
    {
        xmin = x2;
        xmax = x1;
    }
    else
    {
        xmin = x1;
        xmax = x2;
    }
    if (y1 > y2)
    {
        ymin = y2;
        ymax = y1;
    }
    else
    {
        ymin = y1;
        ymax = y2;
    }
    center_x = (x1 + x2) / 2;
    center_y = (y1 + y2) / 2;
}
void Line::paintShape(QPainter &p, QImage *image, bool isSave)
{

    p.setPen(pen);

    p.drawLine(x1, y1, x2, y2);
//    highlightPoint(p,center_x,center_y);
    if (isSave)
    {
        QPainter q(image);
        q.setPen(pen);
        q.drawLine(x1, y1, x2, y2);
    }
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
    highlightPoint(p, x1, y1);
    highlightPoint(p, x2, y2);
    p.setPen(curPen);
}
void Line::move(int dx, int dy)
{
    for (int i = 0; i < points.size(); i++)
        movePoint(points[i], dx, dy);
    updateInfo();
}
void Line::changeColor(QPainter &p, QImage *image, bool isSave)
{
    QColor color = QColor(rgb);
    color.setGreen(255 - color.green());
    color.setRed(255 - color.red());
    color.setBlue(255 - color.blue());
    pen.setColor(color);
    p.setPen(pen);
    paintShape(p, image, isSave);
}
double Line::calculateInfo()
{
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}
