#include "mainwindow.h"

#include <QApplication>
#include<QFile>
#include<QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile qss(":/style/style.qss");//加载qss文件
    qss.open(QIODevice::ReadOnly);
    if(qss.isOpen())
    {
        qDebug()<<("open qss success");
        QString style=QLatin1String(qss.readAll());//将qss中的字符串常量转化成QString
        a.setStyleSheet(style);//设置样式表
        qss.close();
    }else{//打开文件失败
        qDebug()<<("open qss filed");
        return 0;
    }
    MainWindow w;
    w.setWindowTitle("AIbum");//设置窗口名字
    w.showMaximized();//窗口最大化显示
    // w.show();
    return a.exec();
}
