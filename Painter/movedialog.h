#ifndef MOVEDIALOG_H
#define MOVEDIALOG_H

#include <QDialog>

namespace Ui {
class moveDialog;
}

class moveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit moveDialog(QWidget *parent = nullptr);
    ~moveDialog();
    int getdx();
    int getdy();

private:
    Ui::moveDialog *ui;
};

#endif // MOVEDIALOG_H
