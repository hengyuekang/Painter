#include "oval.h"

Oval::Oval()
{
    type = OVAL;
}
Oval::Oval(QVector<QPoint*> points, ShapeType type, QRgb rgb, QPen pen)
{
    this->points = points;
    this->type = type;
    this->rgb = rgb;
    this->pen = pen;
    refreshData();
}
Oval::~Oval() {}
void Oval::setPoint(QPoint a)
{
    while (points.size() < 4)
    {
        QPoint* newP = new QPoint;
        points.push_back(newP);
    }
    points[2]->setX(a.x());
    points[2]->setY(a.y());
    refreshData();
}
QPoint* Oval::isAround(QPoint a)
{
    for (int i = 0; i < points.size(); i++)
    {
        if (isAroundPoint(points[i], a))
            return points[i];
    }
    return NULL;
}
bool Oval::isInside(QPoint a)
{
    if ( a.x() > minX + 1 && a.x() < maxX - 1
            && a.y() > minY + 1 && a.y() < maxY - 1 )
        return true;
    return false;
}
void Oval::refreshData()
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
    minX = points[0]->x();
    maxX = points[0]->x();
    minY = points[0]->y();
    maxY = points[0]->y();
    for (int i = 0; i < points.size(); i++)
    {
        if (points[i]->x() > maxX) maxX = points[i]->x();
        if (points[i]->x() < minX) minX = points[i]->x();
        if (points[i]->y() > maxY) maxY = points[i]->y();
        if (points[i]->y() < minY) minY = points[i]->y();
    }
    centerX = (minX + maxX) / 2;
    centerY = (minY + maxY) / 2;
    ra = (maxX - minX) / 2;
    rb = (maxY - minY) / 2;
}
void Oval::ovalPoints(QPainter& p,QImage* image, bool isSave, int centerX, int centerY, int x, int y)
{
    p.drawPoint(x + centerX, y + centerY);
    p.drawPoint(-x + centerX, y + centerY);
    p.drawPoint(x + centerX, -y + centerY);
    p.drawPoint(-x + centerX, -y + centerY);
    if (isSave)
    {
        image->setPixel(x + centerX, y + centerY, rgb);
        image->setPixel(-x + centerX, y + centerY, rgb);
        image->setPixel(x + centerX, -y + centerY, rgb);
        image->setPixel(-x + centerX, -y + centerY, rgb);
    }
}
void Oval::paintShape(QPainter& p, QImage* image, bool isSave)
{

//    p.begin(image);

    p.setPen(pen);
    int x = 0, y = rb;
    ovalPoints(p, image, isSave, centerX, centerY, x, y);

    double d1 = pow(rb, 2) - pow(ra, 2) * rb + 0.25 * pow(ra, 2);
    while ( pow(ra,2) * (y - 0.5) > pow(rb, 2) * (x + 1) )
    {
        //在区域一
        if (d1 < 0) //选择E点
        {
            d1 += pow(rb, 2)*(2 * x + 3);
        }
        else          //选择SE点
        {
            d1 += pow(rb, 2)*(2 * x + 3) + pow(ra, 2)*(-2 * y + 2);
            y--;
        }
        x++;
        ovalPoints(p, image, isSave, centerX, centerY, x, y);

    }

    double d2 = pow(rb, 2) * pow(x + 0.5, 2) + pow(ra, 2) * pow(y - 1, 2) - pow(ra * rb, 2);
    while (y > 0)       //区域二
    {
        if (d2 < 0)       //选择SE点
        {
            d2 += pow(rb, 2) * (2 * x + 2) + pow(ra, 2)*(-2 * y + 3);
            x++;
        }
        else
        {
            d2 += pow(ra, 2) * (-2 * y + 3);
        }
        y--;
        ovalPoints(p, image, isSave, centerX, centerY, x, y);
    }


}
void Oval::paintFrame(QPainter& p)
{
    QPen curPen = p.pen();
    QPen framePen(Qt::blue, 1 ,Qt::DashDotLine, Qt::RoundCap);
    p.setPen(framePen);
    p.drawLine(x1, y1, x2, y2);
    p.drawLine(x2, y2, x3, y3);
    p.drawLine(x3, y3, x4, y4);
    p.drawLine(x4, y4, x1, y1);
    paintVertex(p, x1, y1);
    paintVertex(p, x3, y3);
    p.setPen(curPen);
}
void Oval::move(int dx, int dy)
{
    for (int i = 0; i < points.size(); i++)
    {
        movePoint(points[i], dx, dy);
    }
    refreshData();
}
void Oval::changeColor(QPainter &p, QImage *image, bool isSave)
{
    QColor color=QColor(rgb);
    color.setGreen(255-color.green());
    color.setRed(255-color.red());
    color.setBlue(255-color.blue());
    pen.setColor(color);
    p.setPen(pen);
    paintShape(p,image,isSave);

}
double Oval::calculateInfo()
{
    return 3.1415926*ra*rb;
}
