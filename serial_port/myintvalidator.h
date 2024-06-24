#ifndef MYINTVALIDATOR_H
#define MYINTVALIDATOR_H
#include <QIntValidator>

class MyIntValidator : public QIntValidator
{
    Q_OBJECT
public:
    explicit MyIntValidator(QObject *parent = nullptr);

    MyIntValidator(int bottom, int top, QObject *parent);
    void setRange(int bottom, int top) override;
    virtual State validate(QString &input, int &pos) const override;    //pos：the cursor position光标位置
    virtual void fixup(QString &input) const override;
};

#endif // MYINTVALIDATOR_H
