#ifndef PROTREETHREAD_H
#define PROTREETHREAD_H

#include <QThread>
#include<QTreeWidget>
class ProTreeThread : public QThread
{
    //宏->使类可以支持信号和槽
    Q_OBJECT
public:
    ProTreeThread();

    //stc_path:文件拷贝前的源地址
    //dist_path:文件拷贝的目标地址
    //parent_item:文件的父节点
    //self:整个TreeWidget
    ProTreeThread(const QString& src_path,const QString& dist_path,QTreeWidgetItem* parent_item,
                  int file_count,QTreeWidget* self,QTreeWidgetItem* root,QObject* parent=nullptr);
    ~ProTreeThread();

protected:
    //实现线程要重写run()函数
    void run();

private:
    //run()中调用的方法
    void CreateProTree(const QString& src_path,const QString& dist_path,QTreeWidgetItem* parent_item,
                       int& file_count,QTreeWidget* slef,QTreeWidgetItem* root,QTreeWidgetItem* preItem=nullptr);


    QString _src_path;
    QString _dist_path;
    int _file_count;
    QTreeWidgetItem* _parent_item;
    QTreeWidget* _self;
    QTreeWidgetItem* _root;
    bool _bstop;//控制线程退出的变量
public slots:
    void SlotCanceProgress();
signals:
    void SigUpdateProgress(int);//更新进度的信号
    void SigFinishProgress(int);//完成的信号
};

#endif // PROTREETHREAD_H
