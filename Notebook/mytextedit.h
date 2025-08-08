#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H

#include <QTextEdit>

class MyTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    MyTextEdit(QWidget *parent = nullptr);
    void zoomInCustom();    //字体放大
    void zoomOutCustom();   //字体缩小

    int factor, min_factor, max_factor;

signals:
    void emit_factor(int factor); //将缩放因子传递给其他控件

protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // MYTEXTEDIT_H
