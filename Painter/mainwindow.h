#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "shape.h"

#include <QMainWindow>
#include <QMouseEvent>
QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //    paint shape
    void paintEvent(QPaintEvent *ev);
    //    mouse functions
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
private slots:
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_action_Save_triggered();

    void on_colorButton_clicked();

    void on_fillButton_clicked();

    void on_actionLine_triggered();

    void on_action_PolyLine_triggered();

    void on_actionRectangle_triggered();

    void on_actionOval_triggered();

    void on_actionPolygon_triggered();

    void on_actionMove_triggered();

    void on_actionChange_triggered();

    void on_actionLengthorarea_triggered();

private:
    Ui::MainWindow *ui;
    //    all the shapes
    QVector<Shape *> shapes;
    //    type of shpen which is choosen
    ShapeType curShapeType;
    //    curr shape
    Shape *curShape;
    QPen curPen;
    //save the image
    QImage *image;
//    QImage img;
    QRgb rgb;
    bool isSave;
    QString saveFileName;

    //edit polyline or polygon
    QPoint *editPos;
    bool isEdit;
    //paint shape
    bool isMove;
    //  move shape
    int dx, dy;
    //    change the color of curr shape
    bool isChange;
};
#endif // MAINWINDOW_H
