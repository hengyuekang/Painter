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
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    curShapeType = BLANK;
    curShape = NULL;
    //        The image is stored using a 24-bit RGB format (8-8-8).
    image = new QImage(700, 500, QImage::Format_RGB888);
    img = QImage(700, 500, QImage::Format_RGB888);
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
    startPos = NULL;
    endPos = NULL;
    isMove = false;
    isChange=false;
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
    //TODO:or choose the image?
    QPainter p(this);
    p.setPen(curPen);

    //one is choosen,paint all the shapes on the image
    if (curShape != NULL)
    {
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
    if (isEdit)
    {
        curShape->paintFrame(p);
    }
    if(isChange&&isEdit&&curShape!=NULL)
    {
        curShape->changeColor(p,image,true);
        isChange=false;
        update();
    }
}
void MainWindow::mousePressEvent(QMouseEvent *ev)
{
    //    still painting
    if (curShape != NULL && isEdit && curShape->isAround(ev->pos()) != NULL) //edit point
    {
        editPos = curShape->isAround(ev->pos());
    }
    else if (curShape != NULL && isEdit && curShape->isInside(ev->pos())) //move
    {
        isMove = true;
        startPos = new QPoint;
        startPos->setX(ev->pos().x());
        startPos->setY(ev->pos().y());
    }
    else
    {
        QVector<QPoint *> points;
        QPoint *evPos = new QPoint(ev->pos());
        points.push_back(evPos);
        Line *line = new Line(points, curShapeType, rgb, curPen);
        Rectangle *rectangle = new Rectangle(points, curShapeType, rgb, curPen);
        Oval *oval = new Oval(points, curShapeType, rgb, curPen);
        PolyLine *polyline = new PolyLine(points, curShapeType, rgb, curPen);
        Polygon *polygon = new Polygon(points, curShapeType, rgb, curPen);
        Fill *fill = new Fill(points, curShapeType, rgb, curPen);
        switch (curShapeType)
        {
        case LINE:
            shapes.append(line);
            curShape = line;
            break;
        case RECTANGLE:
            shapes.append(rectangle);
            curShape = rectangle;
            break;
        case OVAL:
            shapes.append(oval);
            curShape = oval;
            break;
        case POLYGON:
            if (curShape != NULL && curShape->getType() == POLYGON && !((Polygon *)curShape)->isPolyEnd())
            {
                curShape->setPoint(ev->pos());
            }
            else
            {
                shapes.append(polygon);
                polygon->startNewLine(ev->pos());
                curShape = polygon;
            }
            break;
        case FILL:
            shapes.append(fill);
            curShape = fill;
            break;
        case POLYLINE:
            if (curShape != NULL && curShape->getType() == POLYLINE && !((PolyLine *)curShape)->isPolyLineEnd())
            {
                curShape->setPoint(ev->pos());
            }
            else
            {
                shapes.append(polyline);
                polyline->startNewLine(ev->pos());
                curShape = polyline;
            }
            break;
        default:
            break;
        }

        isEdit = false;
        isMove = false;
    }
    update();
}

void MainWindow::mouseMoveEvent(QMouseEvent *ev)
{
        if (isMove)
        {
            if (endPos == NULL)
                endPos = new QPoint;
            endPos->setX(ev->pos().x());
            endPos->setY(ev->pos().y());
            curShape->move(dx, dy);
            startPos->setX(ev->pos().x());
            startPos->setY(ev->pos().y());
        }
    else if (editPos != NULL)
    {
        //        update new position
        curShape->setPoint(editPos, ev->pos());
    }
    else
    {
        if (curShape != NULL)
        {
            curShape->setPoint(ev->pos());
        }
    }
    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *ev)
{

        if (isMove)
        {
            isMove = false;
            if (endPos == NULL)
                endPos = new QPoint;
            endPos->setX(ev->pos().x());
            endPos->setY(ev->pos().y());
            curShape->move(dx, dy);
            startPos = NULL;
            endPos = NULL;
        }
    else if (editPos != NULL)
    {
        curShape->setPoint(editPos, ev->pos());
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
                curShape->setPoint(ev->pos());

            if (curShape->getType() == POLYGON && !((Polygon *)curShape)->isPolyEnd())
                isEdit = false;
            else if (curShape->getType() == POLYLINE && !((PolyLine *)curShape)->isPolyLineEnd())
                isEdit = false;
            else
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
    isMove=true;
    qDebug() << "move";
    moveDialog *ptr = new moveDialog(this);
//    input something
        Qt::WindowFlags flags = ptr->windowFlags();
//        dialog:fixedsize
        ptr->setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint);

        int ref = ptr->exec();             // 以模态方式显示对话框
        if (ref==QDialog::Accepted)        // OK键被按下,对话框关闭
        {
            // 当BtnOk被按下时,则设置对话框中的数据
            dx=ptr->getdx();
            dy=ptr->getdy();
            qDebug()<<dx<<dy;
            curShape->move(dx,dy);
        }

        // 最后删除释放对话框句柄
        delete ptr;
        update();
}


void MainWindow::on_actionChange_triggered()
{
    qDebug()<<"change";
    isChange=true;
    update();
}


void MainWindow::on_actionLengthorarea_triggered()
{
    qDebug()<<"calculate length or area";
    double res=0.0;
    if(curShape!=NULL)
    {
        res=curShape->calculateInfo();
    }
    QString resstring=QString::number(res);
    std::string str = resstring.toStdString();
    const char* ch = str.c_str();
    QMessageBox::information(this,
        tr("length or area of current shape"),
        tr(ch),
        QMessageBox::Ok,
        QMessageBox::Ok);
    update();


}

