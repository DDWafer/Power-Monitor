#include "acc.h"
#include "ui_acc.h"

acc::acc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::acc)
{
    ui->setupUi(this);
    QPixmap image; //定义一张图片

    //image.load("F:/now work/power_control_beijing/source_5tab/source_5tab/qt_3/power.png");//加载
    image.load("power.png");//加载
    ui->label_6->clear();//清空
    ui->label_6->setPixmap(image);//加载到Label标签
    ui->label_6->show();//显示
}

acc::~acc()
{
    delete ui;
}

void acc::on_pushButton_clicked()
{
     this->hide();
}
