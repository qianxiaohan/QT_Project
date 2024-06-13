#include "optwidget.h"
#include "mainframe.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
    MainFrame MF;
    MF.show();
//    MF.loadStyleSheet("F:/QT/QT Project/rename/style/myqss.css");
    MF.loadStyleSheet(":/style/myqss.css");
    return a.exec();
}
