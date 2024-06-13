//默认界面，打开软件后出现的该页面
#ifndef OPENWIDGET_H
#define OPENWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QGroupBox>
#include <QLabel>
#include <QDragEnterEvent>
#include <QDropEvent>


class OpenWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OpenWidget(QWidget *parent = nullptr);

signals:
    void switchToOptwidget(QString);

private:
    QPushButton *open_dir_btn;
//    QPushButton *open_file_btn;
    QGroupBox *group;
    QGroupBox *group2;
    QLabel *pic_label;  //图片标签
    QLabel *label1;
    QLabel *label2;
    QLabel *label3;

protected:
    //重写父类QWidget的拖拽事件
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;

private slots:
    void onclicked_open_btn(); //打开文件夹的槽函数，信号switchToOptwidget发射出去
    void onclicked_open_file_btn(); //打开文件的槽函数
};

#endif // OPENWIDGET_H
