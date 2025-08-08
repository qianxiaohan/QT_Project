#include "mytextedit.h"

#include <QKeyEvent>

MyTextEdit::MyTextEdit(QWidget *parent) : QTextEdit(parent)
{
    // factor = 0: 缩放100%
    // factor = -9: 缩放10%
    // factor = 20: 缩放300%
    factor = 0;
    min_factor = -9;
    max_factor = 20;
}

void MyTextEdit::zoomInCustom()
{
    factor++;
    QFont f = font();
    f.setPointSize(f.pointSize() +  1);
    setFont(f);
}

void MyTextEdit::zoomOutCustom()
{
    factor--;
    QFont f = font();
    f.setPointSize(f.pointSize() -  1);
    setFont(f);
}


void MyTextEdit::keyPressEvent(QKeyEvent *event)
{
    // 按下CTRL+加号放大字体
    //modifiers：该属性保存事件发生前立即存在的键盘修饰键标志。
    if((event->modifiers() & Qt::ControlModifier) && (event->key() == Qt::Key_Plus)){
        if(factor < max_factor && factor > min_factor){
            zoomInCustom();
        }
        emit emit_factor(factor);
        event->accept();    //子类处理，就不用将事件传递给父类进行处理
    }// 按下CTRL+减号缩小字体
    else if((event->modifiers() & Qt::ControlModifier) && (event->key() == Qt::Key_Minus)){
        if(factor < max_factor && factor > min_factor){
            zoomOutCustom();
        }
        emit emit_factor(factor);
        event->accept();
    }// 按下CTRL+0恢复默认
    else if((event->modifiers() & Qt::ControlModifier) && (event->key() == Qt::Key_0)){
        QFont f = font();
        f.setPointSize(12);
        setFont(f);
        factor = 0;
        event->accept();
        emit emit_factor(factor);
    }else{
        QTextEdit::keyPressEvent(event); //将事件传递给父类进行处理
    }
}
