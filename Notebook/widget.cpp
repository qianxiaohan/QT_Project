#include "widget.h"
#include "ui_widget.h"

#include <QFileDialog>
#include <QPushButton>
#include <QTextStream>
#include <QComboBox>
#include <QDebug>
#include <QTextCodec>
#include <QList>
#include <QShortcut>
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //连接槽函数
    connect(ui->openBtn, SIGNAL(clicked()), this, SLOT(openBtn_clicked()));
    connect(ui->saveBtn, SIGNAL(clicked()), this, SLOT(saveBtn_clicked()));
    connect(ui->closeBtn, SIGNAL(clicked()), this, SLOT(closeBtn_clicked()));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBox_indexChange(int)));
    connect(ui->textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(textEdit_cursorChange()));
    connect(ui->textEdit, &QTextEdit::textChanged, this, &Widget::textEdit_textChange);
    connect(ui->textEdit, &MyTextEdit::emit_factor, this, &Widget::textEdit_factor);

    QShortcut *shortcutOpen = new QShortcut(QKeySequence(tr("Ctrl+O", "File|Open")), this);
    QShortcut *shortcutSave = new QShortcut(QKeySequence(tr("Ctrl+S", "File|Save")), this);
    connect(shortcutOpen, &QShortcut::activated, this, &Widget::openBtn_clicked);
    connect(shortcutSave, &QShortcut::activated, this, &Widget::saveBtn_clicked);

    //高亮光标所在行
    QList<QTextEdit::ExtraSelection> extras;
    QTextEdit::ExtraSelection extra;
    extra.cursor = ui->textEdit->textCursor();
    extra.format.setBackground(QBrush(Qt::yellow));
    extra.format.setProperty(QTextFormat::FullWidthSelection, true);
    extras.append(extra);
    ui->textEdit->setExtraSelections(extras);

    //只显示纯文本
    ui->textEdit->setAcceptRichText(false);

    //设置标题
    this->setWindowTitle("无标题 - 记事本");
    this->setWindowIcon(QIcon("../Notebook/img/icon.png"));

    //将当前容器的布局进行设置，这样拖拽后，组件会自定变化大小
    this->setLayout(ui->verticalLayout);

    //初始化成员
    currentCodec = "UTF-8";
    defaultAnsiEncoding = "GBK";

    //设置焦点
    ui->textEdit->setFocus();
}

Widget::~Widget()
{
    delete ui;
}

QString Widget::detectFileEncoding(const QString &fileName)
{
    QFile qfile(fileName);
    if(!qfile.open(QIODevice::ReadOnly)){
        return "UTF-8";
    }

    QByteArray ba = qfile.read(4096);   //最多读取4KB
    QTextCodec::ConverterState state;
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    codec->toUnicode( ba.constData(), ba.size(), &state);
    qfile.close();
    if (state.invalidChars == 0)
    { 
        return QString("UTF-8");
    }
    return defaultAnsiEncoding;
}

void Widget::loadFile(const QString &fileName, const QString &encoding)
{
    QFile file;
    //读取文件操作
    file.setFileName(fileName);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)){
        return;
    }

    QString fileNameWithoutPath = fileName.split('/').back();
    qDebug() << fileNameWithoutPath;
    this->setWindowTitle(fileNameWithoutPath + " - 记事本");

    //一行一行读取
    //读取之前取消槽函数的连接
    disconnect(ui->textEdit, &QTextEdit::textChanged, this, &Widget::textEdit_textChange);
    disconnect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBox_indexChange(int)));

    if(currentCodec == "UTF-8"){
        ui->comboBox->setCurrentIndex(0);
    }
    else{
        ui->comboBox->setCurrentIndex(1);
    }

    QTextStream in(&file);
    in.setCodec(QTextCodec::codecForName(encoding.toUtf8()));
    ui->textEdit->clear();
    ui->textEdit->append(in.readAll());
    file.close();
    connect(ui->textEdit, &QTextEdit::textChanged, this, &Widget::textEdit_textChange);
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBox_indexChange(int)));
    ui->textEdit->setFocus();
}

void Widget::saveFile(const QString &fileName, const QString &encoding)
{
    QFile file;

    file.setFileName(fileName);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        return;
    }
    this->setWindowTitle(fileName.split('/').back() + " - 记事本");

    //写入文件
    QTextStream out(&file);
    //设置保存的编码格式
    out.setCodec(encoding.toUtf8());
    QString data = ui->textEdit->toPlainText();
    out << data;
    file.close();
}

void Widget::clearText()
{
    disconnect(ui->textEdit, &QTextEdit::textChanged, this, &Widget::textEdit_textChange);
    ui->textEdit->clear();
    connect(ui->textEdit, &QTextEdit::textChanged, this, &Widget::textEdit_textChange);
}

void Widget::closeEvent(QCloseEvent *event)
{

    if(isModify){
        int ret = QMessageBox::warning(this, tr("注意"),
                                       tr("文件已经修改\n"
                                          "是否保存?"),
                                       "保存",
                                       "不保存",
                                       "取消",
                                       0,
                                       2);

        switch (ret) {
        case 0:
          // 保存文件
            saveBtn_clicked();
            event->accept();
            break;
        case 1:
          // 不保存文件，恢复原状
            event->accept();
            break;
        case 2:
            // 取消关闭
            event->ignore();
            break;
        default:
          // should never be reached
          break;
        }
    }
    else{
        event->accept();
    }
}

void Widget::openBtn_clicked()
{
    QTextCursor tc = ui->textEdit->textCursor();

    fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"), "C:/", tr("Text Files (*.txt)"));

    if(fileName.isEmpty())
        return;

    //获得焦点
    ui->textEdit->setFocus();

    //恢复光标位置
    ui->textEdit->setTextCursor(tc);

    //判断文件编码格式
    currentCodec = detectFileEncoding(fileName);

    //读取文件
    loadFile(fileName, currentCodec);

}

void Widget::saveBtn_clicked()
{
    fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"), "C:/", tr("Text Files (*.txt)"));

    if(fileName.isEmpty())
        return;

    //保存文件
    saveFile(fileName, currentCodec);
}

void Widget::closeBtn_clicked()
{
    if(isModify){
        int ret = QMessageBox::warning(this, tr("注意"),
                                       tr("文件已经修改\n"
                                          "是否保存?"),
                                       "保存",
                                       "不保存",
                                       "关闭文件",
                                       0,
                                       2);

        switch (ret) {
        case 0:
          // 保存文件
            saveBtn_clicked();
            //保存后去掉标题栏的'*'
            if(this->windowTitle().startsWith('*'))
                this->setWindowTitle(this->windowTitle().remove(0, 1));
            break;
        case 1:
          // 不保存文件，恢复原状
            loadFile(fileName, currentCodec);
            //去除标题栏的'*'
            if(this->windowTitle().startsWith('*'))
                this->setWindowTitle(this->windowTitle().remove(0,1));
            break;
        case 2:
            // 关闭文件
            clearText();
            this->setWindowTitle("无标题 - 记事本");
            fileName = "";
            break;
        default:
          // should never be reached
          break;
        }
        // 恢复未修改状态
        isModify = false;
    }
    else{
        clearText();
        this->setWindowTitle("无标题 - 记事本");
        fileName = "";
    }

    //文本编辑框获得焦点
    ui->textEdit->setFocus();
}

void Widget::comboBox_indexChange(int index)
{
    qDebug() << index;
    currentCodec = (index == 0)? "UTF-8" : "GBK";
    loadFile(fileName, currentCodec);
}

void Widget::textEdit_cursorChange()
{
    QTextCursor tc = ui->textEdit->textCursor();
    //获取光标所在行和列
    int col = tc.columnNumber();
    int row = tc.blockNumber();
    QString str("第" + QString::number(row+1) + "行，" + "第" + QString::number(col+1) + "列");
    ui->label_2->setText(str);

    //高亮光标所在行
    QList<QTextEdit::ExtraSelection> extras;
    QTextEdit::ExtraSelection extra;
    extra.cursor = tc;
    extra.format.setBackground(QBrush(Qt::yellow));
    extra.format.setProperty(QTextFormat::FullWidthSelection, true);
    extras.append(extra);
    ui->textEdit->setExtraSelections(extras);
}

void Widget::textEdit_textChange()
{
    QString title = this->windowTitle();
    if(!title.startsWith("*")){
        this->setWindowTitle("*" + title);
    }
    qDebug() << isModify;
    if(!isModify) isModify = true;
}

void Widget::textEdit_factor(int factor)
{
    int x = 100 + factor * 10;
    ui->label_3->setText(QString::number(x) + "%");
}


