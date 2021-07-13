#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "shape.h"
#include "line.h"
#include "fill.h"
#include "oval.h"
#include "polygon.h"
#include "polyline.h"
#include "rectangle.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
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
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionNew_triggered()
{
    qDebug() << "new" ;
    shapes.clear();
    curShape = NULL;
        isEdit = false;
        isMove = false;
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
        ui->label->setPixmap(QPixmap::fromImage(*image));
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

