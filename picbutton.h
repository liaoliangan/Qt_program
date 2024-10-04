#ifndef PICBUTTON_H
#define PICBUTTON_H

#include <QPushButton>
#include<QPixmap>
#include<QEvent>
class PicButton : public QPushButton
{
public:
    PicButton(QWidget* parent=nullptr);
    void SetIcons(const QString& normal,
                  const QString& hover,const QString& pressed);
private:
    QString _normal;
    QString _hover;
    QString _pressed;
    void setNormalIcon();//正常状态下图片的函数
    void setHoverIcon();//悬浮状态下图片的函数
    void setPressIcon();//按下状态下图片的函数
protected:
    bool event(QEvent* e)override;
};

#endif // PICBUTTON_H
