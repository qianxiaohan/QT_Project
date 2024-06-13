#include "myheaderview.h"
#include <QDebug>

MyHeaderView::MyHeaderView(Qt::Orientation orientation, QWidget * parent)
    : QHeaderView(orientation, parent)
{

}

MyHeaderView::~MyHeaderView()
{

}

void MyHeaderView::SetItemWidget(int index, QWidget *widget)
{
    widget->setParent(this);
    mItems[index].item = widget;
}

void MyHeaderView::SetItemMargins(int index, Margins margins)
{
    mItems[index].margins = margins;
}

void MyHeaderView::SetHeaderAlignment(Qt::Alignment alignment)
{
    setDefaultAlignment(alignment); //设置文本的对齐方式
    int top = (this->height() - mItems[0].item->height())/2;
    int left = (this->sectionSize(0) - mItems[0].item->width())/2;
    mItems[0].margins = Margins(left, left, top, top);  //全选框始终居中
    //居中
       //获取组件的高度
    //用section的大小减去组件的大小
//    if(alignment == Qt::AlignCenter)    //居中显示
//    {
//        int top = (this->height() - mItems[0].item->height())/2;
//        int left = (this->sectionSize(0) - mItems[0].item->width())/2;
//        mItems[0].margins = Margins(left, left, top, top);
//    }
}

MyHeaderView::Item::Item() : item(nullptr)
{
}

MyHeaderView::Margins::Margins(int left, int right, int top, int buttom) :
    left(left),
    right(right),
    top(top),
    buttom(buttom)
{
}

void MyHeaderView::showEvent(QShowEvent *e)
{
   for (int i = 0; i < count(); i++)
   {
       if (!mItems[i].item)
           mItems[i].item = new QWidget(this);
       else
           mItems[i].item->setParent(this);

       //setGeometry(x, y, w, h)
       mItems[i].item->setGeometry(sectionViewportPosition(i) + mItems[i].margins.left + 1 ,
                                   mItems[i].margins.top,
                                   sectionSize(i) - mItems[i].margins.left - mItems[i].margins.right - 1,
                                   height() - mItems[i].margins.top - mItems[i].margins.buttom - 1);
       mItems[i].item->show();
   }

    QHeaderView::showEvent(e);
}
