#ifndef WIDGET_H
#define WIDGET_H

#include <QCloseEvent>
#include <QFile>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QString detectFileEncoding(const QString &fileName);
    void loadFile(const QString &fileName, const QString &encoding);
    void saveFile(const QString &fileName, const QString &encoding);
    void clearText();
    QString currentCodec;
    QString defaultAnsiEncoding;
    QString fileName;
    QByteArray buffer;
    QByteArray temp_buffer;
    bool isModify = false;

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void openBtn_clicked();
    void saveBtn_clicked();
    void closeBtn_clicked();

    void comboBox_indexChange(int index);
    void textEdit_cursorChange();
    void textEdit_textChange();

    void textEdit_factor(int factor);

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
