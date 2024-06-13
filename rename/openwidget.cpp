#include "openwidget.h"
#include <QGridLayout>
#include <QPixmap>  //设置图片所需头文件
#include <QFileDialog>
#include <QMimeData>
#include <QDebug>

OpenWidget::OpenWidget(QWidget *parent) : QWidget(parent)   //: QWidget(parent) 通过初始化参数列表的方式先调用父类的构造函数
{
    setGeometry(0, 0, 900, 600);
    setAcceptDrops(true);   //允许拖拽
    group = new QGroupBox(this);
    group->setObjectName("groupbox");
    group->setGeometry(150, 100, 600, 300);
    group2 = new QGroupBox(this);
    group2->setObjectName("groupbox2");

    group2->setGeometry(0, 450, 900, 100);

    QGridLayout *layout = new QGridLayout;
    QHBoxLayout *hlayout = new QHBoxLayout;
    open_dir_btn = new QPushButton("添加文件夹");
    open_dir_btn->setFixedWidth(100);
//    open_file_btn = new QPushButton("添加文件");

    open_dir_btn->setObjectName("open_dir_btn");    //设置类名以便于qss/css文件通过id访问组件
//    open_file_btn->setObjectName("open_file_btn");  //在qss中通过QPushButton#open_file_btn{ }来设置样式

    //按钮设置样式
    open_dir_btn->setCursor(Qt::PointingHandCursor);
//    open_file_btn->setCursor(Qt::PointingHandCursor);

    //标签设置样式
    pic_label = new QLabel;
    label1 = new QLabel("将文件夹拖拽至此处或点击下方按钮");
    label1->setFont(QFont("微软雅黑", 12));
    label2 = new QLabel("第1步：添加文件所在文件夹\t>\t第2步：设置重命名参数\t>\t第3步：一键重命名");
    label2->setFont(QFont("微软雅黑", 12));
    QPixmap pixmap(":/image/copy_dir.png");
    pic_label->setPixmap(pixmap);

    //Grid布局管理器添加器件
//    layout->addWidget(open_file_btn, 2, 1, Qt::AlignTop);
    layout->addWidget(open_dir_btn, 2, 0, 1, 5, Qt::AlignHCenter | Qt::AlignTop);
    layout->addWidget(pic_label, 0, 0, 1, 5, Qt::AlignHCenter); //水平居中
    layout->addWidget(label1, 1, 0, 1, 5, Qt::AlignHCenter | Qt::AlignTop); //水平居中且向上对齐

    //水平布局管理器
    hlayout->addWidget(label2, 0, Qt::AlignHCenter);

    group->setLayout(layout);
    group2->setLayout(hlayout);
    connect(open_dir_btn, &QPushButton::clicked, this, &OpenWidget::onclicked_open_btn);   //按下按键发出switchToOptwidget信号
//    connect(open_file_btn, &QPushButton::clicked, this, &OpenWidget::onclicked_open_file_btn);
}

void OpenWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();  //接受设置QT默认的行为
        setCursor(Qt::DragMoveCursor);  // 设置自定义的光标图标
    } else {
        setCursor(Qt::ForbiddenCursor);  // 如果不是文件URL，设置禁止图标
    }
}

void OpenWidget::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        QList<QUrl> urlList = event->mimeData()->urls();
        for (int i = 0; i < urlList.size(); i++) {
            QString file = urlList.at(i).toLocalFile();
//            qDebug() << file << "\n";
            emit switchToOptwidget(file);   //发出信号，并传递文件夹路径
        }
        event->acceptProposedAction();
        unsetCursor();  // 重置光标图标
    } else {
        setCursor(Qt::ForbiddenCursor);  // 如果不是文件URL，设置禁止图标
    }
}

void OpenWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    Q_UNUSED(event);    //告诉编译器函数体没有使用参数
    setCursor(Qt::ArrowCursor);
}

void OpenWidget::onclicked_open_btn()
{
    //打开一个对话框
    QString dir = QFileDialog::getExistingDirectory(this, tr("打开文件夹"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if(dir.isEmpty())
        return;
    emit switchToOptwidget(dir);   //发出信号，并传递文件夹路径
}

void OpenWidget::onclicked_open_file_btn()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"), "/home", tr("All Files (*.*)"));
//    qDebug() << fileName;

}
