#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "shape.h"
#include "line.h"
#include "fill.h"
#include "oval.h"
#include "polygon.h"
#include "polyline.h"
#include "rectangle.h"
#include "movedialog.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QColorDialog>
#include <string>
#define WIDTH 700
#define HEIGHT 500
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    curShapeType = BLANK;
    curShape = NULL;
    //        The image is stored using a 24-bit RGB format (8-8-8).
    image = new QImage(WIDTH, HEIGHT, QImage::Format_RGB888);
//    img = QImage(WIDTH, HEIGHT, QImage::Format_RGB888);
    rgb = qRgb(0, 0, 0);
    isSave = false;
    //      white background
    QRgb bgRgb = qRgb(255, 255, 255); //background: white
    for (int i = 0; i < image->width(); i++)
        for (int j = 0; j < image->height(); j++)
            image->setPixel(i, j, bgRgb);
    //                edit
    editPos = NULL;
    isEdit = false;
    //                    paint
    isMove = false;
    isChange = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::paintEvent(QPaintEvent *ev)
{
    if (isSave)
    {
        QRgb bgRgb = qRgb(255, 255, 255);
        for (int i = 0; i < image->width(); i++)
            for (int j = 0; j < image->height(); j++)
                image->setPixel(i, j, bgRgb);
    }
    QPainter p(this);
    p.setPen(curPen);

    //begin to paint a shape
    if (curShape != NULL)
    {
//        other shapes
        for (int i = 0; i < shapes.size() - 1; i++)
        {
            shapes[i]->paintShape(p, image, true);
        }
        curShape->paintShape(p, image, isSave);
    }
    else
    {
        for (int i = 0; i < shapes.size(); i++)
        {
            shapes[i]->paintShape(p, image, true);
        }
    }

    if (isSave)
    {
        qDebug() << "now save" << saveFileName;
        image->save(saveFileName);
        isSave = false;
    }
    if (isEdit&& curShape != NULL)
    {
        curShape->paintFrame(p);
    }
    if (isChange && isEdit && curShape != NULL)
    {
        curShape->changeColor(p, image, true);
        isChange = false;
        update();
    }
}
void MainWindow::mousePressEvent(QMouseEvent *ev)
{
    // exit shape using a point
    if (curShape != NULL && isEdit && curShape->pointAround(ev->pos()) != NULL) //edit shape
    {
        editPos = curShape->pointAround(ev->pos());
    }
    else
    {
        //        a new shape
        QVector<QPoint *> points;
        QPoint *evPos = new QPoint(ev->pos());
        points.push_back(evPos);
        switch (curShapeType)
        {
        case LINE:
            curShape = new Line(points, curShapeType, rgb, curPen);
            shapes.append(curShape);
            break;
        case RECTANGLE:
            curShape= new Rectangle(points, curShapeType, rgb, curPen);
            shapes.append(curShape);
            break;
        case OVAL:
            curShape= new Oval(points, curShapeType, rgb, curPen);
            shapes.append(curShape);
            break;
        case POLYGON:
//            not end
            if (curShape != NULL && curShape->getType() == POLYGON && !((Polygon *)curShape)->isPolyEnd())
            {
                curShape->setStartPoint(ev->pos());
            }
            else
            {
                curShape=new Polygon(points, curShapeType, rgb, curPen);
                shapes.append(curShape);
                curShape->startNewLine(ev->pos());
            }
            break;
        case FILL:
            curShape=new Fill(points, curShapeType, rgb, curPen);
            shapes.append(curShape);
            break;
        case POLYLINE:
            if (curShape != NULL && curShape->getType() == POLYLINE && !((PolyLine *)curShape)->isPolyLineEnd())
            {
                curShape->setStartPoint(ev->pos());
            }
            else
            {
                curShape=new PolyLine(points, curShapeType, rgb, curPen);
                shapes.append(curShape);
                curShape->startNewLine(ev->pos());
            }
            break;
        default:
            break;
        }

        isEdit = false;
    }
    update();
}

void MainWindow::mouseMoveEvent(QMouseEvent *ev)
{

    if (editPos != NULL)
    {
        curShape->setPoint(editPos, ev->pos());
    }
    else
    {
//        paint the shape
        if (curShape != NULL)
        {
            curShape->setStartPoint(ev->pos());
        }
    }
    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *ev)
{

    if (editPos != NULL)
    {
//        curShape->setPoint(editPos,ev->pos());
        editPos = NULL;
    }
    else
    {
        if (curShape != NULL)
        {
            //            end the polygon
            if (curShape->getType() == POLYGON)
                ((Polygon *)curShape)->setEndPoint(ev->pos());
            else if (curShape->getType() == POLYLINE)
                ((PolyLine *)curShape)->setEndPoint(ev->pos());
            else
                curShape->setStartPoint(ev->pos());

            if (curShape->getType() == POLYGON && !((Polygon *)curShape)->isPolyEnd())
                isEdit = false;
            else if (curShape->getType() == POLYLINE && !((PolyLine *)curShape)->isPolyLineEnd())
                isEdit = false;
            else
//                painting ends
                isEdit = true;
        }
    }

    update();
}
void MainWindow::on_actionNew_triggered()
{
    qDebug() << "new";
    shapes.clear();
    curShape = NULL;
    isEdit = false;
    isMove = false;
    ui->label->clear();
    QRgb bgRgb = qRgb(255, 255, 255);
    for (int i = 0; i < image->width(); i++)
        for (int j = 0; j < image->height(); j++)
            image->setPixel(i, j, bgRgb);
    update();
}

void MainWindow::on_actionOpen_triggered()
{
    qDebug() << "open";
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,
                                            tr("Open Config"), "", tr("JPG Files (*.bmp;; *.png;; *.jpg;; *.tif;; *.GIF)"));
    qDebug() << fileName;
    //        get the image
    image->load(fileName);
    //        The QPixmap class is an off-screen image representation that can be used as a paint device.
    //        use QLabel to show a image
    ui->label->clear();
    ui->label->setPixmap(QPixmap::fromImage(*image));
    update();
}

void MainWindow::on_action_Save_triggered()
{
    qDebug() << "save";
    isSave = true;
    saveFileName = QFileDialog::getSaveFileName(this,
                                                tr("Save Image"),
                                                "",
                                                tr("JPG Files (*.jpg)"));
    qDebug() << saveFileName;
    image->save(saveFileName);
    update();
}

void MainWindow::on_colorButton_clicked()
{
    qDebug() << "color";
    //    choose the color
    QColor color = QColorDialog::getColor(Qt::red, this, "choose color", QColorDialog::DontUseNativeDialog);
    curPen.setColor(color);
    rgb = qRgb(color.red(), color.green(), color.blue());
    qDebug() << color.name() << color.red() << color.green() << color.blue();
    update();
}

void MainWindow::on_fillButton_clicked()
{
    qDebug() << "fill";
    curShapeType = FILL;
    curShape = NULL;
    isEdit = false;
    isMove = false;
    update();
}

void MainWindow::on_actionLine_triggered()
{
    qDebug() << "line";
    curShapeType = LINE;
    curShape = NULL;
    isEdit = false;
    isMove = false;
    update();
}

void MainWindow::on_action_PolyLine_triggered()
{
    qDebug() << "polyline";
    curShapeType = POLYLINE;
    curShape = NULL;
    isEdit = false;
    isMove = false;
    update();
}

void MainWindow::on_actionRectangle_triggered()
{
    qDebug() << "rectangle";
    curShapeType = RECTANGLE;
    curShape = NULL;
    isEdit = false;
    isMove = false;
    update();
}

void MainWindow::on_actionOval_triggered()
{
    qDebug() << "oval";
    curShapeType = OVAL;
    curShape = NULL;
    isEdit = false;
    isMove = false;
    update();
}

void MainWindow::on_actionPolygon_triggered()
{
    qDebug() << "polygon";
    curShapeType = POLYGON;
    curShape = NULL;
    isEdit = false;
    isMove = false;
    update();
}

void MainWindow::on_actionMove_triggered()
{
    isMove = true;
    qDebug() << "move";
    moveDialog *ptr = new moveDialog(this);
    //    input something
    Qt::WindowFlags flags = ptr->windowFlags();
    //        dialog:fixed size
    ptr->setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint);

    int ref = ptr->exec();
    if (ref == QDialog::Accepted)
    {
        dx = ptr->getdx();
        dy = ptr->getdy();
        qDebug() << dx << dy;
        curShape->move(dx, dy);
    }

    delete ptr;
    update();
}

void MainWindow::on_actionChange_triggered()
{
    qDebug() << "change";
    isChange = true;
    update();
}

void MainWindow::on_actionLengthorarea_triggered()
{
    qDebug() << "calculate length or area";
    double res = 0.0;
    if (curShape != NULL)
    {
        res = curShape->calculateInfo();
    }
    QString resstring = QString::number(res);
    std::string str = resstring.toStdString();
    const char *ch = str.c_str();
    QMessageBox::information(this,
                             tr("length or area of current shape"),
                             tr(ch),
                             QMessageBox::Ok,
                             QMessageBox::Ok);
    update();
}

