#ifndef PROSETPAGE_H
#define PROSETPAGE_H

#include <QWizardPage>

namespace Ui {
class ProSetPage;
}

class ProSetPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ProSetPage(QWidget *parent = nullptr);
    ~ProSetPage();
    void GetProSettings(QString& name,QString& path);//把填写的信息和路径返回
protected:
    bool isComplete() const;
private slots:
    void on_pushButton_clicked();
private:
    Ui::ProSetPage *ui;
};

#endif // PROSETPAGE_H
