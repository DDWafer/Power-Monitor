#ifndef NEWWINDOW_H
#define NEWWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
namespace Ui {
class newwindow;
}

class newwindow : public QMainWindow
{
    Q_OBJECT

public:
    //explicit newwindow(QWidget *parent = 0, QSerialPort *serial=nullptr);
     newwindow(QWidget *parent, QSerialPort *serial);
    ~newwindow();
    QSerialPort *m_serial;
    void serial_check();

signals:
    void newdata();
    void close_serial();

private slots:
    void on_OpenSerialButton_clicked();
    void ReadData();
    void timerEvent(QTimerEvent *);
    void serial_lose(QSerialPort::SerialPortError error);  //串口丢失

private:
    Ui::newwindow *ui;

};

#endif // NEWWINDOW_H
