#ifndef CONST_H
#define CONST_H

enum TreeItemType{
    TreeItemPro=1,//表示项目的条目
    TreeItemDir,//表示项目的文件夹
    TreeItemPic//表示项目的图片
};

const int PROGTESS_WIDTH=300;
const int PROGRESS_MAX=300;

enum PicBtnState{//播放图标的三种状态
    PicBtnSoateNormal=1,
    PicBtnSoateHover=2,
    PicBtnSoatePress=3,
    PicBtnSoate2Normal=4,
    PicBtnSoate2Hover=5,
    PicBtnSoate2Press=6
};

const int PREITEM_SIZE=100;//预览大小
const int PREICON_SIZE=90;

#endif // CONST_H
