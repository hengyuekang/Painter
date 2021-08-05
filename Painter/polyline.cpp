#include "polyline.h"
#include "line.h"
PolyLine::PolyLine()
{
    type = POLYLINE;
}
PolyLine::PolyLine(QVector<QPoint *> points, ShapeType type, QRgb rgb, QPen pen)
{
    this->points = points;
    this->type = type;
    this->rgb = rgb;
    this->isEnd = false;
    this->pen = pen;
    updateInfo();
}
PolyLine::~PolyLine() {}
void PolyLine::setStartPoint(QPoint a)
{
    //    start point
    while (points.size() < 1)
    {
        QPoint *newP = new QPoint;
        points.append(newP);
    }
    points.last()->setX(a.x());
    points.last()->setY(a.y());
    updateInfo();
}
void PolyLine::setEndPoint(QPoint a)
{
    if (points.size() > 2 && isAroundPoint(points[points.size() - 2], a))
    {
        isEnd = true;
        points.last() = points[points.size() - 2];
    }
    else
    {
        points.last()->setX(a.x());
        points.last()->setY(a.y());
        startNewLine(a);
    }
    updateInfo();
}
void PolyLine::startNewLine(QPoint a)
{
    if (isEnd)
        return;
    QPoint *newP = new QPoint;
    points.append(newP);
    points.last()->setX(a.x());
    points.last()->setY(a.y());
    updateInfo();
}
bool PolyLine::isPolyLineEnd()
{
    return isEnd;
}
QPoint *PolyLine::pointAround(QPoint a)
{
    for (int i = 0; i < points.size(); i++)
    {
        if (isAroundPoint(points[i], a))
            return points[i];
    }
    return NULL;
}
void PolyLine::updateInfo()
{
    xmin = points[0]->x();
    xmax = points[0]->x();
    ymin = points[0]->y();
    ymax = points[0]->y();
    for (int i = 0; i < points.size(); i++)
    {
        if (points[i]->x() > xmax)
            xmax = points[i]->x();
        if (points[i]->x() < xmin)
            xmin = points[i]->x();
        if (points[i]->y() > ymax)
            ymax = points[i]->y();
        if (points[i]->y() < ymin)
            ymin = points[i]->y();
    }
    center_x = (xmin + xmax) / 2;
    center_y = (ymin + ymax) / 2;
}
void PolyLine::paintShape(QPainter &p, QImage *image, bool isSave)
{

    p.setPen(pen);
    for (int i = 0; i < points.size() - 1; i++)
    {
        Line *line = new Line(points[i]->x(), points[i]->y(), points[i + 1]->x(), points[i + 1]->y(), LINE, rgb, pen);
        line->paintShape(p, image, isSave);
    }
}
void PolyLine::paintFrame(QPainter &p)
{
    QPen curPen = p.pen();
    QPen framePen(Qt::blue, 1, Qt::DashDotLine, Qt::RoundCap);
    p.setPen(framePen);
    p.drawLine(xmin, ymin, xmin, ymax);
    p.drawLine(xmin, ymin, xmax, ymin);
    p.drawLine(xmax, ymin, xmax, ymax);
    p.drawLine(xmin, ymax, xmax, ymax);
    for (int i = 0; i < points.size() - 1; i++)
    {
        highlightPoint(p, points[i]->x(), points[i]->y());
    }
    p.setPen(curPen);
}
void PolyLine::move(int dx, int dy)
{
    for (int i = 0; i < points.size(); i++)
    {
        movePoint(points[i], dx, dy);
    }
    updateInfo();
}
void PolyLine::changeColor(QPainter &p, QImage *image, bool isSave)
{
    QColor color = QColor(rgb);
    color.setGreen(255 - color.green());
    color.setRed(255 - color.red());
    color.setBlue(255 - color.blue());
    pen.setColor(color);
    p.setPen(pen);
    paintShape(p, image, isSave);
}
double PolyLine::calculateInfo()
{
    double res = 0.0;
    for (int i = 0; i < points.size() - 1; i++)
    {
        Line *line = new Line(points[i]->x(), points[i]->y(), points[i + 1]->x(), points[i + 1]->y(), LINE, rgb, pen);
        res += line->calculateInfo();
    }
    return res;
}
