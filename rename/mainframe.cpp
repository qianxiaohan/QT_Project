#include "mainframe.h"
#include <QGridLayout>
#include <QDebug>
#include <QStatusBar>
#include <QFile>

MainFrame::MainFrame(QWidget *parent) : QMainWindow(parent)
{
    setFixedSize(900, 600);
    setWindowTitle("批量修改文件名");
    QIcon icon(":/image/window_icon.png");    //使用相对路径
    setWindowIcon(icon);
    setIconSize(QSize(30, 30));  //设置图标大小

    openwidget = new OpenWidget(this);

    optwidget = new OptWidget(this);

    optwidget->hide();
    statusBar()->showMessage("Ready");  //显示状态栏

    connect(openwidget, &OpenWidget::switchToOptwidget, this, &MainFrame::switchToOptwidget);
    connect(optwidget, &OptWidget::switchToOpenwidget, this, &MainFrame::switchToOpenwidget);
    connect(optwidget, &OptWidget::update_table_signal, this, &MainFrame::update_statusbar);
}

MainFrame::~MainFrame()
{
//    delete openwidget;
//    delete optwidget;
    //    delete group;
}

void MainFrame::loadStyleSheet(const QString &filename)
{
    QFile file(filename);
    if(file.open(QFile::ReadOnly))
    {
        QString styleSheet(file.readAll());
        this->setStyleSheet(styleSheet);    //设置qss
        file.close();
    }
}

void MainFrame::onclicked_open_dir_btn()
{
    //MainWindow w;
    //this->hide();
    optwidget->show();
    openwidget->hide();
}

void MainFrame::onclicked_return_btn()
{

}

//实现两个界面来回切换
void MainFrame::switchToOptwidget(QString dir_path)
{
    optwidget->updateTable(dir_path);
    optwidget->show();
    openwidget->hide();
}

void MainFrame::switchToOpenwidget()
{
    optwidget->hide();
    openwidget->show();
    statusBar()->showMessage("Ready");
}

void MainFrame::update_statusbar(int row_cnt)
{
    QString str = "共有" + QString::number(row_cnt) + "个文件";
    statusBar()->showMessage(str);  //设置状态栏
}
