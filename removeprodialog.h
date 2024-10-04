#ifndef REMOVEPRODIALOG_H
#define REMOVEPRODIALOG_H

#include <QDialog>

namespace Ui {
class RemoveProDialog;
}

class RemoveProDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveProDialog(QWidget *parent = nullptr);
    ~RemoveProDialog();
    bool IsRemoved();//判断是否勾选单选框
private:
    Ui::RemoveProDialog *ui;
};

#endif // REMOVEPRODIALOG_H
