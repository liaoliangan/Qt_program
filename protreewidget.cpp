#include "protreewidget.h"
#include<QDir>
#include"const.h"
#include"protreeitem.h"
#include<QGuiApplication>
#include<QMenu>
#include<QFileDialog>
#include"removeprodialog.h"
#include"slideshowdlg.h"
ProTreeWidget::ProTreeWidget(QWidget *parent): QTreeWidget(parent)
    ,_right_btn_item(nullptr),_dialog_progress(nullptr),
    _active_item(nullptr),_selected_item(nullptr),
    _thread_create_pro(nullptr),_thread_open_pro(nullptr),
    _open_progressdlg(nullptr)
{
    //this->header()->hide();隐藏表头，但是不知道为什么没有hide()函数
    this->setHeaderHidden(true);//隐藏表头,qt6.6.3版本用这个

    connect(this,&ProTreeWidget::itemPressed,this,&ProTreeWidget::SlotItemPressed);//按下一个按钮触发itemPressed这个信号

    _action_import=new QAction(QIcon(":/icon/import.png"),tr("导入文件"),this);
    _action_setstart=new QAction(QIcon(":/icon/core.png"),tr("设置活动项目"),this);
    _action_closepro=new QAction(QIcon(":/icon/close.png"),tr("关闭项目"),this);
    _action_slideshow=new QAction(QIcon(":/icon/slideshow.png"),tr("轮播图播放"),this);

    connect(_action_import,&QAction::triggered,this,&ProTreeWidget::SlotImport);//连接按下的信号和导入的槽函数

    connect(_action_setstart,&QAction::triggered,this,&ProTreeWidget::SlotSetActive);//设置活动项目

    connect(_action_closepro,&QAction::triggered,this,&ProTreeWidget::SlotClosePro);

    connect(this,&ProTreeWidget::itemDoubleClicked,this,&ProTreeWidget::SlotDoubltClickItem);//itemDoubleClicked双击发出这个信号

    connect(_action_slideshow,&QAction::triggered,this,&ProTreeWidget::SlotSlideShow);
}


void ProTreeWidget::AddProToTree(const QString name, const QString path)
{
    QDir dir(path);
    QString file_path=dir.absoluteFilePath(name);//获取文件夹的绝对路径
    if(_set_path.find(path)!=_set_path.end())//找到重名的文件夹
        return;//退出
    QDir pro_dir(file_path);
    if(!pro_dir.exists())
    {
        bool enable=pro_dir.mkpath(file_path);//创建文件夹
        if(!enable)
        {
            return;
        }
    }

    _set_path.insert(file_path);
    auto* item=new ProTreeItem(this,name,file_path,TreeItemPro);
    item->setData(0,Qt::DisplayRole,name);//设置角色
    item->setData(0,Qt::DecorationRole,QIcon(":/icon/dir.png"));//设置图标
    item->setData(0,Qt::ToolTipRole,file_path);//提示：鼠标悬浮时，提示文件路径
    this->addTopLevelItem(item);
}

void ProTreeWidget::SlotItemPressed(QTreeWidgetItem *pressedItem, int column)
{
    if(QGuiApplication::mouseButtons()==Qt::RightButton)//判断按钮的类型 如果是右键
    {
        QMenu menu(this);
        int itemtype = pressedItem->type();//返回枚举类型,强转为int
        if(itemtype==TreeItemPro)//自定义的枚举常量,"const.h"
        {
            _right_btn_item=pressedItem;//缓存一下
            qDebug()<<"123987"<<dynamic_cast<ProTreeItem*>(_right_btn_item)->GetPath();
            //加入菜单
            menu.addAction(_action_import);
            menu.addAction(_action_setstart);
            menu.addAction(_action_closepro);
            menu.addAction(_action_slideshow);
            menu.exec(QCursor::pos());//在鼠标的位置运行
        }
    }
}

void ProTreeWidget::SlotImport()
{
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::Directory);//设置选择文件的模式->文件夹
    file_dialog.setWindowTitle(tr("选择导入的文件夹"));
    QString path="";
    if(!_right_btn_item)
    {
        qDebug()<<"_right_btn_item is empty";
        return;
    }

    path = dynamic_cast<ProTreeItem*>(_right_btn_item)->GetPath();
    //获取路径给文件对话框设置一个默认的路径
    file_dialog.setDirectory(path);
    file_dialog.setViewMode(QFileDialog::Detail);//设置展示形式

    QStringList fileNames;
    if(file_dialog.exec())
    {
        fileNames=file_dialog.selectedFiles();//获取选择的文件
    }

    if(fileNames.length()<=0)
    {//如果什么都没有选，退出
        return;
    }

    QString import_path=fileNames.at(0);//获取第一个名字
    int file_count=0;
    _dialog_progress=new QProgressDialog(this);

    _thread_create_pro=
        std::make_shared<ProTreeThread>
        (std::ref(import_path),std::ref(path),//参数传递时用的是右值，但构造函数写的是引用，所以需要用std::ref()来将右值转为引用
        _right_btn_item,
        file_count,this,_right_btn_item,nullptr);//智能指针

    connect(_thread_create_pro.get(),&ProTreeThread::SigUpdateProgress,
            this,&ProTreeWidget::SlotUpdateProgress);//把线程与对话框更新连接

    connect(_thread_create_pro.get(),&ProTreeThread::SigFinishProgress,
            this,&ProTreeWidget::SlotFinishProgress);//把线程与对话框连接


    connect(_dialog_progress,&QProgressDialog::canceled,this,&ProTreeWidget::SlotCancelProgress);//发送信号，通知进程


    connect(this,&ProTreeWidget::SigCancelProgress,
            _thread_create_pro.get(),&ProTreeThread::SlotCanceProgress);//把线程与对话框连接

    _thread_create_pro->start();//启动进程

    _dialog_progress->setWindowTitle("Please wait...");
    _dialog_progress->setFixedWidth(PROGTESS_WIDTH);//设置对话框的固定大小
    _dialog_progress->setRange(0,PROGTESS_WIDTH);//设置波动范围
    _dialog_progress->exec();//模态对话框显示
}

void ProTreeWidget::SlotUpdateProgress(int count)
{
    if(!_dialog_progress)//判断对话框是否为空
    {
        return;
    }

    if(count>=PROGRESS_MAX)
    {
        _dialog_progress->setValue(count%PROGRESS_MAX);//将数量与对话框最大数量取余,得到百分比
    }else{
        _dialog_progress->setValue(count);
    }
}

void ProTreeWidget::SlotFinishProgress()
{
    _dialog_progress->setValue(PROGRESS_MAX);
    _dialog_progress->deleteLater();//每次导入都会创建对话框，所以用完直接删除
}

void ProTreeWidget::SlotCancelProgress()
{
    emit SigCancelProgress();//发送取消信号
    delete _dialog_progress;//删除进度对话框
    _dialog_progress=nullptr;//置空
}

void ProTreeWidget::SlotSetActive()
{
    if(!_right_btn_item)
    {
        return;
    }
    QFont nullFont;//定义字体
    nullFont.setBold(false);//是否加粗

    if(_active_item)//如果之前设置过启动项目
    {
        _active_item->setFont(0,nullFont);
    }

    _active_item =_right_btn_item;
    nullFont.setBold(true);
    _active_item->setFont(0,nullFont);
}

void ProTreeWidget::SlotClosePro()
{
    //添加删除的对话框RemoveProDialog
    RemoveProDialog remove_pro_dialog;
    auto res=remove_pro_dialog.exec();//判断执行的结果
    if(res!=QDialog::Accepted)//不接受
    {
        return;//退出
    }

    bool b_remove=remove_pro_dialog.IsRemoved();
    auto index_right_btn=this->indexOfTopLevelItem(_right_btn_item);//获取关闭项目的索引
    ProTreeItem* protreeitem=dynamic_cast<ProTreeItem*>(_right_btn_item);
    ProTreeItem* selecteditem=dynamic_cast<ProTreeItem*>(_selected_item);

    auto delete_path=protreeitem->GetPath();//获取路径
    _set_path.remove(delete_path);//从set容器中删除
    if(b_remove)
    {
        QDir delete_dir(delete_path);
        delete_dir.removeRecursively();//递归删掉目录
    }

    if(protreeitem==_active_item)//如果删除的目录与当前激活的目录是同一个
    {
        _active_item=nullptr;//将激活的条目设置为空，防止野指针
    }

    if(selecteditem && protreeitem==selecteditem->GetRoot())
    {
        selecteditem=nullptr;
        _selected_item=nullptr;
        emit SigClearSelected();
    }

    delete this->takeTopLevelItem(index_right_btn);//从目录树中移出
    _right_btn_item=nullptr;//把右击的条目设为空

}

void ProTreeWidget::SlotUpOpenProgress(int count)
{
    if(!_open_progressdlg)//判断对话框是否为空
    {
        return;
    }

    if(count>=PROGRESS_MAX)
    {
        _open_progressdlg->setValue(count%PROGRESS_MAX);//将数量与对话框最大数量取余,得到百分比
    }else{
        _open_progressdlg->setValue(count);
    }
}

void ProTreeWidget::SlotFinishOpenProgress()
{
    if(!_open_progressdlg)//判断对话框是否为空
    {
        return;
    }
    _open_progressdlg->setValue(PROGRESS_MAX);
    delete _open_progressdlg;
    _open_progressdlg=nullptr;
}

void ProTreeWidget::SlotCanceOpenProgress()
{
    emit SigCancelOpenProgress();//发送取消信号
    delete _open_progressdlg;//删除进度对话框
    _open_progressdlg=nullptr;//置空
}

void ProTreeWidget::SlotDoubltClickItem(QTreeWidgetItem *doubleitem, int col)
{
    if(QGuiApplication::mouseButtons()==Qt::LeftButton)//判断按键的类型
    {
        auto* tree_double=dynamic_cast<ProTreeItem*>(doubleitem);
        if(!tree_double)
        {
            return;
        }

        int itemtype=tree_double->type();
        if(itemtype==TreeItemPic)
        {
            emit SigUpdateSelected(tree_double->GetPath());//如果是图片，发送信号，将图片的路径发送出去
            _selected_item=doubleitem;
        }
    }
}

void ProTreeWidget::SlotSlideShow()
{
    if(!_right_btn_item)
    {
        return;
    }

    auto* right_pro_item=dynamic_cast<ProTreeItem*>(_right_btn_item);
    auto* last_child_item=right_pro_item->GetLastPicChild();//获取最后一个节点
    if(!last_child_item)
    {
        return;
    }

    auto* first_child_item=right_pro_item->GetFirstPicChild();//获取第一个节点
    if(!first_child_item)
    {
        return;
    }

    _slide_show_dlg=std::make_shared<SlideShowDlg>(this,first_child_item,last_child_item);//创建对话框

    _slide_show_dlg->setModal(true);//设置成模态
    _slide_show_dlg->showMaximized();
}

void ProTreeWidget::SlotOpenPro(const QString &path)
{
    if(_set_path.find(path)!=_set_path.end())//如果存在，说明已经打开过
    {
        return;//关闭
    }

    _set_path.insert(path);
    int file_count=0;
    QDir pro_dir(path);
    const QString proname=pro_dir.dirName();

    _thread_open_pro = std::make_shared<OpenTreeThread>(path,file_count,this,nullptr);
    _thread_open_pro->start();


    _open_progressdlg=new QProgressDialog(this);
    connect(_thread_open_pro.get(),&OpenTreeThread::SigUpdateProgress,
            this,&ProTreeWidget::SlotUpOpenProgress);//把线程与对话框更新连接

    connect(_thread_open_pro.get(),&OpenTreeThread::SigFinishProgress,
            this,&ProTreeWidget::SlotFinishOpenProgress);//把线程与对话框连接

    connect(_open_progressdlg,&QProgressDialog::canceled,this,&ProTreeWidget::SlotCancelProgress);//发送信号，通知进程
    connect(this,&ProTreeWidget::SigCancelOpenProgress,
            _thread_open_pro.get(),&OpenTreeThread::SlotCancelProgress);//把线程与对话框连接

    _thread_open_pro->start();//启动进程

    _open_progressdlg->setWindowTitle("Please wait...");
    _open_progressdlg->setFixedWidth(PROGTESS_WIDTH);//设置对话框的固定大小
    _open_progressdlg->setRange(0,PROGTESS_WIDTH);//设置波动范围
    _open_progressdlg->exec();//模态对话框显示
}

void ProTreeWidget::SlotNextShow()
{
    if(!_selected_item)
        return;
    auto* curItem=dynamic_cast<ProTreeItem*>(_selected_item)->GetNextItem();
    if(!curItem)
        return;


    //点击完更新
    emit SigUpdatePic(curItem->GetPath());
    _selected_item=curItem;
    this->setCurrentItem(curItem);//设置当前哪一个item是被激活的状态
}

void ProTreeWidget::SlotPreShow()
{
    if(!_selected_item)
        return;
    auto* curItem=dynamic_cast<ProTreeItem*>(_selected_item)->GetPreItem();
    if(!curItem)
        return;


    //点击完更新
    emit SigUpdatePic(curItem->GetPath());
    _selected_item=curItem;
    this->setCurrentItem(curItem);//设置当前哪一个item是被激活的状态
}

void ProTreeWidget::SlotSetMusic()
{
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::ExistingFiles);
    file_dialog.setWindowTitle(tr("选择导入文件夹"));
    file_dialog.setDirectory(QDir::currentPath());
    file_dialog.setViewMode(QFileDialog::Detail);
    file_dialog.setNameFilter("(*.mp3)");
    QStringList fileNames;
    if(file_dialog.exec())
    {
        fileNames=file_dialog.selectedFiles();
    }else
        return;

    if(fileNames.length()<=0)
        return;

}
