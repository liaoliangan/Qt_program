#include "picshow.h"
#include "ui_picshow.h"

PicShow::PicShow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PicShow)
{
    ui->setupUi(this);
    ui->previous_btn->SetIcons(":/icon/previous.png",
                               ":/icon/previous_hover.png",
                               ":/icon/previous_press.png");
    ui->next_btn->SetIcons(":/icon/next.png",
                           ":/icon/next_hover.png",
                           ":/icon/next_press.png");

    QGraphicsOpacityEffect* opacity_pre=new QGraphicsOpacityEffect(this);//设置透明度
    opacity_pre->setOpacity(0);
    ui->previous_btn->setGraphicsEffect(opacity_pre);

    QGraphicsOpacityEffect* opacity_next=new QGraphicsOpacityEffect(this);//透明度
    opacity_next->setOpacity(0);
    ui->next_btn->setGraphicsEffect(opacity_next);


    _animation_show_pre=new QPropertyAnimation(opacity_pre,"opacity",this);
    _animation_show_pre->setEasingCurve(QEasingCurve::Linear);//设置曲线<渐变效果或者直接显示>
    _animation_show_pre->setDuration(500);//设置动画时长


    _animation_show_next=new QPropertyAnimation(opacity_next,"opacity",this);
    _animation_show_next->setEasingCurve(QEasingCurve::Linear);
    _animation_show_next->setDuration(500);


    connect(ui->next_btn,&QPushButton::clicked,this,&PicShow::SigNextClicked);
    connect(ui->previous_btn,&QPushButton::clicked,this,&PicShow::SigPreClicked);
}

PicShow::~PicShow()
{
    delete ui;
}

void PicShow::ReloadPic()
{
    if(_selected_path!="")
    {
        const auto& width=ui->gridLayout->geometry().width();//获取矩形区域的宽度
        const auto& height=ui->gridLayout->geometry().height();//获取矩形区域的高度
        _pix_map.load(_selected_path);
        _pix_map=_pix_map.scaled(width,height,Qt::KeepAspectRatio,Qt::SmoothTransformation);//等比例拉伸图片，Qt::KeepAspectRatio保证图片比例一致，Qt::SmoothTransformation保证图片不会因为拉伸而降低像素
        ui->label->setPixmap(_pix_map);
        ui->label->setPixmap(_pix_map);
    }
}

bool PicShow::event(QEvent *event)
{
    switch(event->type())
    {
    case QEvent::Enter:
        ShowPreNextBtns(true);
        break;
    case QEvent::Leave:
        ShowPreNextBtns(false);
        break;
    default:
        break;
    }

    return QDialog::event(event);//调用基类的成员函数，来执行基类的其他的事件
}

void PicShow::ShowPreNextBtns(bool b_show)
{
    if(!b_show&&_b_btnvisible)//表示需要隐藏状态[不需要显示，并且现在处于显示状态]
    {
        _animation_show_pre->stop();//停止动画
        _animation_show_pre->setStartValue(1);//设置开始值[1为可见，0为不可见]
        _animation_show_pre->setEndValue(0);//设置结束值[1为可见，0为不可见]
        _animation_show_pre->start();

        _animation_show_next->stop();//停止动画
        _animation_show_next->setStartValue(1);//设置开始值[1为可见，0为不可见]
        _animation_show_next->setEndValue(0);//设置结束值[1为可见，0为不可见]
        _animation_show_next->start();

        _b_btnvisible=false;
        return;
    }

    if(b_show&&!_b_btnvisible)//表示需要显示状态[需要显示，并且现在处于隐藏状态]
    {
        _animation_show_pre->stop();//停止动画
        _animation_show_pre->setStartValue(0);//设置开始值[1为可见，0为不可见]
        _animation_show_pre->setEndValue(1);//设置结束值[1为可见，0为不可见]
        _animation_show_pre->start();

        _animation_show_next->stop();//停止动画
        _animation_show_next->setStartValue(0);//设置开始值[1为可见，0为不可见]
        _animation_show_next->setEndValue(1);//设置结束值[1为可见，0为不可见]
        _animation_show_next->start();

        _b_btnvisible=true;
        return;
    }
}

void PicShow::SlotSelectItem(const QString &path)
{
    _selected_path=path;
    _pix_map.load(path);
    auto width=this->width()-20;
    auto height=this->height()-20;
    _pix_map=_pix_map.scaled(width,height,Qt::KeepAspectRatio,Qt::SmoothTransformation);//等比例拉伸图片，Qt::KeepAspectRatio保证图片比例一致，Qt::SmoothTransformation保证图片不会因为拉伸而降低像素
    ui->label->setPixmap(_pix_map);
}

void PicShow::SlotUpdatePic(const QString &path)
{
    _selected_path=path;
    if(_selected_path!="")
    {
        const auto& width=ui->gridLayout->geometry().width();//获取矩形区域的宽度
        const auto& height=ui->gridLayout->geometry().height();//获取矩形区域的高度
        _pix_map.load(_selected_path);
        _pix_map=_pix_map.scaled(width,height,Qt::KeepAspectRatio,Qt::SmoothTransformation);//等比例拉伸图片，Qt::KeepAspectRatio保证图片比例一致，Qt::SmoothTransformation保证图片不会因为拉伸而降低像素
        ui->label->setPixmap(_pix_map);
        ui->label->setPixmap(_pix_map);
    }
}

void PicShow::SlotDeleteItem()
{
    _selected_path="";
}
