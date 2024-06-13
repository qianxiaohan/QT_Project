#ifndef OPTWIDGET_H
#define OPTWIDGET_H

#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
#include <QFileDialog>
#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QGroupBox>
#include <QSpinBox>
#include <QIcon>
#include <QRadioButton>
#include <QCheckBox>
#include "mytablewidget.h"
#include <QMap>


class OptWidget : public QWidget
{
    Q_OBJECT

public:
    OptWidget(QWidget *parent = nullptr);
    ~OptWidget();

    void updateTable(const QString &dir_path);   //根据路径修改表格

signals:
    void switchToOpenwidget();  //自定义信号，切换到另一个界面
    void update_table_signal(int); //表格更新后发送信号

private:
    enum SIZE_WINDOW{
        WIDTH = 900,
        HEIGHT = 600
    };

    QList<int> selectedRows;        //存储选择了哪些行
    QString current_path;   //当前工作路径
    QPushButton *open_btn;  //打开文件夹按钮
    QPushButton *update_btn;    //更新按钮
    QPushButton *reset_btn;   //重置序号按钮
    QPushButton *change_dir_btn;   //更改目录按钮
    QPushButton *return_btn;    //返回主界面按钮
    MyTableWidget *table;
    QLineEdit *updateline;  //修改文本框
    QLineEdit *outputdir;  //显示输出目录
    QLabel *updatelabel;
    QLabel *num_start_label;        //序号起始标签
    QLabel *num_interval_label;     //序号间隔标签
    QLabel *output_dir_label; //输出目录标签
    QGroupBox *group;
    QSpinBox *start_spinbox;
    QSpinBox *interval_spinbox;
    QRadioButton *qrb1;
    QRadioButton *qrb2;
    QCheckBox *all_check;   //全选按钮

    //member functions
    void createGridlayout();
    void createTable();
    void createComponet();
    void setComponetFont(int fontsize);
    void setDefault();      //所有部件置为默认状态
    void addRows(int row);   //将所选行添加到一个QList

    //槽函数
private slots:
    //void openfile();    //
    void getLineEditText(const QString &str);   //获取LineEdit文本
    void onclicked_reset_btn();   //重置按钮
    void onclicked_radio_btn();
    void onclicked_radio1_btn();    //两个单选按钮
    void onclicked_chage_btn();     //更改目录按钮
    void onclicked_return_btn();        //返回主界面按钮
    void onclicked_all_check(bool is_checked); //全选框被勾选
    void onclicked_update_btn();    //重命名按钮
    void onclicked_open_btn();      //按钮打开打开文件
    void slot_value_changed(int value);      //修改spinbox的触发的槽函数
    void slot2_value_changed(int value);

};
#endif //OPTWIDGET_H
