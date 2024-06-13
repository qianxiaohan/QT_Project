#ifndef MYHEADERVIEW_H
#define MYHEADERVIEW_H

#include <QHeaderView>
#include <QShowEvent>
#include <Qt>

class MyHeaderView : public QHeaderView
{
    //Q_OBJECT：使用信号和槽函数必备的宏
public:
    struct Margins
    {
        int left;
        int right;
        int top;
        int buttom;

        Margins(int left = 2, int right = 2, int top = 2, int buttom = 2);  //通过构造函数设置默认参数
    };


    explicit MyHeaderView(Qt::Orientation orientation, QWidget *parent = nullptr);
    ~MyHeaderView();

    void SetItemWidget(int index, QWidget * widget);    //表头添加组件，比如checkbox、pushbutton
    void SetItemMargins(int index, Margins margins);    //设置边距
    void SetHeaderAlignment(Qt::Alignment alignment);  //设置表头组件的对齐方式

private:
    struct Item
    {
        QWidget * item;     //组件
        Margins margins;    //边缘

        Item(); //默认构造函数
    };

    QMap<int, Item> mItems;
    void showEvent(QShowEvent *e) override;
};

#endif // MYHEADERVIEW_H
