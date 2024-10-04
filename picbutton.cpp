#include "picbutton.h"

PicButton::PicButton(QWidget* parent)
{

}

void PicButton::SetIcons(const QString &normal,
                         const QString &hover, const QString &pressed)
{
    _normal=normal;
    _hover=hover;
    _pressed=pressed;

    QPixmap temPixmap;//临时pix变量
    temPixmap.load(normal);
    this->resize(temPixmap.size());
    this->setIcon(temPixmap);//设置图片
    this->setIconSize(temPixmap.size());//设置按钮大小
}

void PicButton::setNormalIcon()
{
    QPixmap temPixmap;//临时pix变量
    temPixmap.load(_normal);
    this->setIcon(temPixmap);//设置图片
}

void PicButton::setHoverIcon()
{
    QPixmap temPixmap;//临时pix变量
    temPixmap.load(_hover);
    this->setIcon(temPixmap);//设置图片
}

void PicButton::setPressIcon()
{
    QPixmap temPixmap;//临时pix变量
    temPixmap.load(_pressed);
    this->setIcon(temPixmap);//设置图片
}

bool PicButton::event(QEvent *e)
{
    switch(e->type())//判断事件类型
    {
    case QEvent::Enter:
        setHoverIcon();
        break;
    case QEvent::Leave:
        setNormalIcon();
        break;
    case QEvent::MouseButtonPress:
        setPressIcon();
        break;
    case QEvent::MouseButtonRelease:
        setHoverIcon();
        break;
    default:
        break;
    }

    return QPushButton::event(e);//调用基类的event()
}
