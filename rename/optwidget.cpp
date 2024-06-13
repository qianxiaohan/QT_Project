#include "optwidget.h"
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QAbstractButton>
#include <QDebug>
#include <QDir>     //获取指定文件夹下的所有文件名
#include <QCollator>    //比较String大小

QString getFileSize(qint64 size)//将字节数size转换为KB、MB、GB
{
    qint64 kiloBytes = 1024;    //1KB, 1024B
    qint64 megaBytes = kiloBytes * 1024;    //1MB, 1024 * 1024B
    qint64 gigaBytes = megaBytes * 1024;    //1GB, 1024 * 1024 * 1024B

    if(size < kiloBytes){
        return QString::number(size) + "B";
    }
    else if(size < megaBytes){
        double kbsize = static_cast<double>(size) / kiloBytes;  //qint64转为double
        return QString::number(kbsize, 'f', 2) + "KB"; //返回一个小数点后两位的QString对象
//        qDebug() << size << "B";
    }
    else if(size < gigaBytes){
        double mbsize = static_cast<double>(size) / megaBytes;
        return QString::number(mbsize, 'f', 2) + "MB";
    }
    else{
        double gbsize = static_cast<double>(size) / gigaBytes;
        return QString::number(gbsize, 'f', 2) + "GB";
    }
}


OptWidget::OptWidget(QWidget *parent)
    : QWidget(parent)
{
    setGeometry(0, 0, WIDTH, HEIGHT);
    //setFixedSize(WIDTH, HEIGHT);     //窗口大小

    //setAttribute(Qt::WA_DeleteOnClose, true); //关闭界面自动销毁

    createComponet();   //生成各种器件
    createTable();      //创建一个表格
    createGridlayout(); //创建一个Gridlayout布局
    setComponetFont(11);  //设置部件字体

    //SIGNAL()、SLOT()是QT4的写法
    connect(open_btn, SIGNAL(clicked()), this, SLOT(onclicked_open_btn())); //信号与槽函数连接
    connect(updateline, SIGNAL(textChanged(QString)), this, SLOT(getLineEditText(QString)));
    connect(reset_btn, SIGNAL(clicked()), this, SLOT(onclicked_reset_btn()));
    connect(qrb2, SIGNAL(clicked()), this, SLOT(onclicked_radio_btn()));
    connect(qrb1, SIGNAL(clicked()), this, SLOT(onclicked_radio1_btn()));
    connect(change_dir_btn,SIGNAL(clicked()), this, SLOT(onclicked_chage_btn()));
    //QT5写法，
    connect(return_btn, &QPushButton::clicked, this, &OptWidget::onclicked_return_btn);
    connect(all_check, &QCheckBox::clicked, this, &OptWidget::onclicked_all_check);
    connect(update_btn, &QPushButton::clicked, this, &OptWidget::onclicked_update_btn);
    //[signal] void QSpinBox::valueChanged(int i)，[signal] void QSpinBox::valueChanged(const QString &text)
    //QSpinBox有两个信号函数重载，要告诉编译器使用的哪个信号函数，因此可以使用函数指针，或者使用QT4写法
    void(QSpinBox::*valueChangedInt)(int) = &QSpinBox::valueChanged;    //使用函数指针
    connect(start_spinbox, valueChangedInt, this, &OptWidget::slot_value_changed);
    connect(interval_spinbox, valueChangedInt, this, &OptWidget::slot_value_changed);
}

OptWidget::~OptWidget()
{

}

void OptWidget::createGridlayout()
{
    group = new QGroupBox(this);
    QGridLayout *glayout = new QGridLayout;

    //gridlayout第一行设置
    glayout->addWidget(updatelabel, 0, 0, Qt::AlignRight);
    glayout->addWidget(updateline, 0, 1, 1, 2); // 该组件横跨2列
    glayout->addWidget(num_start_label, 0, 3, Qt::AlignRight);
    glayout->addWidget(start_spinbox, 0, 4, 0);
    glayout->addWidget(num_interval_label, 0, 5, Qt::AlignRight);
    glayout->addWidget(interval_spinbox, 0, 6, 0);
    glayout->addWidget(reset_btn, 0, 7, 0);

    //gridlayout第二行设置，组件不设置对齐方式
    glayout->addWidget(output_dir_label, 1, 0, Qt::AlignRight); //第二行，第一列添加组件
    //glayout->addWidget(return_btn, 1, 0, Qt::AlignRight); //第二行，第一列添加组件
    glayout->addWidget(qrb1, 1, 1);
    glayout->addWidget(qrb2, 1, 2);
    glayout->addWidget(outputdir, 1, 3);
    glayout->addWidget(change_dir_btn, 1, 4);


    //网格布局第三行设置
    glayout->addWidget(return_btn, 2, 0, Qt::AlignRight);
    glayout->addWidget(open_btn, 2, 1, 0);
    glayout->addWidget(update_btn, 2, 2, 0);

    //glayout->setHorizontalSpacing(20);

    group->setGeometry(0, 0, WIDTH, 150);
    group->setLayout(glayout);

}

void OptWidget::createTable()
{
    table = new MyTableWidget(this);
    table->setGeometry(0, 150, WIDTH, 400);
    table->setColumnCount(5);
    //设置列宽
    table->setColumnWidth(0, 56);
    table->setColumnWidth(1, 300);
    table->setColumnWidth(2, 300);
    table->setColumnWidth(3, 110);
    table->setColumnWidth(4, 90);


    all_check = new QCheckBox("");  //全选框
    all_check->setChecked(true);    //默认全选
    all_check->setFixedSize(16, 16);
    table->setHorizontalHeaderLabels(QStringList() << "" << "文件名" << "修改文件名预览"
                                     << "文件大小" << "执行结果");
    table->SetHorizontalHeaderItemWidget(0, all_check);     //自定义类实现，表头添加组件
    table->SetHorizontalHeaderItemMargins(0, MyHeaderView::Margins(10,10,5,5)); //设定组件的在表头的位置


    table->setFont(QFont("微软雅黑",11));
    table->setAlternatingRowColors(true);   //允许交替修改表格颜色
    table->setShowGrid(false);  //取消表格线
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);  //不允许编辑表格
    table->setSelectionMode(QAbstractItemView::NoSelection);    //不允许选择单元格
    table->setFocusPolicy(Qt::NoFocus);     //取消焦点框
    table->verticalHeader()->hide();    //隐藏行号
    table->horizontalHeader()->setCascadingSectionResizes(false);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);    //不允许拉拽表格
    table->getHeaderView()->SetHeaderAlignment(Qt::AlignLeft);        //设置表头文字对齐方式，全选框居中
//    table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);   //设置默认居左

    table->horizontalHeader()->setFrameShape(QFrame::NoFrame);
//    table->horizontalHeader()->setStretchLastSection(true); //最后一列自适应大小
    table->setFrameShape(QFrame::NoFrame);
}

void OptWidget::createComponet()
{
    //按钮设置
    open_btn = new QPushButton("打开文件夹");
    update_btn = new QPushButton("一键修改");
    change_dir_btn = new QPushButton("更改目录");
    return_btn = new QPushButton("返回主界面");
    QIcon reset_icon(":/image/reset.png");
    reset_btn = new QPushButton(reset_icon, "重置");

    open_btn->setObjectName("opt_open_btn");
    update_btn->setObjectName("opt_update_btn");
    change_dir_btn->setObjectName("change_dir_btn");
    return_btn->setObjectName("opt_return_btn");
    reset_btn->setObjectName("reset_btn");
    open_btn->setFixedWidth(100);


    change_dir_btn->setEnabled(false);  //更改目录按钮默认不启用
    change_dir_btn->setVisible(false);
    QSizePolicy qsp =  change_dir_btn->sizePolicy();
    qsp.setRetainSizeWhenHidden(true); //隐藏时不改变控件大小
    change_dir_btn->setSizePolicy(qsp);
    change_dir_btn->setFixedWidth(70);
//    change_dir_btn->setStyleSheet("QPushButton")

    qrb1 = new QRadioButton("原文件夹");
    qrb2 = new QRadioButton("自定义");
    qrb1->setChecked(true);     //默认输出到当前文件夹
    qrb1->setCursor(Qt::PointingHandCursor);
    qrb2->setCursor(Qt::PointingHandCursor);


    QFont font("Courier");
    open_btn->setFont(font);
    open_btn->setToolTip("open file");


    update_btn->setFont(font);
    update_btn->setToolTip("更新文件");

    //按钮设置光标样式
    open_btn->setCursor(Qt::PointingHandCursor);
    update_btn->setCursor(Qt::PointingHandCursor);
    change_dir_btn->setCursor(Qt::PointingHandCursor);
    return_btn->setCursor(Qt::PointingHandCursor);
    reset_btn->setCursor(Qt::PointingHandCursor);

    //输入控件设置
    start_spinbox = new QSpinBox();
    interval_spinbox = new QSpinBox();

    start_spinbox->setRange(1, 200);
    interval_spinbox->setRange(1, 10);

    start_spinbox->setFixedWidth(46);
    interval_spinbox->setFixedWidth(46);

    //start_spinbox->resize(40,40);

    //行文本框设置
    updateline = new QLineEdit;
    outputdir = new QLineEdit;
    updateline->setFont(QFont("微软雅黑",10));
    updateline->setReadOnly(true);
    updateline->setObjectName("updateline");
    updateline->setFixedWidth(200);
    //updateline->setVisible(false);
    outputdir->setFixedWidth(200);
    outputdir->setReadOnly(true);   //只读
    outputdir->setVisible(false);   //默认不可见
    qsp =  outputdir->sizePolicy();
    qsp.setRetainSizeWhenHidden(true);  //隐藏时不改变控件大小
    outputdir->setSizePolicy(qsp);
    outputdir->setObjectName("outputdir");
    outputdir->setCursor(Qt::ForbiddenCursor);

    //标签设置
    updatelabel = new QLabel("文件名前缀：");
    num_start_label = new QLabel("起始序号：");
    num_interval_label = new QLabel("间隔数：");
    output_dir_label = new QLabel("输出目录：");

}

//设置部件的字体
void OptWidget::setComponetFont(int fontsize)
{
    QFont font("微软雅黑", fontsize);
    open_btn->setFont(font);
    update_btn->setFont(font);    //更新按钮
    reset_btn->setFont(font);   //重置序号按钮
    //table->setFont(font);
    updateline->setFont(font);  //修改文本框
    outputdir->setFont(font);  //显示输出目录
    updatelabel->setFont(font);
    num_start_label->setFont(font);        //序号起始标签
    num_interval_label->setFont(font);     //序号间隔标签
    output_dir_label->setFont(font); //输出目录标签
    group->setFont(font);
    start_spinbox->setFont(font);
    interval_spinbox->setFont(font);
    qrb1->setFont(font);
    qrb2->setFont(font);
}

void OptWidget::setDefault()
{
    table->clearContents();                 //清空数据
    table->setRowCount(0);
    updateline->setReadOnly(true);
    updateline->clear();
    qrb1->setChecked(true);
    qrb2->setChecked(false);
    start_spinbox->setValue(1);
    interval_spinbox->setValue(1);
    outputdir->setReadOnly(true);
    outputdir->clear();
    outputdir->setVisible(false);
    change_dir_btn->setEnabled(false);
    change_dir_btn->setVisible(false);
}

void OptWidget::addRows(int row)
{
    selectedRows.append(row);
    table->selectRow(row);
}

void OptWidget::updateTable(const QString &dir_path)
{
    table->clearContents();     //更新表格前清除表中内容
    selectedRows.clear();       //清空所选行
    current_path = dir_path;    //设置当前工作路径
    outputdir->clear();         //清除并重新设置目录
    outputdir->setText(dir_path);
    update_btn->setEnabled(false);
    QDir filedir(dir_path);
    QStringList postfixs;
    QStringList filenames = filedir.entryList(QDir::Files, QDir::NoSort); //获取dir目录下的所有文件名
    QCollator collator;
    collator.setNumericMode(true);
    std::sort(filenames.begin(), filenames.end(), collator);    //按数字大小进行排序
    int n = filenames.count();
    table->setRowCount(n);

    for (int i = 0; i < n; i++) {
        QStringList temp = filenames[i].split('.');
        postfixs << temp[temp.count()-1];   //后缀名
        QTableWidgetItem *item = new QTableWidgetItem(filenames[i]);
        QCheckBox *checkbox = new QCheckBox;
//        int row = selectedRows.at(i);   //获取选择的行
        QString fullname = current_path + QString("/") + filenames[i];//获取完整路径
        QFile file(fullname);   //读取文件的大小
        //为每个复选框绑定点击事件
        connect(checkbox, &QCheckBox::stateChanged, this, [=](int state)
        {
            if(state == Qt::Checked)
                selectedRows.append(i);
            else
                selectedRows.removeOne(i);
            if(!selectedRows.isEmpty())
                updateline->setReadOnly(false); //当有一行以上的表格数据被选上，文件名修改框可以修改
            else
                updateline->setReadOnly(true);
            if(selectedRows.size() < n)
                all_check->setChecked(false);
            else
                all_check->setChecked(true);
//            qDebug() << selectedRows;   //打印选中行
        }
        );

        //第一列添加复选框QCheckBox
        QWidget *widget = new QWidget;
        QHBoxLayout *layout = new QHBoxLayout;
        checkbox->setChecked(true);
        layout->setMargin(0);
        layout->addWidget(checkbox);
        layout->setAlignment(Qt::AlignCenter);
        widget->setLayout(layout);
        table->setCellWidget(i, 0, widget);
        table->setItem(i, 1, item);

        //读取文件大小
        if(file.exists())
        {
            QString filesize = getFileSize(file.size());
            QTableWidgetItem *item2 = new QTableWidgetItem(filesize);    //在表中显示文件的大小
            table->setItem(i, 3, item2);
        }
        file.close();
    }


    updateline->setReadOnly(false);     //允许输入文件名前缀
//    outputdir->setText(dir_path);       //显示文件所在路径

    emit update_table_signal(table->rowCount());     //向主窗口发送更新状态栏的信号
}

//槽函数定义
void OptWidget::onclicked_open_btn()
{
    QString path = outputdir->text();

    if(path.isEmpty())
        path = "/home";

    QString dir = QFileDialog::getExistingDirectory(this, tr("打开文件夹"),
                                                    path,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    //outputdir->setText(dir);
    if(dir.isEmpty())
        return;
    updateTable(dir);
    updateline->clear();
}

void OptWidget::slot_value_changed(int value)
{
    if(!updateline->text().isEmpty())   //输入框非空串时修改表中的文件名
    {
        for (int i = 0; i < selectedRows.size(); i++) {
            int row = selectedRows.at(i);
            QStringList templist = table->item(i, 1)->text().split('.');
            QString postfix = templist[templist.count()-1]; //后缀名
            int d = interval_spinbox->value();
            int a_i = value + i*d; //等差数列，ai = a1 + (i-1)d
            QString newstr = updateline->text() + QString::number(a_i) + "." + postfix;
            QTableWidgetItem *item = new QTableWidgetItem(newstr);
            table->setItem(row, 2, item);
        }
    }
}

void OptWidget::slot2_value_changed(int value)
{
    if(!updateline->text().isEmpty())   //输入框非空串时修改表中的文件名
    {
        for (int i = 0; i < selectedRows.size(); i++) {
            int row = selectedRows.at(i);
            QStringList templist = table->item(i, 1)->text().split('.');
            QString postfix = templist[templist.count()-1]; //后缀名
            int a1 = start_spinbox->value();
            int a_i = a1 + i*value; //等差数列，ai = a1 + (i-1)d
            QString newstr = updateline->text() + QString::number(a_i) + "." + postfix;
            QTableWidgetItem *item = new QTableWidgetItem(newstr);
            table->setItem(row, 2, item);
        }
    }
}

//获取LineEdit的文本处理后将其显示到表格中
void OptWidget::getLineEditText(const QString &str)
{
    qDebug() << str;
    if(!str.isEmpty() && all_check->isChecked()) //不为空串时启用更新按钮
        update_btn->setEnabled(true);
    else
        update_btn->setEnabled(false);

    for(int i = 0; i < selectedRows.size(); i++)
    {
        int row = selectedRows.at(i);   //获取行号
        if(!str.isEmpty())
        {
            QStringList templist = table->item(i, 1)->text().split('.');
            QString postfix = templist[templist.count()-1];
            int v1 = start_spinbox->value();
            int d = interval_spinbox->value();
            int a_i = v1 + i*d; //等差数列，ai = a1 + (i-1)d
            QString newstr = str + QString::number(a_i) + "." + postfix;
            QTableWidgetItem *item = new QTableWidgetItem(newstr);
            table->setItem(row, 2, item);
        }
        else
        {
            table->setItem(row, 2, new QTableWidgetItem(""));
        }
    }
}

void OptWidget::onclicked_reset_btn()
{
    start_spinbox->setValue(1);
    interval_spinbox->setValue(1);
    updateline->clear();    //清空文本框
}

void OptWidget::onclicked_radio_btn()
{
    change_dir_btn->setEnabled(true);
    outputdir->setVisible(true);
    change_dir_btn->setVisible(true);
//    outputdir->show();
//    change_dir_btn->show();
}

void OptWidget::onclicked_radio1_btn()
{

    change_dir_btn->setEnabled(false);
//    outputdir->hide();
    outputdir->setVisible(false);
    change_dir_btn->setVisible(false);
//    change_dir_btn->hide();
}

void OptWidget::onclicked_chage_btn()
{

    QString path = outputdir->text();
    QString dir = QFileDialog::getExistingDirectory(this, tr("选择保存路径"),
                                                    path,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if(!dir.isEmpty())
        outputdir->setText(dir);
}

void OptWidget::onclicked_return_btn()
{
    emit switchToOpenwidget();
    setDefault();
}

//全选按钮槽函数
void OptWidget::onclicked_all_check(bool is_checked)
{
    updateline->setReadOnly(!is_checked);
    if(updateline->text().isEmpty())    //如果前缀名输入框没有字符，不启用更新按钮
        update_btn->setEnabled(false);
    else
        update_btn->setEnabled(is_checked); //当全选按钮选中且输入框有字符，才启用更新按钮
    for (int i = 0; i < table->rowCount(); i++ ) {
        QCheckBox *tempcheck = qobject_cast<QCheckBox*>(table->cellWidget(i, 0)->children().at(1));   //将QWidget向下转换为QCheckBox
        if(tempcheck)
        {
            tempcheck->setChecked(is_checked);
            if(!is_checked)
            {
                qDebug() << selectedRows;
                for(int i = 0; i < selectedRows.size(); i++)
                {
                    selectedRows.removeAt(i);
                }

            }
        }
    }
}

void OptWidget::onclicked_update_btn()
{
    //读取文件名，并进行重命名

    for(int i = 0; i < selectedRows.size(); i++)
    {
        int row = selectedRows.at(i);   //获取选择的行
        QString filename = table->item(row, 1)->text();   //获取文件名
        QString fullname = current_path + "/" + filename;
        QString newname = table->item(row, 2)->text();
        QString newpath = outputdir->text();
        QString newfilename = outputdir->text() + "/" + newname;
//        qDebug() << "newname: " << newname << '\n';
        QFile file(fullname);   //使用完整的绝对路径

        if(file.exists())
        {
            qDebug() << file.size();
            if(file.rename(newfilename))
            {
                table->setItem(row, 4, new QTableWidgetItem("成功"));

            }
            else
            {
                table->setItem(row, 4, new QTableWidgetItem("失败"));
            }
        }
        file.close();   //关闭文件
    }
    updateline->setReadOnly(true);
    update_btn->setEnabled(false);  //不允许按下更新按钮，需要重新选择文件夹
}

