#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QResizeEvent>>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    QWidget*  _proTree;
    QWidget*  _picshow;
protected:
    void resizeEvent(QResizeEvent* event);
private slots:
    void SlotCreatePro(bool);
    void SlotOpenPro(bool);
signals:
    void SigOpenPro(const QString& path);
};
#endif // MAINWINDOW_H
