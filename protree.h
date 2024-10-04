#ifndef PROTREE_H
#define PROTREE_H

#include <QDialog>
#include<QTreeWidget>

namespace Ui {
class ProTree;
}

class ProTree : public QDialog
{
    Q_OBJECT

public:
    explicit ProTree(QWidget *parent = nullptr);
    ~ProTree();

private:
    Ui::ProTree *ui;

public:
    void AddProTree(const QString name,const QString path);
    QTreeWidget* GetTreeWidget();
};

#endif // PROTREE_H
