#include "oval.h"

Oval::Oval()
{
    type = OVAL;
}
Oval::Oval(QVector<QPoint *> points, ShapeType type, QRgb rgb, QPen pen)
{
    this->points = points;
    this->type = type;
    this->rgb = rgb;
    this->pen = pen;
    updateInfo();
}
Oval::~Oval() {}
void Oval::setStartPoint(QPoint a)
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
QPoint *Oval::pointAround(QPoint a)
{
    for (int i = 0; i < points.size(); i++)
    {
        if (isAroundPoint(points[i], a))
            return points[i];
    }
    return NULL;
}
void Oval::updateInfo()
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
    ra = (xmax - xmin) / 2;
    rb = (ymax - ymin) / 2;
}
void Oval::paintShape(QPainter &p, QImage *image, bool isSave)
{

    p.setPen(pen);
    p.drawEllipse(QPoint(center_x, center_y), ra, rb);
    if (isSave)
    {
        QPainter q(image);
        q.setPen(pen);
        q.drawEllipse(QPoint(center_x, center_y), ra, rb);
    }
}
void Oval::paintFrame(QPainter &p)
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
void Oval::move(int dx, int dy)
{
    for (int i = 0; i < points.size(); i++)
    {
        movePoint(points[i], dx, dy);
    }
    updateInfo();
}
void Oval::changeColor(QPainter &p, QImage *image, bool isSave)
{
    QColor color = QColor(rgb);
    color.setGreen(255 - color.green());
    color.setRed(255 - color.red());
    color.setBlue(255 - color.blue());
    pen.setColor(color);
    p.setPen(pen);
    paintShape(p, image, isSave);
}
double Oval::calculateInfo()
{
    return 3.1415926 * ra * rb;
}
