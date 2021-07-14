#include "polygon.h"
#include "line.h"
#include "qvector.h"
#include <algorithm>
Polygon::Polygon()
{
type = POLYGON;
}
Polygon::Polygon(QVector<QPoint*> points, ShapeType type, QRgb rgb, QPen pen)
{
    this->points = points;
    this->type = type;
    this->rgb = rgb;
    this->isEnd = false;
    this->pen = pen;
    refreshData();
}
Polygon::~Polygon() {}
void Polygon::setPoint(QPoint a)
{
    while (points.size() < 1) //delete
    {
        QPoint* newP = new QPoint;
        points.append(newP);
    }
    points.last()->setX(a.x());
    points.last()->setY(a.y());
    refreshData();
}

void Polygon::setEndPoint(QPoint a)
{
    if (points.size() > 2 && isAroundPoint(points[0], a))
    {
        isEnd = true;
        points.last() = points[0];
    }
    else
    {
        points.last()->setX(a.x());
        points.last()->setY(a.y());
        startNewLine(a);
    }
    refreshData();
}
void Polygon::startNewLine(QPoint a)
{
    if (isEnd)
        return ;
    QPoint* newP = new QPoint;
    points.append(newP);
    points.last()->setX(a.x());
    points.last()->setY(a.y());
    refreshData();
}
bool Polygon::isPolyEnd() { return isEnd; }
QPoint* Polygon::isAround(QPoint a)
{
    for (int i = 0; i < points.size(); i++)
    {
        if (isAroundPoint(points[i], a))
            return points[i];
    }
    return NULL;
}
bool Polygon::isInside(QPoint a)
{
    if (!isEnd) return false;
    if ( a.x() > minX + 1 && a.x() < maxX - 1
            && a.y() > minY + 1 && a.y() < maxY - 1 )
        return true;
    return false;
}
void Polygon::refreshData()
{
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
}
void Polygon::paintShape(QPainter& p, QImage* image, bool isSave)
{
    p.setPen(pen);
    for (int i = 0; i < points.size() - 1; i++)
    {
        Line* line = new Line(points[i]->x(), points[i]->y(), points[i+1]->x(), points[i+1]->y(), LINE, rgb, pen);
        line->paintShape(p, image, isSave);
    }
    if (isEnd)
    {
        Line* line = new Line(points[0]->x(), points[0]->y(), points.last()->x(), points.last()->y(), LINE, rgb, pen);
        line->paintShape(p, image, isSave);
    }
}
void Polygon::paintFrame(QPainter& p)
{
    QPen curPen = p.pen();
    QPen framePen(Qt::blue, 1 ,Qt::DashDotLine, Qt::RoundCap);
    p.setPen(framePen);
    p.drawLine(minX, minY, minX, maxY);
    p.drawLine(minX, minY, maxX, minY);
    p.drawLine(maxX, minY, maxX, maxY);
    p.drawLine(minX, maxY, maxX, maxY);
    for (int i = 0; i < points.size() - 1; i++)
    {
        paintVertex(p, points[i]->x(), points[i]->y());
    }
    p.setPen(curPen);
}
void Polygon::move(int dx, int dy)
{
    for (int i = 0; i < points.size() - 1; i++)
    {
        movePoint(points[i], dx, dy);
    }
    refreshData();
}
void Polygon::changeColor(QPainter &p, QImage *image, bool isSave)
{
    QColor color=QColor(rgb);
    color.setGreen(255-color.green());
    color.setRed(255-color.red());
    color.setBlue(255-color.blue());
    pen.setColor(color);
    p.setPen(pen);
    paintShape(p,image,isSave);

}
double angleVector(const QPoint &basic,const QPoint &curr)
{
    double radian = ((basic.x() * curr.x()) + (basic.y() * curr.y())) / (sqrt((basic.x() * basic.x()) + (basic.y() * basic.y())) * (sqrt((curr.x() * curr.x()) + (curr.y() * curr.y() ))));
        return fabs((acos(radian)) * 180.0 / 3.1415926);
}
double Polygon::calculateInfo()
{
    QPoint basic_vector(maxX-minX,0);
    QVector<QPoint> temp_points;
    for(int i=0;i<points.size();i++)
    {
        temp_points.push_back(*(points[i]));
    }
    std::sort(temp_points.begin(),temp_points.end(),[basic_vector,this](const QPoint &p1,decltype(p1) p2){return angleVector(basic_vector,QPoint(p1.x()-minX,p1.y()-minY))<angleVector(basic_vector,QPoint(p2.x()-minX,p2.y()-minY));});
    int point_num = temp_points.size();
        if(point_num < 3)return 0.0;
        double s = temp_points[0].y() * (temp_points[point_num-1].x() - temp_points[1].x());
        for(int i = 1; i < point_num; ++i)
            s += temp_points[i].y() * (temp_points[i-1].x() - temp_points[(i+1)%point_num].x());
        return fabs(s/2.0);
}
