#include "slideshowdlg.h"
#include "ui_slideshowdlg.h"
#include "prelistwid.h"
SlideShowDlg::SlideShowDlg(QWidget *parent,QTreeWidgetItem *first_item, QTreeWidgetItem *last_item)
    : QDialog(parent),_first_item(first_item),_last_item(last_item)
    , ui(new Ui::SlideShowDlg)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);//隐藏窗口:无边框版

    ui->slidepreBtn->SetIcons(":/icon/previous.png",
                              ":/icon/previous_hover.png",
                              ":/icon/previous_press.png");

    ui->slidenextBtn->SetIcons(":/icon/next.png",
                               ":/icon/next_hover.png",
                               ":/icon/next_press.png");

    ui->closeBtn->SetIcons(":/icon/closeshow.png",
                           ":/icon/closeshow_hover.png",
                           ":/icon/closeshow_press.png");

    ui->playBtn->SetIcons(":/icon/play.png",
                          ":/icon/play_hover.png",
                          ":/icon/play_press.png",
                          ":/icon/pause.png",
                          ":/icon/pause_hover.png",
                          ":/icon/pause_press.png");

    connect(ui->closeBtn,&QPushButton::clicked,this,&SlideShowDlg::close);//关闭
    connect(ui->slidenextBtn,&QPushButton::clicked,this,&SlideShowDlg::SlotSlideNext);//向后查看
    connect(ui->slidepreBtn,&QPushButton::clicked,this,&SlideShowDlg::SlotSlidePre);//向前查看

    auto* prelistwid=dynamic_cast<PreListWid*>(ui->preListWidget);
    connect(ui->picAnimation,&PicAnimationWid::SigUpPreList,
            prelistwid,&PreListWid::SlotUpPreList);


    connect(ui->picAnimation,&PicAnimationWid::SigSelectPreItem,
            prelistwid,&PreListWid::SlotUpSelect);


    connect(prelistwid,&PreListWid::SigUpSelectShow,
            ui->picAnimation,&PicAnimationWid::SlotUpSelectShow);

    connect(ui->playBtn,&PicStateBtn::clicked,ui->picAnimation,&PicAnimationWid::SlotStartOrStop);

    connect(ui->picAnimation,&PicAnimationWid::SigStart,ui->playBtn,&PicStateBtn::SlotStart);
    connect(ui->picAnimation,&PicAnimationWid::SigStop,ui->playBtn,&PicStateBtn::SlotStop);

    ui->picAnimation->SetPixmap(_first_item);
    ui->picAnimation->Start();
}

SlideShowDlg::~SlideShowDlg()
{
    delete ui;
}

void SlideShowDlg::SlotSlideNext()
{
    ui->picAnimation->SlideNext();
}

void SlideShowDlg::SlotSlidePre()
{
    ui->picAnimation->SlidePre();
}
