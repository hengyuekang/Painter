#include "rectangle.h"
#include "line.h"
Rectangle::Rectangle()
{
    type = RECTANGLE;
}
Rectangle::Rectangle(QVector<QPoint *> points, ShapeType type, QRgb rgb, QPen pen)
{
    this->points = points;
    this->type = type;
    this->rgb = rgb;
    this->pen = pen;
    updateInfo();
}
Rectangle::~Rectangle() {}
void Rectangle::setStartPoint(QPoint a)
{
    while (points.size() < 4)
    {
        QPoint *newP = new QPoint;
        points.push_back(newP);
    }
    points[2]->setX(a.x());
    points[2]->setY(a.y());
    updateInfo();
}
QPoint *Rectangle::pointAround(QPoint a)
{
    for (int i = 0; i < points.size(); i++)
    {
        if (isAroundPoint(points[i], a))
            return points[i];
    }
    return NULL;
}
void Rectangle::updateInfo()
{
    x1 = points[0]->x();
    y1 = points[0]->y();
    if (points.size() < 4)
    {
        x2 = x1;
        y2 = y1;
        x3 = x1;
        y3 = y1;
        x4 = x1;
        y4 = y1;
    }
    else
    {
        //clockwise
        points[1]->setX(points[2]->x());
        points[1]->setY(points[0]->y());
        points[3]->setX(points[0]->x());
        points[3]->setY(points[2]->y());

        x2 = points[1]->x();
        y2 = points[1]->y();
        x3 = points[2]->x();
        y3 = points[2]->y();
        x4 = points[3]->x();
        y4 = points[3]->y();
    }
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
void Rectangle::paintShape(QPainter &p, QImage *image, bool isSave)
{
    p.setPen(pen);
    Line *l12 = new Line(x1, y1, x2, y2, LINE, rgb, pen);
    l12->paintShape(p, image, isSave);
    Line *l23 = new Line(x2, y2, x3, y3, LINE, rgb, pen);
    l23->paintShape(p, image, isSave);
    Line *l34 = new Line(x3, y3, x4, y4, LINE, rgb, pen);
    l34->paintShape(p, image, isSave);
    Line *l41 = new Line(x4, y4, x1, y1, LINE, rgb, pen);
    l41->paintShape(p, image, isSave);
}
void Rectangle::paintFrame(QPainter &p)
{
    QPen curPen = p.pen();
    QPen framePen(Qt::blue, 1, Qt::DashDotLine, Qt::RoundCap);
    p.setPen(framePen);
    p.drawLine(x1, y1, x2, y2);
    p.drawLine(x2, y2, x3, y3);
    p.drawLine(x3, y3, x4, y4);
    p.drawLine(x4, y4, x1, y1);
    highlightPoint(p, x1, y1);
    highlightPoint(p, x3, y3);
    p.setPen(curPen);
}
void Rectangle::move(int dx, int dy)
{
    for (int i = 0; i < points.size(); i++)
    {
        movePoint(points[i], dx, dy);
    }
    updateInfo();
}
void Rectangle::changeColor(QPainter &p, QImage *image, bool isSave)
{
    QColor color = QColor(rgb);
    color.setGreen(255 - color.green());
    color.setRed(255 - color.red());
    color.setBlue(255 - color.blue());
    pen.setColor(color);
    p.setPen(pen);
    paintShape(p, image, isSave);
}
double Rectangle::calculateInfo()
{
    double width = fabs(xmax - xmin);
    double length = fabs(ymax - ymin);
    return width * length;
}
