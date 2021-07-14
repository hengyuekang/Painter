#include "movedialog.h"
#include "ui_movedialog.h"
#include <QString>
moveDialog::moveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::moveDialog)
{
    ui->setupUi(this);
}

moveDialog::~moveDialog()
{
    delete ui;
}
int moveDialog::getdx()
{
    QString sdx=ui->xEdit->toPlainText();
    return sdx.toInt();
}
int moveDialog::getdy()
{
    QString sdy=ui->yEdit->toPlainText();
    return sdy.toInt();
}
