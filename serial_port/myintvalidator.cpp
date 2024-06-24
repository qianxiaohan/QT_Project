#include "myintvalidator.h"

MyIntValidator::MyIntValidator(QObject *parent)
    : QIntValidator(parent)
{
}

MyIntValidator::MyIntValidator(int bottom, int top, QObject *parent)
    : QIntValidator(bottom, top, parent)
{
}

void MyIntValidator::setRange(int bottom, int top)
{
    QIntValidator::setRange(bottom, top);
}

QValidator::State MyIntValidator::validate(QString &input, int &pos) const
{
    return QIntValidator::validate(input, pos); //调用基类的validate进一步验证
}

void MyIntValidator::fixup(QString &input) const
{
    input = QString("%1").arg(bottom());    //出现非法值时QLineEdit会自动调用fixup()
}






