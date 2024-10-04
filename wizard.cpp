#include "wizard.h"
#include "ui_wizard.h"

Wizard::Wizard(QWidget *parent)
    : QWizard(parent)
    , ui(new Ui::Wizard)
{
    ui->setupUi(this);
}

Wizard::~Wizard()
{
    delete ui;
}

//创建动作的向导完成后，点击"完成"后要做的事
void Wizard::done(int result)
{
    if(result==QDialog::Rejected)//如果返回的结果是拒绝
    {
        return QWizard::done(result);
    }

    QString name,path;
    ui->wizardPage1->GetProSettings(name,path);
    emit SigProSettings(name,path);
    QWizard::done(result);
}
