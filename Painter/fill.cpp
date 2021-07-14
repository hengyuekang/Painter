#include "fill.h"

Fill::Fill()
{
    type = FILL;
}
Fill::Fill(QVector<QPoint *> points, ShapeType type, QRgb rgb, QPen pen)
{
    this->points = points;
    this->type = type;
    this->rgb = rgb;
    this->pen = pen;
    refreshData();
}
Fill::~Fill() {}

void Fill::refreshData()
{
}
bool notEdge(QImage *image, int x, int y)
{
    return image->pixel(x, y) == qRgb(255,255,255);
}
void Fill::addPoint(QImage *image, int x, int y)
{
    QPoint *pt = new QPoint(x, y);
    //    QImage:hardware-independent image representation that allows direct access to the pixel data, and can be used as a paint device
    //    Sets color at (x, y) to index_or_rgb.
    image->setPixel(x, y, rgb);
    //    equal to push_back
    points.append(pt);
    //    end:border:x+1,x-1,y+1,y-1 to the extreme value
    if (x < 2 || x > image->width() - 2 || y < 2 || y > image->height() - 2)
        return;
    //    assume that edge of shape is not white
    //    if not,add point recursively
    if (notEdge(image, x + 1, y))
        addPoint(image, x + 1, y);
    if (notEdge(image, x - 1, y))
        addPoint(image, x - 1, y);
    if (notEdge(image, x, y + 1))
        addPoint(image, x, y + 1);
    if (notEdge(image, x, y - 1))
        addPoint(image, x, y - 1);
}
void Fill::paintShape(QPainter &p, QImage *image, bool isSave)
{
    //    Sets the painter's pen to be the given pen.
    p.setPen(pen);
//    QPainter q(image);
    QColor currcolor=p.pen().color();
    rgb=qRgb(currcolor.red(),currcolor.green(),currcolor.blue());
    qDebug()<<currcolor;
    //    find the area to be filled
    if (points.size() < 2)
    {
        addPoint(image, points.first()->x(), points.first()->y());
    }
    for (int i = 0; i < points.size(); i++)
    {
        //        Draws a single point at the given position using the current pen's color
        p.drawPoint(points[i]->x(), points[i]->y());
//        q.drawPoint(points[i]->x(), points[i]->y());
        if (isSave)
            image->setPixel(points[i]->x(), points[i]->y(), rgb);
    }
}
void Fill::changeColor(QPainter &p, QImage *image, bool isSave)
{
    QColor color=QColor(rgb);

    color.setGreen(255-color.green());
    color.setRed(255-color.red());
    color.setBlue(255-color.blue());
    pen.setColor(color);
    rgb=qRgb(color.red(),color.green(),color.blue());
    p.setPen(pen);
    qDebug()<<pen.color();
    for (int i = 0; i < points.size(); i++)
    {
        //        Draws a single point at the given position using the current pen's color
        p.drawPoint(points[i]->x(), points[i]->y());
//        q.drawPoint(points[i]->x(), points[i]->y());
        if (isSave)
            image->setPixel(points[i]->x(), points[i]->y(), rgb);
    }

}
