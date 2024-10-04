#ifndef PROTREEWIDGET_H
#define PROTREEWIDGET_H

#include <QTreeWidget>
#include<QTreeWidgetItem>
#include<qset.h>
#include<QAction>
#include<QProgressDialog>
#include"protreethread.h"
#include"OpenTreeThread.h"
#include<QtMultimedia/QMediaPlayer>
// #include <QtMultimedia/QMediaPlaylist>//找不到头文件
class SlideShowDlg;//前向声明，防止互引用

class ProTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    ProTreeWidget(QWidget* parent = nullptr);
    void AddProToTree(const QString name,const QString path);
private:
    QSet<QString> _set_path;

    //右键单击某个item之后弹出菜单
    QTreeWidgetItem* _right_btn_item;
    QAction* _action_import;//导入项目的动作
    QAction* _action_setstart;//开始动作
    QAction* _action_closepro;//关闭项目的动作
    QAction* _action_slideshow;//轮流播放的动作
    QProgressDialog* _dialog_progress;//进程对话框，来展示当前的加载进度
    std::shared_ptr<ProTreeThread> _thread_create_pro;//智能指针来管理线程的生命周期
    QTreeWidgetItem* _active_item;//保存哪一个项目被设置为启动项目
    QTreeWidgetItem* _selected_item;//获取当前选中的文件
    std::shared_ptr<OpenTreeThread> _thread_open_pro;
    QProgressDialog* _open_progressdlg;
    std::shared_ptr<SlideShowDlg> _slide_show_dlg;
    QMediaPlayer* _player;//音乐播放器的类
    // QMediaPlaylist* _playlist;
private slots:
    void SlotItemPressed(QTreeWidgetItem* item,int column);
    void SlotImport();
    void SlotUpdateProgress(int count);
    void SlotFinishProgress();
    void SlotCancelProgress();
    void SlotSetActive();
    void SlotClosePro();

    void SlotUpOpenProgress(int count);
    void SlotFinishOpenProgress();
    void SlotCanceOpenProgress();
    void SlotDoubltClickItem(QTreeWidgetItem* doubleitem,int col);
    void SlotSlideShow();
public slots:
    void SlotOpenPro(const QString& path);
    void SlotNextShow();
    void SlotPreShow();
    void SlotSetMusic();

    //SQLite存储创建的项目，以便于下次打开时，将创建的项目展示在目录树上
signals:
    void SigCancelProgress();
    void SigCancelOpenProgress();
    void SigUpdateSelected(const QString& path);
    void SigUpdatePic(const QString& path);
    void SigClearSelected();
};

#endif // PROTREEWIDGET_H
