#ifndef PICSHOW_H
#define PICSHOW_H

#include <QDialog>
#include<QGraphicsOpacityEffect>//动画的头文件
#include<QPropertyAnimation>
namespace Ui {
class PicShow;
}

class PicShow : public QDialog
{
    Q_OBJECT

public:
    explicit PicShow(QWidget *parent = nullptr);
    ~PicShow();
    void ReloadPic();
protected:
    bool event(QEvent* event)override;
private:
    Ui::PicShow *ui;
    QPropertyAnimation* _animation_show_pre;//控制前一个按钮的动画
    QPropertyAnimation* _animation_show_next;//控制下一个按钮的动画
    void ShowPreNextBtns(bool b_show);//是否显示两个按钮
    bool _b_btnvisible;//按钮是否可见
    QString _selected_path;
    QPixmap _pix_map;
public slots:
    void SlotSelectItem(const QString& path);
    void SlotUpdatePic(const QString& path);
    void SlotDeleteItem();
signals:
    void SigPreClicked();
    void SigNextClicked();
};

#endif // PICSHOW_H
