#include "polyline.h"
#include "line.h"
PolyLine::PolyLine()
{
    type=POLYLINE;
}
PolyLine::PolyLine(QVector<QPoint*> points, ShapeType type, QRgb rgb, QPen pen)
{
    this->points = points;
    this->type = type;
    this->rgb = rgb;
    this->isEnd = false;
    this->pen = pen;
    refreshData();
}
PolyLine::~PolyLine() {}
void PolyLine::setPoint(QPoint a)
{
//    start point
    while (points.size() < 1)
        {
            QPoint* newP = new QPoint;
            points.append(newP);
        }
        points.last()->setX(a.x());
        points.last()->setY(a.y());
        refreshData();
}
void PolyLine::setEndPoint(QPoint a)
{
    if (points.size() > 2 && isAroundPoint(points[points.size()-2], a))
        {
            isEnd = true;
            points.last() = points[points.size()-2];
        }
        else
        {
            points.last()->setX(a.x());
            points.last()->setY(a.y());
            startNewLine(a);
        }
        refreshData();
}
void PolyLine::startNewLine(QPoint a)
{
    if (isEnd)
        return ;
    QPoint* newP = new QPoint;
    points.append(newP);
    points.last()->setX(a.x());
    points.last()->setY(a.y());
    refreshData();
}
bool PolyLine::isPolyLineEnd()
{
    return isEnd;
}
QPoint* PolyLine::isAround(QPoint a)
{
    for (int i = 0; i < points.size(); i++)
    {
        if (isAroundPoint(points[i], a))
            return points[i];
    }
    return NULL;
}
bool PolyLine::isInside(QPoint a)
{
    if (!isEnd) return false;
    if ( a.x() > minX + 1 && a.x() < maxX - 1
            && a.y() > minY + 1 && a.y() < maxY - 1 )
        return true;
    return false;
}
void PolyLine::refreshData()
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
void PolyLine::paintShape(QPainter& p, QImage* image, bool isSave)
{
//    p.begin(image);

    p.setPen(pen);
    for (int i = 0; i < points.size() - 1; i++)
        {
            Line* line = new Line(points[i]->x(), points[i]->y(), points[i+1]->x(), points[i+1]->y(), LINE, rgb, pen);
            line->paintShape(p, image, isSave);
        }


}
void PolyLine::paintFrame(QPainter& p)
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
void PolyLine::move(int dx, int dy)
{
    for (int i = 0; i < points.size() ; i++)
    {
        movePoint(points[i], dx, dy);
    }
    refreshData();
}
void PolyLine::changeColor(QPainter &p, QImage *image, bool isSave)
{
    QColor color=QColor(rgb);
    color.setGreen(255-color.green());
    color.setRed(255-color.red());
    color.setBlue(255-color.blue());
    pen.setColor(color);
    p.setPen(pen);
    paintShape(p,image,isSave);

}
double PolyLine::calculateInfo()
{
    double res=0.0;
    for (int i = 0; i < points.size() - 1; i++)
        {
            Line* line = new Line(points[i]->x(), points[i]->y(), points[i+1]->x(), points[i+1]->y(), LINE, rgb, pen);
            res+=line->calculateInfo();
        }
    return res;
}
