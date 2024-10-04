#include "prosetpage.h"
#include "ui_prosetpage.h"
#include<QLineEdit>
#include<QDir>
#include<QFileDialog>
ProSetPage::ProSetPage(QWidget *parent)
    : QWizardPage(parent)
    , ui(new Ui::ProSetPage)
{
    ui->setupUi(this);
    registerField("proPath",ui->lineEdit_2);//注册域
    registerField("proName",ui->lineEdit);//注册域
    connect(ui->lineEdit,&QLineEdit::textEdited,this,&ProSetPage::completeChanged);
    //completeChanged信号发出后isComplete触发判断页面是否完成
    connect(ui->lineEdit_2,&QLineEdit::textEdited,this,&ProSetPage::completeChanged);
    QString curPath = QDir::currentPath();
    ui->lineEdit_2->setText(curPath);
    ui->lineEdit_2->setCursorPosition(ui->lineEdit_2->text().size());

    //有输入时后面有一个一键清除的按钮
    ui->lineEdit->setClearButtonEnabled(true);
    ui->lineEdit_2->setClearButtonEnabled(true);
}

ProSetPage::~ProSetPage()
{
    delete ui;
}


//把填写的信息和路径返回
void ProSetPage::GetProSettings(QString &name, QString &path)
{
    name=ui->lineEdit->text();
    path=ui->lineEdit_2->text();
}

bool ProSetPage::isComplete() const
{
     //当两个字段都是空，退出
    if(ui->lineEdit->text()==""||ui->lineEdit_2->text()=="")
         return false;
    //判断目录是否存在
    QDir dir(ui->lineEdit_2->text());
    if(!dir.exists())
    {
        ui->tips->setText("project path  is not exist");
        return false;
    }
    //判断路径是否存在
    QString absFilePath=dir.absoluteFilePath(ui->lineEdit->text());
    QDir dist_dir(absFilePath);
    //如果项目存在，不合理，退出
    if(dist_dir.exists())
    {
        ui->tips->setText("project has exists,change path or name!");
        return false;
    }
    //如果没有错误，tips设为空
    ui->tips->setText("");

    //当重写了子类的函数时,还想要基类的功能，要调用基类的函数
    return QWizardPage::isComplete();
}

void ProSetPage::on_pushButton_clicked()
{
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::Directory);//设置打开模式，打开目录而不是文件
    file_dialog.setWindowTitle("选择导入的文件夹");
    auto path=QDir::currentPath();
    file_dialog.setDirectory(path);
    file_dialog.setViewMode(QFileDialog::Detail);//以详情的模式来展示

    QStringList fileNames;
    if(file_dialog.exec())//对话框执行
    {
        fileNames=file_dialog.selectedFiles();//所有文件
    }
    if(fileNames.length()<=0)//没有文件
    {
        return;
    }
    QString import_path=fileNames.at(0);
    ui->lineEdit_2->setText(import_path);//设置导入路径
}

