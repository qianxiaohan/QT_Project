#include "mainwindow.h"
#include <QFile>
#include <QStatusBar>
#include <QTime>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(800, 650);
    serialportwidget = new SerialPortWidget;
    setCentralWidget(serialportwidget);
    setWindowTitle("简易串口助手 By Lyx");
    QPixmap pixmap(":/image/windowicon.png");
    setWindowIcon(QIcon(pixmap));
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    timer->start(1000);
    statusBarLabel1 = new QLabel("未选择串口", this);
    statusBarLabel2 = new QLabel("接收：0", this);
    statusBarLabel3 = new QLabel("发送：0", this);
    statusBarLabel4 = new QLabel("", this);

    statusBarLabel1->setFixedWidth(200);
    statusBarLabel2->setFixedWidth(150);
    statusBarLabel3->setFixedWidth(150);

    statusBar()->addWidget(statusBarLabel1);
    statusBar()->addWidget(statusBarLabel2);
    statusBar()->addWidget(statusBarLabel3);

    statusBar()->addPermanentWidget(statusBarLabel4);

    connect(serialportwidget, &SerialPortWidget::showPortName, this, &MainWindow::slot_showPortName);
    connect(serialportwidget, &SerialPortWidget::signalUpdateReciveBytes, this, &MainWindow::slotUpdateReceiveBytes);
    connect(serialportwidget, &SerialPortWidget::signalUpdateSendBytes, this, &MainWindow::slotUpdateSendBytes);
}

MainWindow::~MainWindow()
{
    serialportwidget->saveSetting();    //保存设置文件.json
}

void MainWindow::loadStyleSheet(const QString &filename)
{
    QFile file(filename);
    if(file.open(QFile::ReadOnly))
    {
        QString styleSheet(file.readAll());
        this->setStyleSheet(styleSheet);    //设置qss
        file.close();
    }
}

void MainWindow::updateTime()
{
    QTime time = QTime::currentTime();
    QString ctime = time.toString("hh:mm:ss");
    statusBarLabel4->setText("当前时间：" + ctime);
}

void MainWindow::slot_showPortName(const QString &info)
{
    statusBarLabel1->setText(info);
}

void MainWindow::slotUpdateReceiveBytes(const QString &bytes)
{
    statusBarLabel2->setText("接收：" + bytes);
}

void MainWindow::slotUpdateSendBytes(const QString &bytes)
{
    statusBarLabel3->setText("发送：" + bytes);
}


