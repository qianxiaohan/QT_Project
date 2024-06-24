#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serialportwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void loadStyleSheet(const QString &filename);

private:
    SerialPortWidget *serialportwidget;
    QLabel *statusBarLabel1;
    QLabel *statusBarLabel2;
    QLabel *statusBarLabel3;
    QLabel *statusBarLabel4;


private slots:
    void updateTime();
    void slot_showPortName(const QString &info);
    void slotUpdateReceiveBytes(const QString &bytes);
    void slotUpdateSendBytes(const QString &bytes);

};
#endif // MAINWINDOW_H
