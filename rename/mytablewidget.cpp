#include "mytablewidget.h"

MyTableWidget::MyTableWidget(QWidget * parent)
    : QTableWidget(parent)
{
    myheadview = new MyHeaderView(Qt::Orientation::Horizontal, this);
    setHorizontalHeader(myheadview);        //upcasting向上造型
}

MyTableWidget::~MyTableWidget()
{
    delete myheadview;
}

void MyTableWidget::SetHorizontalHeaderItemWidget(int column, QWidget *widget)
{
    myheadview->SetItemWidget(column, widget);
}

void MyTableWidget::SetHorizontalHeaderItemMargins(int column, MyHeaderView::Margins margins)
{
    myheadview->SetItemMargins(column, margins);
}
