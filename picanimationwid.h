#ifndef PICANIMATIONWID_H
#define PICANIMATIONWID_H
#include<QWidget>
#include<QTreeWidgetItem>
class PicAnimationWid : public QWidget
{
    Q_OBJECT
public:
    explicit PicAnimationWid(QWidget *parent = nullptr);
    ~PicAnimationWid();
    void SetPixmap(QTreeWidgetItem* item);
    void Start();
    void Stop();
    void SlideNext();
    void SlidePre();
protected:
    void paintEvent(QPaintEvent* event);
signals:
private:
    float _factor;//动画因子，用来控制动画显示
    QTimer* _timer;//定时器来控制动画显示
    QPixmap _pixmap1;
    QPixmap _pixmap2;
    QTreeWidgetItem* _cur_item;//当前运行的item
    QMap<QString,QTreeWidgetItem*> _map_items;
    bool _b_start;
    void UpSelectPixmap(QTreeWidgetItem* item);
private slots:
    void TimeOut();
public slots:
    void SlotUpSelectShow(QString path);
    void SlotStartOrStop();
signals:
    void SigUpPreList(QTreeWidgetItem* );
    void SigSelectPreItem(QTreeWidgetItem* );
    void SigStart();
    void SigStop();
};

#endif // PICANIMATIONWID_H
