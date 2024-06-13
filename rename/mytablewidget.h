#ifndef MYTABLEWIDGET_H
#define MYTABLEWIDGET_H
#include <QTableWidget>
#include "myheaderview.h"
#include <QMap>

class MyTableWidget : public QTableWidget
{
    //Q_OBJECT

public:
    MyTableWidget(QWidget * parent = nullptr);
    ~MyTableWidget();

    void SetHorizontalHeaderItemWidget(int column, QWidget * widget);
    void SetHorizontalHeaderItemMargins(int column, MyHeaderView::Margins margins);
    MyHeaderView* getHeaderView() { return myheadview;}
//    QMap<int, QString> filemap;     //文件名的键值对，行号 ：文件路径

private:
    MyHeaderView *myheadview;

};

#endif // MYTABLEWIDGET_H
