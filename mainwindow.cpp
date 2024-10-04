#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QMenu>//菜单
#include<QAction>//动作
#include<QDebug>
#include<QFileDialog>
#include"wizard.h"
#include"protree.h"
#include"ProTreeWidget.h"
#include"PicShow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setMinimumSize(1629,869);//设置窗口最小值
    ui->setupUi(this);
    //文件菜单
    QMenu* menu_file=menuBar()->addMenu(tr("文件(&F)"));

    //创建项目动作
    QAction* act_create_pro=new QAction(QIcon(":/icon/createpro.png"),tr("创建项目"),this);
    act_create_pro->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));//为"创建"动作创建快捷键
    menu_file->addAction(act_create_pro);//加进菜单

    //创建打开项目
    QAction* act_open_pro =new QAction(QIcon(":/icon/openpro.png"),tr("打开"),this);
    act_open_pro->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));//为"打开"动作添加快捷键
    menu_file->addAction(act_open_pro);//加进菜单

    //创建设置菜单
    QMenu* menu_set=menuBar()->addMenu(tr("设置(&S)"));//快捷键Alt+S
    //设置背景音乐
    QAction* act_music=new QAction(QIcon(":/icon/music.png"),tr("背景音乐"),this);
    act_music->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_M));//为"背景音乐"动作创建快捷键
    menu_set->addAction(act_music);

    //创建项目连接信号和槽
    connect(act_create_pro,&QAction::triggered,this,&MainWindow::SlotCreatePro);//创建项目

    connect(act_open_pro,&QAction::triggered,this,&MainWindow::SlotOpenPro);//打开目录

    _proTree = new ProTree();
    ui->proLayout->addWidget(_proTree);

    QTreeWidget* tree_widget=dynamic_cast<ProTree*>(_proTree)->GetTreeWidget();
    auto* pro_tree_widget=dynamic_cast<ProTreeWidget*>(tree_widget);
    connect(this,&MainWindow::SigOpenPro,pro_tree_widget,&ProTreeWidget::SlotOpenPro);


    _picshow=new PicShow();
    ui->picLayout->addWidget(_picshow);
    auto* pro_pic_show=dynamic_cast<PicShow*>(_picshow);
    connect(pro_tree_widget,&ProTreeWidget::SigUpdateSelected,pro_pic_show,&PicShow::SlotSelectItem);

    connect(pro_pic_show,&PicShow::SigNextClicked,pro_tree_widget,&ProTreeWidget::SlotNextShow);
    connect(pro_pic_show,&PicShow::SigPreClicked,pro_tree_widget,&ProTreeWidget::SlotPreShow);

    connect(pro_tree_widget,&ProTreeWidget::SigUpdatePic,pro_pic_show,&PicShow::SlotUpdatePic);

    connect(pro_tree_widget,&ProTreeWidget::SigClearSelected,pro_pic_show,&PicShow::SlotDeleteItem);

    connect(act_music,&QAction::triggered,
            pro_tree_widget,&ProTreeWidget::SlotSetMusic);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    auto* pro_pic_show=dynamic_cast<PicShow*>(_picshow);
    pro_pic_show->ReloadPic();//重新加载
    QMainWindow::resizeEvent(event);
}

void MainWindow::SlotCreatePro(bool)
{
    qDebug()<<"slot create pro triggered";
    Wizard wizard(this);//向导对象
    wizard.setWindowTitle(tr("创建项目"));
    auto* page=wizard.page(0);
    page->setTitle(tr("设置项目配置"));
    //连接信号和槽，把项目配置传回来
    connect(&wizard,&Wizard::SigProSettings,
            dynamic_cast<ProTree*>(_proTree),&ProTree::AddProTree);//动态类型转换将QWidget*类型转为ProTree*类型
    wizard.show();//展示
    wizard.exec();//执行
    //断开所有信号
    disconnect();
}

void MainWindow::SlotOpenPro(bool)
{

    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::Directory);//设置为目录模式
    file_dialog.setWindowTitle("选择导入的文件夹");
    file_dialog.setDirectory(QDir::currentPath());//设置默认的目录
    file_dialog.setViewMode(QFileDialog::Detail);//设置显示的模式:详情
    QStringList fileName;
    if(file_dialog.exec())//返回值有效
    {
        fileName=file_dialog.selectedFiles();
    }

    if(fileName.length()<=0)
    {
        return;
    }

    QString import_path=fileName.at(0);
    emit SigOpenPro(import_path);
}
