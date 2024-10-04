#include "protreeitem.h"
#include"const.h"

ProTreeItem::ProTreeItem(QTreeWidget *view, const QString &name, const QString &path, int type)
    :QTreeWidgetItem(view,type),_name(name),_path(path),_root(this),_pre_item(nullptr),_next_item(nullptr)
{

}

ProTreeItem::ProTreeItem(QTreeWidgetItem *parent, const QString &name, const QString &path,
                         QTreeWidgetItem *root, int type)
    :QTreeWidgetItem(parent,type),_name(name),_path(path),_root(root),_pre_item(nullptr),_next_item(nullptr)
{

}

const QString &ProTreeItem::GetPath()
{
    return _path;
}

QTreeWidgetItem *ProTreeItem::GetRoot()
{
    return _root;
}

void ProTreeItem::SetPreItem(QTreeWidgetItem *item)
{
    _pre_item=item;
}

void ProTreeItem::SetNextItem(QTreeWidgetItem *item)
{
    _next_item=item;
}

ProTreeItem *ProTreeItem::GetPreItem()
{
    return dynamic_cast<ProTreeItem*>(_pre_item);
}

ProTreeItem *ProTreeItem::GetNextItem()
{
    return dynamic_cast<ProTreeItem*>(_next_item);
}

ProTreeItem *ProTreeItem::GetLastPicChild()
{
    if(this->type()==TreeItemPic)//如果是图片，没有子节点
    {
        return nullptr;
    }

    auto child_count=this->childCount();//取出所有的子节点
    if(child_count==0)//如果子节点数量=0，返回空
    {
        return nullptr;
    }

    for (int i = child_count-1; i >= 0; i--)
    {
        auto* last_child=this->child(i);//遍历每一个节点
        auto* last_tree_item=dynamic_cast<ProTreeItem*>(last_child);
        int item_type=last_tree_item->type();
        if(item_type==TreeItemPic)//如果最后一个节点是图片，就返回
        {
            return last_tree_item;
        }

        last_child=last_tree_item->GetLastPicChild();//如果最后一个节点是文件夹，就递归调用

        if(!last_child)
        {
            continue;//如果文件夹为空，跳过本次循环，继续找
        }

        last_tree_item=dynamic_cast<ProTreeItem*>(last_child);//如果找到了，返回
        return last_tree_item;
    }

    return nullptr;//遍历所有文件夹都没有,返回空指针
}

ProTreeItem *ProTreeItem::GetFirstPicChild()
{
    if(this->type()==TreeItemPic)//如果是图片，没有子节点
    {
        return nullptr;
    }

    auto child_count=this->childCount();//取出所有的子节点
    if(child_count==0)//如果子节点数量=0，返回空
    {
        return nullptr;
    }

    for (int i = 0; i < child_count-1; i++)
    {
        auto* first_child=this->child(i);//遍历每一个节点
        auto* first_tree_item=dynamic_cast<ProTreeItem*>(first_child);
        int item_type=first_tree_item->type();
        if(item_type==TreeItemPic)//如果最后一个节点是图片，就返回
        {
            return first_tree_item;
        }

        first_child=first_tree_item->GetLastPicChild();//如果最后一个节点是文件夹，就递归调用

        if(!first_child)
        {
            continue;//如果文件夹为空，跳过本次循环，继续找
        }

        first_tree_item=dynamic_cast<ProTreeItem*>(first_child);//如果找到了，返回
        return first_tree_item;
    }

    return nullptr;//遍历所有文件夹都没有,返回空指针
}
