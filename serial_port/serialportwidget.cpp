#include "serialportwidget.h"

#include <QCoreApplication>
#include <QFile>
#include <QFileDialog>
#include <QFormLayout>
#include <QJsonArray>
#include <QMessageBox>
#include <QTime>
#include <QTimer>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QDir>
#include <QJsonDocument>
#include <QTextCodec>
#include <QRegExpValidator>
#include "myintvalidator.h"

SerialPortWidget::SerialPortWidget(QWidget *parent) : QWidget(parent)
{
    createRecGroupBox();
    createSendGroupBox();
    setupUI();
    loadSettingJSON();
}

void SerialPortWidget::setupUI()
{
    //主布局
    QVBoxLayout *mainlayout = new QVBoxLayout;
    mainlayout->addWidget(group1);
    mainlayout->addWidget(group2);

    setLayout(mainlayout);  //设置主布局

    serialport = new QSerialPort(this);

    //获取串口信息
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
        serialPortComboBox->addItem(info.portName());   //获取串口名

    //连接信号与槽
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SerialPortWidget::timeSend);

    connect(changeColorCheckBox, &QPushButton::clicked, this, &SerialPortWidget::onclicked_changeColorCheckBox);
    connect(clearRecButton, &QPushButton::clicked, this, &SerialPortWidget::onclicked_clearRecButton);
    connect(clearSendButton, &QPushButton::clicked, this, &SerialPortWidget::onclicked_clearSendButton);
    connect(saveRecButton, &QPushButton::clicked, this, &SerialPortWidget::onclicked_saveRecButton);
    connect(hexCheckBox, &QCheckBox::stateChanged, this, &SerialPortWidget::stateChanged_hexCheckBox);
    connect(sendButton, &QCheckBox::clicked, this, &SerialPortWidget::onclicked_sendButton);
    times = 0;
    connect(openSerialButton, &QCheckBox::clicked, this, &SerialPortWidget::onclicked_openSerialButton);
    connect(serialport, &QSerialPort::readyRead, this, &SerialPortWidget::readData);
    connect(timerCheckBox, &QCheckBox::stateChanged, this, &SerialPortWidget::stateChanged_timerCheckBox);
    connect(sendTextEdit, &QTextEdit::textChanged, this, &SerialPortWidget::validateHEX);
    connect(sendHEXCheckBox, &QCheckBox::stateChanged, this, &SerialPortWidget::stateChanged_sendHEXCheckBox);
}

void SerialPortWidget::createRecGroupBox()
{
    group1 = new QGroupBox("接收数据", this);
    serialPortLabel = new QLabel("串口选择");
    baudRateLabel = new QLabel("波特率");
    stopBitsLabel = new QLabel("停止位");
    dataBitsLabel = new QLabel("数据位");
    parityLabel = new QLabel("校验位");
    openSerialLabel = new QLabel("串口操作");
    saveRecButton = new QPushButton("保存接收");
    clearRecButton = new QPushButton("清除接收");
    serialPortComboBox = new QComboBox;
    baudRateComboBox = new QComboBox;
    stopBitsComboBox = new QComboBox;
    dataBitsComboBox = new QComboBox;
    parityComboBox = new QComboBox;
    QPixmap pixmap(":/image/openbtn.png");

    openSerialButton = new QPushButton(pixmap, "打开串口");

    hexCheckBox = new QCheckBox("16进制显示");
    changeColorCheckBox  = new QCheckBox("黑底绿字");
    timestampCheckBox = new QCheckBox("显示时间");
    receiveTextBrowser = new QTextBrowser;

    //控件大小设置
    saveRecButton->setFixedWidth(70);

    //控件设置
    baudRateComboBox->addItem(QStringLiteral("1200"), QSerialPort::Baud1200);
    baudRateComboBox->addItem(QStringLiteral("2400"), QSerialPort::Baud2400);
    baudRateComboBox->addItem(QStringLiteral("4800"), QSerialPort::Baud4800);
    baudRateComboBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    baudRateComboBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    baudRateComboBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    baudRateComboBox->addItem(QStringLiteral("57600"), QSerialPort::Baud57600);
    baudRateComboBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);

    stopBitsComboBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
    stopBitsComboBox->addItem(QStringLiteral("1.5"), QSerialPort::OneAndHalfStop);
    stopBitsComboBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    dataBitsComboBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    dataBitsComboBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    dataBitsComboBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    dataBitsComboBox->addItem(QStringLiteral("5"), QSerialPort::Data5);

    parityComboBox->addItem(QStringLiteral("None"), QSerialPort::NoParity);
    parityComboBox->addItem(QStringLiteral("Odd"), QSerialPort::OddParity);
    parityComboBox->addItem(QStringLiteral("Even"), QSerialPort::EvenParity);

    //接收数据区域布局设置
    QGridLayout *gridlayout = new QGridLayout;
    gridlayout->addWidget(serialPortLabel, 0, 0);
    gridlayout->addWidget(serialPortComboBox, 0, 1);
    gridlayout->addWidget(baudRateLabel, 1, 0);
    gridlayout->addWidget(baudRateComboBox, 1, 1);
    gridlayout->addWidget(stopBitsLabel, 2, 0);
    gridlayout->addWidget(stopBitsComboBox, 2, 1);
    gridlayout->addWidget(dataBitsLabel, 3, 0);
    gridlayout->addWidget(dataBitsComboBox, 3, 1);
    gridlayout->addWidget(parityLabel, 4, 0);
    gridlayout->addWidget(parityComboBox, 4, 1);
    gridlayout->addWidget(openSerialLabel, 5, 0);
    gridlayout->addWidget(openSerialButton, 5, 1);
    gridlayout->addWidget(saveRecButton, 6, 0);
    gridlayout->addWidget(clearRecButton, 6, 1);
    gridlayout->addWidget(hexCheckBox, 7, 0);
    gridlayout->addWidget(changeColorCheckBox, 7, 1);
//    gridlayout->addWidget(rtsCheckBox, 8, 0);
//    gridlayout->addWidget(dtrCheckBox, 8, 1);
    gridlayout->addWidget(timestampCheckBox, 8, 0);
    gridlayout->addWidget(receiveTextBrowser, 0, 2, 12, 1);

    group1->setLayout(gridlayout);
}

void SerialPortWidget::createSendGroupBox()
{
    //发送区控件
    group2 = new QGroupBox("发送数据", this);

//    QPushButton *buttons[buttonNum];
//    QLineEdit *lineEdits[buttonNum];
//    QCheckBox *checkBoxs[buttonNum];
    QGroupBox *textGroupBox;
    QGroupBox *sendGroupBox;
    timerCheckBox = new QCheckBox("定时发送");
    sendLineCheckBox = new QCheckBox("发送新行");
    sendHEXCheckBox = new QCheckBox("发送16进制");
    showSendTextCheckBox = new QCheckBox("显示发送数据");
    timeLineEdit = new QLineEdit;
    QLabel *timeLabel = new QLabel("ms/次");
    clearSendButton = new QPushButton("清空发送");
    clearSendButton->setFixedHeight(50);

    timeLineEdit->setFixedWidth(50);
    timeLineEdit->setText("1000");
    timeLineEdit->setValidator(new MyIntValidator(1000, 1000000, this));  //限定只能输入1~1000000
    timeLineEdit->setToolTip("输入1000-1000000的数字");
//    QRegExpValidator *regexpv = new QRegExpValidator();
//    regexpv->setRegExp(QRegExp("^(5[0-9]{2}|[6-9][0-9]{2}|[1-9][0-9]{3}|[1-9][0-9]{4}|[1-9][0-9]{5}|1000000)"));    //匹配500~1000000的数字
//    timeLineEdit->setValidator(regexpv);
    sendLineCheckBox->setChecked(true); //默认勾选

    sendTextEdit = new QTextEdit;
    sendTextEdit->setFont(QFont("微软雅黑", 9));
    sendTextEdit->setReadOnly(true);
    sendButton = new QPushButton("发送");
    sendButton->setFixedHeight(50);
//    clearSendButton = new QPushButton("清除发送");
    textGroupBox = new QGroupBox("发送多条数据");
    sendGroupBox = new QGroupBox("发送单条数据");



    QGridLayout *gridlayout1 = new QGridLayout;
    QGridLayout *gridlayout2 = new QGridLayout;
//    QLabel *hexLabel= new QLabel("HEX");
    QLabel *strLabel= new QLabel("字符串");
    QLabel *sendLabel = new QLabel("发送");
//    gridlayout1->addWidget(hexLabel, 0, 0, Qt::AlignHCenter | Qt::AlignVCenter);
    gridlayout1->addWidget(strLabel, 0, 0, Qt::AlignHCenter | Qt::AlignVCenter);
    gridlayout1->addWidget(sendLabel, 0, 1, Qt::AlignHCenter | Qt::AlignVCenter);

    for (int i = 0; i < SerialPortWidget::buttonNum; i++) {
//        checkBoxs[i] = new QCheckBox;
        lineEdits[i] = new QLineEdit;
        buttons[i] = new QPushButton(tr("%1").arg(i+1));
        buttons[i]->setFixedWidth(50);
        lineEdits[i]->setFont(QFont("微软雅黑", 9));
        connect(buttons[i], &QPushButton::clicked, [=]{
            if(serialport->isOpen())
            {
                QString str = lineEdits[i]->text();
                QByteArray data = str.toLocal8Bit();
                if(!str.isEmpty() && showSendTextCheckBox->isChecked())
                    showStringwithTime(str, timestampCheckBox->isChecked(), SerialPortWidget::send);
                writeData(data);    //写数据
            }
        });

//        gridlayout1->addWidget(checkBoxs[i], i + 1, 0);
        gridlayout1->addWidget(lineEdits[i], i + 1, 0);     //添加单行文本框
        gridlayout1->addWidget(buttons[i], i + 1, 1);    //添加按钮
    }
    textGroupBox->setLayout(gridlayout1);

    gridlayout2->addWidget(sendLineCheckBox,0, 3);
    gridlayout2->addWidget(sendHEXCheckBox, 0, 4);
    gridlayout2->addWidget(showSendTextCheckBox, 0, 5);

    gridlayout2->addWidget(clearSendButton, 1, 5);
    gridlayout2->addWidget(sendTextEdit, 1, 0, 2, 5);
    gridlayout2->addWidget(sendButton, 2, 5);
    gridlayout2->addWidget(timerCheckBox, 0, 0);
    gridlayout2->addWidget(timeLineEdit, 0, 1);
    gridlayout2->addWidget(timeLabel, 0, 2);

//    connect(showSendTextCheckBox, &QPushButton::clicked, this, );

    //添加一个 垂直的弹簧
    QSpacerItem *spacer = new QSpacerItem(0, 130, QSizePolicy::Expanding, QSizePolicy::Fixed);
    gridlayout2->addItem(spacer, 2, 0);

//    gridlayout2->addWidget(clearSendButton, 1, 5);
//    group2->setLayout(gridlayout2);
    sendGroupBox->setLayout(gridlayout2);

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(textGroupBox);
    hlayout->addWidget(sendGroupBox);
    group2->setLayout(hlayout);

}

void SerialPortWidget::createSettingJSON()
{

}

void SerialPortWidget::loadSettingJSON()
{
//    QFile file("../serial_port/data/mysetting.json");
    QString path = QCoreApplication::applicationDirPath() + "/setting.json";
    QFile file(path);

    //ReadWrite模式：当文件不存在时先创建文件
    if(!file.open(QIODevice::ReadWrite))
    {
        qDebug() << "load json error!\n";
        return;
    }

    QByteArray bytes = file.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(bytes));
    if(loadDoc.isNull())
    {
        qWarning() << "json文件格式错误";
        return;
    }
    QJsonObject rootObj = loadDoc.object();   //获取根对象
    QJsonValue jsonvalue = rootObj.value("settings");
    if(jsonvalue.type() == QJsonValue::Object)  //获取到对象
    {
        int index = baudRateComboBox->findData(jsonvalue["baudrate"].toDouble());
        baudRateComboBox->setCurrentIndex(index);

        index = stopBitsComboBox->findData(jsonvalue["stopBits"].toDouble());
        stopBitsComboBox->setCurrentIndex(index);

        index = dataBitsComboBox->findData(jsonvalue["dataBits"].toDouble());
        dataBitsComboBox->setCurrentIndex(index);

        index = parityComboBox->findData(jsonvalue["parity"].toDouble());
        parityComboBox->setCurrentIndex(index);

        bool ischecked = jsonvalue["hex"].toBool();
        hexCheckBox->setChecked(ischecked);

        ischecked = jsonvalue["changeBackgroudColor"].toBool();
        changeColorCheckBox->setChecked(ischecked);
        if(ischecked)
            receiveTextBrowser->setStyleSheet("color: rgb(0, 170, 0);"
                                              "background-color: rgb(0, 0, 0);"
                                              "font: 9pt \"微软雅黑\";");
        else
            receiveTextBrowser->setStyleSheet("color: rgb(0, 0, 0);"
                                              "background-color: rgb(255, 255, 255);"
                                              "font: 9pt \"微软雅黑\";");

        timestampCheckBox->setChecked(jsonvalue["showtime"].toBool());
        showSendTextCheckBox->setChecked(jsonvalue["showSendText"].toBool());
        sendLineCheckBox->setChecked(jsonvalue["appendNewLine"].toBool());
        sendHEXCheckBox->setChecked(jsonvalue["sendHEX"].toBool());

        QJsonValue jsonarray = jsonvalue["mutiText"];
        if(jsonarray.type() == QJsonValue::Array)   //如果是数组则继续执行
        {
            QJsonArray temparray = jsonarray.toArray();
            for (int i = 0;  i < SerialPortWidget::buttonNum; i++) {
                lineEdits[i]->setText(temparray.at(i).toString());
            }
        }

    }

    file.close();
}

void SerialPortWidget::readData()
{
    QByteArray data = serialport->readAll();
    if(data.endsWith("\r\n"))   //去掉"\r\n"
        data.chop(2);
    receiveAarry.append(data);  //字节数组添加收到的数据
    receiveBuf.append(data);    //记录接受了多少个字节的数据，不会清空
    if(hexCheckBox->isChecked())    //显示16进制数据
    {
        QString hexstring = toSplitHex(data);
        showStringwithTime(hexstring, timestampCheckBox->isChecked(), SerialPortWidget::receive);
    }
    else    //不显示16进制数据
    {
        QTextCodec *codec = QTextCodec::codecForLocale();   //获取 本地编码格式
        QString unicodeData = codec->toUnicode(data);   //将data的编码转为UTF-8
        showStringwithTime(unicodeData, timestampCheckBox->isChecked(), SerialPortWidget::receive);
    }
    emit signalUpdateReciveBytes(QString::number(receiveBuf.size()));

}

void SerialPortWidget::writeData(QByteArray &data)
{
    serialport->write(data);     //向串口写入数据
    sendBuf.append(data);
    emit signalUpdateSendBytes(QString::number(sendBuf.size()));
}

void SerialPortWidget::saveSetting()
{
    //QCoreApplication::applicationDirPath()获取exe所在绝对路径
    //e.g. D:/QT/test.exe返回D:/QT
    QString path = QCoreApplication::applicationDirPath() + "/setting.json";
    QFile file(path);
//    QFile file("../serial_port/data/mysetting.json");
    if(!file.open(QIODevice::WriteOnly))
    {
        qWarning() << "Couldn't open file";
        return;
    }
    QJsonObject rootObj;    //根{}
    QJsonObject obj;

    obj.insert("baudrate", baudRateComboBox->currentData().toDouble());
    obj.insert("stopBits", stopBitsComboBox->currentData().toDouble());
    obj.insert("dataBits", dataBitsComboBox->currentData().toDouble());
    obj.insert("parity", parityComboBox->currentData().toDouble());
    obj.insert("hex", hexCheckBox->isChecked());
    obj.insert("changeBackgroudColor", changeColorCheckBox->isChecked());
    obj.insert("showtime", timestampCheckBox->isChecked());
    obj.insert("showSendText", showSendTextCheckBox->isChecked());
    obj.insert("sendHEX", sendHEXCheckBox->isChecked());
    obj.insert("appendNewLine", sendLineCheckBox->isChecked());


    QJsonArray jsonarray;
    for(int i = 0; i < SerialPortWidget::buttonNum; i++)
    {
        jsonarray.append(lineEdits[i]->text());
    }

    obj.insert("mutiText", jsonarray);

    rootObj.insert("settings", obj);
    QJsonDocument jsondoc(rootObj);
    file.write(jsondoc.toJson());

    file.close();
}

QString SerialPortWidget::toSplitHex(QByteArray &bytearray)
{
    QString hexString = bytearray.toHex().toUpper();
    QString newString;
    for (int i = 0;  i < hexString.length(); i+=2)
        newString.append(hexString.mid(i, 2) + " ");
    qDebug() << hexString;
    return newString;
}

void SerialPortWidget::onclicked_clearSendButton()
{
    sendTextEdit->clear();
}

void SerialPortWidget::onclicked_changeColorCheckBox(bool isclicked)
{
    if(isclicked)
    {
        receiveTextBrowser->setStyleSheet("color: rgb(0, 170, 0);"
                                          "background-color: rgb(0, 0, 0);"
                                          "font: 9pt \"微软雅黑\";");
    }
    else
    {
        receiveTextBrowser->setStyleSheet("color: rgb(0, 0, 0);"
                                          "background-color: rgb(255, 255, 255);"
                                          "font: 9pt \"微软雅黑\";");
    }
}

void SerialPortWidget::onclicked_saveRecButton()
{
    QString path = QCoreApplication::applicationDirPath() + "/save";
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存文件"),
                               path,
                               tr("文本文件 (*.txt)"));

    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "save file error!";
        return;
    }
    QString text = receiveTextBrowser->toPlainText();
    file.write(text.toLocal8Bit());
    file.close();
}

void SerialPortWidget::onclicked_clearRecButton()
{
    receiveTextBrowser->clear();
    receiveAarry.clear();
    receiveBuf.clear();
    emit signalUpdateReciveBytes(QString::number(0));   //清空状态栏已接收和发送的字节数
    emit signalUpdateSendBytes(QString::number(0));
}

void SerialPortWidget::stateChanged_hexCheckBox(int state)
{    
    receiveTextBrowser->clear();

    if(state == Qt::Checked)    //勾选16进制显示
        receiveTextBrowser->setText(toSplitHex(receiveAarry));
    else
    {
        QTextCodec *codec = QTextCodec::codecForLocale();   //获取本地编码
        QString unicodeData = codec->toUnicode(receiveAarry);
        receiveTextBrowser->setText(unicodeData);
    }

}

void SerialPortWidget::onclicked_openSerialButton()
{

    //打开串口
    if(times % 2 == 0)
    {
        //串口设置
        serialport->setPortName(serialPortComboBox->currentText());

        QSerialPort::BaudRate b =
                static_cast<QSerialPort::BaudRate>(baudRateComboBox->currentData().toInt());
        serialport->setBaudRate(b);

        QSerialPort::DataBits databits =
                static_cast<QSerialPort::DataBits>(dataBitsComboBox->currentData().toInt());
        serialport->setDataBits(databits);

        QSerialPort::StopBits stopbits =
                static_cast<QSerialPort::StopBits>(stopBitsComboBox->currentData().toInt());
        serialport->setStopBits(stopbits);

        QSerialPort::Parity parity =
                static_cast<QSerialPort::Parity>(parityComboBox->currentData().toInt());
        serialport->setParity(parity);
        if(!serialport->open(QIODevice::ReadWrite))
        {
            QMessageBox messagebox(this);
            messagebox.setWindowTitle("警告");
            messagebox.setText("打开串口失败!\n请重新选择串口");
            messagebox.addButton(new QPushButton("确定"), QMessageBox::AcceptRole);
            messagebox.exec();  //阻塞主窗口
            return;
        }
        QPixmap pixmap(":/image/closebtn");
        openSerialButton->setIcon(pixmap);
        openSerialButton->setText("关闭串口");
        //打开串口时，不可修改参数
        serialPortComboBox->setEnabled(false);
        baudRateComboBox->setEnabled(false);
        dataBitsComboBox->setEnabled(false);
        stopBitsComboBox->setEnabled(false);
        parityComboBox->setEnabled(false);
        sendTextEdit->setReadOnly(false);

        QSerialPort tempPort(serialport->portName());
        QSerialPortInfo info(tempPort); //获取当前串口的信息

        QString portInfo;
        portInfo.append("已连接到");
        portInfo.append(serialPortComboBox->currentText());
        portInfo.append(", ");
        portInfo.append(baudRateComboBox->currentText());
        portInfo.append(", ");
        portInfo.append(stopBitsComboBox->currentText());
        portInfo.append(", ");
        portInfo.append(dataBitsComboBox->currentText());
        portInfo.append(", ");
        portInfo.append(parityComboBox->currentText());

        qDebug() << portInfo;
        emit showPortName(portInfo);  //向主窗口发射信号，用于更新状态栏

    }
    else    //关闭串口
    {
        QPixmap pixmap(":/image/openbtn");
        openSerialButton->setIcon(pixmap);
        openSerialButton->setText("打开串口");
        //恢复控件状态
        serialPortComboBox->setEnabled(true);
        baudRateComboBox->setEnabled(true);
        dataBitsComboBox->setEnabled(true);
        stopBitsComboBox->setEnabled(true);
        parityComboBox->setEnabled(true);
        sendTextEdit->setReadOnly(true);
        if(serialport->isOpen())
            serialport->close();
        emit showPortName("未连接");  //向主窗口发射信号，用于更新状态栏
    }
    times++;    //按下打开串口按钮的次数
    if(times == 100)
        times = 0;

}

void SerialPortWidget::onclicked_sendButton()
{
    //串口打开了才允许发送数据
    if(serialport->isOpen())
    {
        QString text = sendTextEdit->toPlainText();
        bool rn = sendLineCheckBox->isChecked();
        QString sendText(rn ? QString(text + "\r\n") : QString(text));
        if(!sendText.isEmpty() && showSendTextCheckBox->isChecked())
                showStringwithTime(sendText, timestampCheckBox->isChecked(), SerialPortWidget::send);
        QByteArray data = sendText.toLocal8Bit();
        writeData(data);
    }
}

void SerialPortWidget::stateChanged_timerCheckBox(int state)
{
    if(state == Qt::Checked)
    {
        int msc = timeLineEdit->text().toInt(); //定时指定时间
        timer->start(msc);  //启动定时器，msc毫秒后槽函数timeSend做出响应
    }
    else
    {
        timer->stop();
    }
}

void SerialPortWidget::timeSend()
{
    onclicked_sendButton();
}

void SerialPortWidget::limitInput(const QString &text)
{
    if(text.contains(QRegExp("\\D+")))  //匹配非数字字符
    {
        QMessageBox messagebox(this);
        messagebox.setWindowTitle("警告");
        messagebox.setText("定时时间范围500~1000000!\n请重新输入");
        messagebox.addButton(new QPushButton("确定"), QMessageBox::AcceptRole);
        messagebox.exec();  //阻塞主窗口
        timeLineEdit->setText("1000");
    }

}

void SerialPortWidget::stateChanged_sendHEXCheckBox(int state)
{
    if(state == Qt::Checked)    //如果勾选了发送16进制，将原本文本框中的数据转为16进制
    {                           //例如"这" ===> "\xD5 \xE2" ===> "D5 E2"
        QString text = sendTextEdit->toPlainText();
        QByteArray array = text.toLocal8Bit();
        sendTextEdit->clear();
        sendTextEdit->setText(toSplitHex(array));
    }
    else    //将已经输入的十六进制的字符串转为对应的字符串，例如"D5 E2" ===> "\xD5 \xE2" ===> "这"
    {
        QString text = sendTextEdit->toPlainText();
        QByteArray array = QByteArray::fromHex(text.toLocal8Bit()); //Returns a decoded copy of the hex encoded array hexEncoded.
        sendTextEdit->clear();
        sendTextEdit->setText(array);
    }
}

void SerialPortWidget::validateHEX()
{
    if(!sendHEXCheckBox->isChecked())
        return;
    QString text = sendTextEdit->toPlainText();
    QTextCursor cur = sendTextEdit->textCursor();
    int pos = cur.position() - 1;
    if(pos >= 0)
    {
        qDebug() <<text.at(pos);
        QChar ch = text.at(pos);
        if((ch >= '0' && ch <= '9')
                || (ch >= 'A' && ch <= 'F')
                || (ch >= 'a' && ch <= 'f')
                || (ch == ' '))
        {
            return;
        }
        else
        {
            cur.deletePreviousChar();   //删除光标所在位置的前一个字符
            QMessageBox messagebox(this);
            messagebox.setWindowTitle("提示");
            messagebox.setText("请输入16进制0-9,，A-F!\n按照AE 0D 3F 93的格式输入");
            messagebox.addButton(new QPushButton("确定"), QMessageBox::AcceptRole);
            messagebox.exec();  //阻塞主窗口
        }

    }
}

void SerialPortWidget::showStringwithTime(QString &showString, bool isShow, MODE m)
{
    if(isShow)
    {
        QDateTime dtime = QDateTime::currentDateTime();
        QString ctime = dtime.toString("yyyy-MM-dd hh:mm:ss:zzz");
        switch (m) {
        case SerialPortWidget::send:
            receiveTextBrowser->append("[发送时间：" + ctime + "]：" + showString);
            break;
        case SerialPortWidget::receive:
            receiveTextBrowser->append("[接收时间：" + ctime + "]：" + showString);
            break;
        default:
            break;
        }
    }
    else
    {
        receiveTextBrowser->append(showString + "\r\n");
    }
}
