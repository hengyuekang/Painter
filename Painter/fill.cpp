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
Fill::~Fill() { }

void Fill::refreshData()
{

}
bool notEdge(QImage* image, int x, int y)
{
    return image->pixel(x, y) == qRgb(255, 255, 255);
}
void Fill::addPoint(QImage* image, int x, int y)
{
    QPoint* pt = new QPoint(x, y);
//    QImage:hardware-independent image representation that allows direct access to the pixel data, and can be used as a paint device
//    Sets color at (x, y) to index_or_rgb.
    image->setPixel(x, y, rgb);
//    equal to push_back
    points.append(pt);

    if (x < 2 || x > image->width() -2 || y < 2 || y > image->height()-2)
        return;

    if (notEdge(image, x+1, y))
        addPoint(image, x+1, y);
    if (notEdge(image, x-1, y))
        addPoint(image, x-1, y);
    if (notEdge(image, x, y+1))
        addPoint(image, x, y+1);
    if (notEdge(image, x, y-1))
        addPoint(image, x, y-1);
}
