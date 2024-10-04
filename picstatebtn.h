#ifndef PICSTATEBTN_H
#define PICSTATEBTN_H

#include <QPushButton>

class PicStateBtn : public QPushButton
{
public:
    PicStateBtn(QWidget *parent = nullptr);
    void SetIcons(const QString& normal,const QString& hover,const QString& pressed,//暂停状态下的图片地址
                  const QString& normal_2,const QString& hover_2,const QString& pressed_2);//播放状态下的图片地址
    virtual ~PicStateBtn()=default;
protected:
    bool event(QEvent* event)override;

private:
    QString _normal;
    QString _hover;
    QString _pressed;
    QString _normal_2;
    QString _hover_2;
    QString _pressed_2;
    int _cur_state;

    void setNormalIcon();//设置播放图标正常状态
    void setHoverIcon();//设置播放图标悬浮状态
    void setPressIcon();//设置播放图标按下状态

    void setNormal2Icon();//设置暂停图标正常状态
    void setHover2Icon();//设置暂停图标悬浮状态
    void setPress2Icon();//设置暂停图标按下状态

public slots:
    void SlotStart();
    void SlotStop();
};

#endif // PICSTATEBTN_H
