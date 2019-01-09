#include "newwindow.h"
#include "ui_newwindow.h"
#include "stdio.h"
#include <QDebug>
#include <QByteArray>
int timer_new_ID=0;
newwindow::newwindow(QWidget *parent,QSerialPort *serial) :
    QMainWindow(parent),
    ui(new Ui::newwindow)
{

    ui->setupUi(this);
    //检查可用的串口
    serial_check();
   //m_serial=new  QSerialPort;
    m_serial=serial;
    //连接信号槽
    QObject::connect(m_serial,&QSerialPort::readyRead,this,&newwindow::ReadData);
    connect(m_serial,SIGNAL(errorOccurred(QSerialPort::SerialPortError)),this,SLOT(serial_lose(QSerialPort::SerialPortError)));
    timer_new_ID=this->startTimer(5000);
}

newwindow::~newwindow()
{
    delete ui;
}

void newwindow::on_OpenSerialButton_clicked()
{
    if(ui->OpenSerialButton->text() == tr("打开串口"))
    {
       // m_serial=serial;
      //  m_serial = new QSerialPort;
        //设置串口名
        m_serial->setPortName(ui->PortBox->currentText());
        //打开串口
        m_serial->open(QIODevice::ReadWrite);
        //设置波特率
        switch (ui->BaudBox->currentIndex())
        {
        case 2:
            m_serial->setBaudRate(QSerialPort::Baud115200);//设置波特率为115200
            break;
        case 1:
            m_serial->setBaudRate(QSerialPort::Baud9600);//设置波特率为9600
            break;
        case 0:
            m_serial->setBaudRate(QSerialPort::Baud4800);//设置波特率为4800
            break;
        default:
            break;
        }
        //设置数据位数
        switch (ui->BitBox->currentIndex())
        {
        case 8:
            m_serial->setDataBits(QSerialPort::Data8);//设置数据位8
            break;
        default:
            break;
        }
        //设置校验位
        switch (ui->ParityBox->currentIndex())
        {
        case 0:
            m_serial->setParity(QSerialPort::NoParity);
            break;
        default:
            break;
        }
        //设置停止位
        switch (ui->BitBox->currentIndex())
        {
        case 1:
            m_serial->setStopBits(QSerialPort::OneStop);//停止位设置为1
            break;
        case 2:
            m_serial->setStopBits(QSerialPort::TwoStop);
        default:
            break;
        }
        //设置流控制
        m_serial->setFlowControl(QSerialPort::NoFlowControl);//设置为无流控制
        if(m_serial->isOpen())
        {
            //关闭设置菜单使能
            ui->PortBox->setEnabled(false);
            ui->BaudBox->setEnabled(false);
            ui->BitBox->setEnabled(false);
            ui->ParityBox->setEnabled(false);
           // ui->StopBox->setEnabled(false);
            ui->OpenSerialButton->setText(tr("关闭串口"));
        }
    }
    else
    {
        //关闭串口
        m_serial->clear();
        m_serial->close();
        m_serial->destroyed();
       // emit close_serial();
        //恢复设置使能
        ui->PortBox->setEnabled(true);
        ui->BaudBox->setEnabled(true);
        ui->BitBox->setEnabled(true);
        ui->ParityBox->setEnabled(true);
       // ui->StopBox->setEnabled(true);
        ui->OpenSerialButton->setText(tr("打开串口"));
        serial_check();

    }
}
void newwindow::ReadData()
{
    emit newdata();
}
void newwindow::serial_check(){
    //查找可用的串口
    const auto infos = QSerialPortInfo::availablePorts();
    ui->PortBox->clear();
    for (const QSerialPortInfo &info : infos)
    {
        QSerialPort t_serial;
        t_serial.setPort(info);
        //如果某个串口打开，读取正常，统统关闭
        if(t_serial.open(QIODevice::ReadWrite))
        {
            ui->PortBox->addItem(info.portName());
            t_serial.close();
        }
    }
}
void newwindow::timerEvent(QTimerEvent *){
    if(ui->OpenSerialButton->text() == tr("打开串口")){
        serial_check();
    }
}

void newwindow::serial_lose(QSerialPort::SerialPortError error)
{
    if(error==2)       //拔出串口
    {
        if(ui->OpenSerialButton->text() == tr("关闭串口"))
        {

//            m_serial->destroyed();
           // emit close_serial();
            //恢复设置使能
            ui->PortBox->setEnabled(true);
            ui->BaudBox->setEnabled(true);
            ui->BitBox->setEnabled(true);
            ui->ParityBox->setEnabled(true);
           // ui->StopBox->setEnabled(true);
            ui->OpenSerialButton->setText(tr("打开串口"));
            m_serial->clearError();
            m_serial->clear();
            m_serial->close();
            m_serial->destroyed();
            serial_check();
        }

    }

}

