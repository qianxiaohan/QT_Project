//整个主框架
#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <QMainWindow>
#include <QPushButton>
#include <QGroupBox>
#include "openwidget.h" //子界面
#include "optwidget.h"

class MainFrame : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainFrame(QWidget *parent = nullptr);
    ~MainFrame();
    void loadStyleSheet(const QString &filename);   //根据路径filname添加qss文件

signals:


private:
    QGroupBox *group;
    OpenWidget *openwidget;
    OptWidget *optwidget;

private slots:
    void onclicked_open_dir_btn();
    void onclicked_return_btn();
    void switchToOptwidget(QString dir_path);   //接收文件夹路径
    void switchToOpenwidget();
    void update_statusbar(int row_cnt);    //更新状态栏

};

#endif // MAINFRAME_H
