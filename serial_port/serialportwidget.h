#ifndef SERIALPORTWIDGET_H
#define SERIALPORTWIDGET_H

#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QTextBrowser>
#include <QTextEdit>
#include <QWidget>
#include <QLineEdit>
#include <QJsonObject>
#include <QSerialPort>

class SerialPortWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SerialPortWidget(QWidget *parent = nullptr);

signals:
    void showPortName(const QString &portDescription);
    void signalUpdateReciveBytes(const QString &bytes);
    void signalUpdateSendBytes(const QString &bytes);
//    void clearRS(); //发射信号，清除已接收

private:
    /****发送区控件****/
    enum { buttonNum = 8 };
    enum MODE { send, receive };
    uint16_t times; //记录打开串口按下次数
    QByteArray receiveAarry;    //接收到的字节数组
    QByteArray receiveBuf;      //记录接收了多个字节
    QByteArray sendBuf;     //记录发送了多少个字节

    QSerialPort *serialport = nullptr;

    /****接收区控件****/
    QGroupBox *group1;
    QGroupBox *group2;
    QLabel *serialPortLabel;
    QLabel *baudRateLabel;
    QLabel *stopBitsLabel;
    QLabel *dataBitsLabel;
    QLabel *parityLabel;
    QLabel *openSerialLabel;
    QComboBox *serialPortComboBox;
    QComboBox *baudRateComboBox;
    QComboBox *stopBitsComboBox;
    QComboBox *dataBitsComboBox;
    QComboBox *parityComboBox;
    QPushButton *openSerialButton;
    QTextBrowser *receiveTextBrowser;
    QPushButton *saveRecButton;
    QPushButton *clearRecButton;
    QCheckBox *hexCheckBox;
    QCheckBox *changeColorCheckBox;
    QCheckBox *timestampCheckBox;

    /****发送区控件****/
    QTextEdit *sendTextEdit;
    QPushButton *sendButton;
    QPushButton *clearSendButton;
    QLineEdit *lineEdits[buttonNum];
    QPushButton *buttons[buttonNum];
    QCheckBox *showSendTextCheckBox;
    QCheckBox *sendLineCheckBox;
    QCheckBox *sendHEXCheckBox;
    QCheckBox *timerCheckBox;
    QLineEdit *timeLineEdit;

    QTimer *timer;

    /****多文本发送控件***/
//    QLabel *hexLabel;
//    QLabel *strLabel;
//    QLabel *sendLabel;
private slots:
    void onclicked_clearSendButton();
    void onclicked_changeColorCheckBox(bool isclicked);
    void onclicked_saveRecButton();
    void onclicked_clearRecButton();
    void stateChanged_hexCheckBox(int state);
    void onclicked_openSerialButton();
    void onclicked_sendButton();
    void stateChanged_timerCheckBox(int state);
    void timeSend();    //定时发送数据
    void limitInput(const QString &text);  //限制只能输入整数
    void stateChanged_sendHEXCheckBox(int state);   //发送HEX复选框槽函数


public:
    void showStringwithTime(QString &showString, bool isShow, MODE m);    //根据MODE以及isShow判断是否显示时间以及显示接收还是发送
    void setupUI(); //设置各种组件
    void createRecGroupBox();
    void createSendGroupBox();
    void createSettingJSON();   //将各种设置的数据保存为JSON格式
    void loadSettingJSON(); //加载设置
    void readData(); //根读取串口的数据
    void writeData(QByteArray &data); //向串口发送数据
    void saveSetting(); //保存设置
    QString toSplitHex(QByteArray &bytearray); //输出 "D5 E2 CA C7"这样格式的16进制数据
    void validateHEX();    //验证输入文本是否是16进制
};

#endif // SERIALPORTWIDGET_H
