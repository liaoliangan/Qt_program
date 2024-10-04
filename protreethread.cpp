#include "protreethread.h"
#include<QDir>
#include"ProTreeItem.h"
#include"const.h"
ProTreeThread::ProTreeThread() {}

ProTreeThread::ProTreeThread(const QString &src_path,
                             const QString &dist_path,
                             QTreeWidgetItem *parent_item,
                             int file_count,
                             QTreeWidget *self,
                             QTreeWidgetItem *root,
                             QObject *parent)
    :QThread(parent),_src_path(src_path),_dist_path(dist_path),
    _file_count(file_count),_parent_item(parent_item),_self(self),
    _root(root),_bstop(false)
{
}

ProTreeThread::~ProTreeThread()
{

}

void ProTreeThread::run()
{
    CreateProTree(_src_path,_dist_path,_parent_item,_file_count,_self,_root);//创建树
    if(_bstop)
    {
        auto path = dynamic_cast<ProTreeItem*>(_root)->GetPath();//获取路径
        auto index=_self->indexOfTopLevelItem(_root);//获取根节点所在的索引
        delete _self->takeTopLevelItem(index);
        QDir dir(path);//获取复制的目的路径
        dir.removeRecursively();//将复制完的文件删除掉
        return;
    }

    emit SigFinishProgress(_file_count);
}

void ProTreeThread::CreateProTree(const QString &src_path, const QString &dist_path,
                                  QTreeWidgetItem *parent_item, int &file_count, QTreeWidget *self,
                                  QTreeWidgetItem *root, QTreeWidgetItem *preItem)
{
    if(_bstop==true)
    {
        return;//_bstop为true表示需要退出
    }
    bool needcopy=true;//是否需要拷贝
    if(src_path==dist_path)
    {
        needcopy=false;
    }

    QDir import_dir(src_path);
    //过滤器
    import_dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);//过滤后所需要的
    import_dir.setSorting(QDir::Name);//根据名字排序
    QFileInfoList list=import_dir.entryInfoList();//返回原路径过滤后的有效的文件夹
    for(int i=0;i<list.size();i++)
    {
        if(_bstop==true)
        {
            return;
        }

        QFileInfo fileInfo=list.at(i);
        bool bIsDir=fileInfo.isDir();//判断是否是文件夹
        if(bIsDir)
        {
            if(_bstop)
            {
                return;
            }

            file_count++;
            emit SigUpdateProgress(file_count);

            //目的路径构造文件夹
            QDir dist_dir(_dist_path);
            QString sub_dist_path=dist_dir.absoluteFilePath(fileInfo.fileName());
            QDir sub_dist_dir(sub_dist_path);
            if(!sub_dist_dir.exists())//是否存在
            {
                bool ok=sub_dist_dir.mkpath(sub_dist_path);//创建路径
                if(!ok)
                {
                    continue;
                }
            }

            auto* item=new ProTreeItem(parent_item,fileInfo.fileName(),sub_dist_path,
                                         root,TreeItemDir);
            item->setData(0,Qt::DisplayRole,fileInfo.fileName());
            item->setData(0,Qt::DecorationRole,QIcon(":/icon/dir.png"));
            item->setData(0,Qt::ToolTipRole,sub_dist_path);

            CreateProTree(fileInfo.absoluteFilePath(),sub_dist_path,
                          item,file_count,self,root,preItem);
        }else{
            if(_bstop)
            {
                return;
            }
            const QString& suffix=fileInfo.completeSuffix();//去除完整的后缀，判断是否是图片文件
            if(suffix!="png"&&suffix!="jpeg"&&suffix!="jpg")
            {
                continue;
            }
            file_count++;


            //发送信号通知进度对话框
            emit SigUpdateProgress(file_count);
            if(!needcopy)
            {//判断是否需要拷贝
                continue;
            }

            QDir dist_dir(dist_path);
            QString dist_file_path=dist_dir.absoluteFilePath(fileInfo.fileName());//将文件名字和文件夹路径拼接
            if(!QFile::copy(fileInfo.absoluteFilePath(),dist_file_path))
            {
                continue;
            }

            auto* item=new ProTreeItem(parent_item,fileInfo.fileName(),dist_file_path,root,TreeItemPic);

            item->setData(0,Qt::DisplayRole,fileInfo.fileName());
            item->setData(0,Qt::DecorationRole,QIcon(":/icon/pic.png"));
            item->setData(0,Qt::ToolTipRole,dist_file_path);

            //如果前一个节点不为空，通过链表连接起来
            if(preItem)
            {
                auto* pre_preitem=dynamic_cast<ProTreeItem*>(preItem);
                pre_preitem->SetNextItem(item);
            }
            item->SetPreItem(preItem);//设置前一个节点
            preItem=item;
        }
    }
}

void ProTreeThread::SlotCanceProgress()
{
    this->_bstop=true;
}
