#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qstandarditemmodel.h"
#include <QtDebug>
#include "para.h"
#include <QFile>
#include <QTextStream>
#include <QDataStream>
#include <iostream>
#include <fstream>
#include <string>
#include <QDir>
#include "qwt_legend.h"
#include "qwt_legend_label.h"
#include <synchapi.h>
#include <QTime>
#include <QTextCodec>
#include "warning.h"
#include <acc.h>

//#include <Qt/qmath.h>

#include <QTimer>
#include <QThread>

//#pragma execution_character_set("utf-8")
int page_index=0;     //当前tab页码
int local_state=IDLE;      //当前状态
int valid_page_index[5]={0};     //有效数据的页码
int current_page=0;    //当前页数
int auto_state=OFF;     //auto执行状态
int new_win_initial=OFF;        //初始化串口设置窗口
struct  Data_Temp   data_temp[5]=//有更改============================================，volt和时间可以是浮点
{
    {0,0,0,{0},{0},{0},0},{0,0,0,{0},{0},{0},0},{0,0,0,{0},{0},{0},0},{0,0,0,{0},{0},{0},0},{0,0,0,{0},{0},{0},0}
};
struct  Data_Acp    data_acp=
{
//     .real_last_time=(Node *)malloc(sizeof(Node)),
//     .real_volt=(Node *)malloc(sizeof(Node)),
//     .real_loop_times=(Node *)malloc(sizeof(Node)),
     .real_last_time={0},
     .real_volt={0},
     .real_loop_times={0},
     .real_node_number=0       //实际节点数
};
struct  Graph_Para  graph_para[3]=   //误差图，实际图，设计图
{
{
    .x_in_max=0,     //实际图像横坐标最大值
    .y_in_max=0,     //实际图像纵坐标最大值
    .x_out_max=0,    //设计图像横坐标最大值
    .y_out_max=0,    //设计图像纵坐标最大值
    .x_in_interval=0,    //实际图像横坐标间隔值
    .y_in_interval=0,    //实际图像纵坐标间隔值
    .x_out_interval=0,   //设计图像横坐标间隔值
    .y_out_interval=0,   //设计图像纵坐标间隔值
    .gra_node_num=0,        //图中节点数
    .final_x=0,        //最后一个点的横坐标
    .gra_loop_times=0,     //各循环节点循环次数
    .gra_scan_row=0,
    .gra_last_time={0},  //电压持续时间
    .gra_volt={0},  //电压值
    .gra_loop_head_row=0,
    .state=0

},
{
    .x_in_max=0,     //实际图像横坐标最大值
    .y_in_max=0,     //实际图像纵坐标最大值
    .x_out_max=0,    //设计图像横坐标最大值
    .y_out_max=0,    //设计图像纵坐标最大值
    .x_in_interval=0,    //实际图像横坐标间隔值
    .y_in_interval=0,    //实际图像纵坐标间隔值
    .x_out_interval=0,   //设计图像横坐标间隔值
    .y_out_interval=0,   //设计图像纵坐标间隔值
    .gra_node_num=0,        //图中节点数
    .final_x=0,        //最后一个点的横坐标
    .gra_loop_times=0,     //各循环节点循环次数
    .gra_scan_row=0,
    .gra_last_time={0},  //电压持续时间
    .gra_volt={0},  //电压值
    .gra_loop_head_row=0,
    .state=0
},
{
    .x_in_max=0,     //实际图像横坐标最大值
    .y_in_max=0,     //实际图像纵坐标最大值
    .x_out_max=0,    //设计图像横坐标最大值
    .y_out_max=0,    //设计图像纵坐标最大值
    .x_in_interval=0,    //实际图像横坐标间隔值
    .y_in_interval=0,    //实际图像纵坐标间隔值
    .x_out_interval=0,   //设计图像横坐标间隔值
    .y_out_interval=0,   //设计图像纵坐标间隔值
    .gra_node_num=0,        //图中节点数
    .final_x=0,        //最后一个点的横坐标
    .gra_loop_times=0,     //各循环节点循环次数
    .gra_scan_row=0,
    .gra_last_time={0},  //电压持续时间
    .gra_volt={0},  //电压值
    .gra_loop_head_row=0,
    .state=0
}
};

int pre_hour=1;//用于判断需不需要添加新表格=================================
int change_page=9999;//判断页数改变没有
//===========================
int exce_1=0;
int exce_2=0;
int exce_3=0;
int exce_4=0;
int exce_5=0;//判断exce执行状态。0表示初次运行程序和当前表格；1表示正常输入数据；2表示第二次点击某一张表格运行。===========================================================
int timer_main_ID=0;
float datatemp[100][3]={0};  //本地缓存==========================================可以是浮点型了
float load_datatemp[100][3]={0};  //读入缓存===================================浮点型
int valid_row_number=0;//有效行数
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));

    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));

    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(setpage(int)));
//初始化版权界面
   about = new acc(0);

//初始化串口配置界面
   serial=new  QSerialPort;

   new_win = new newwindow(0,serial);  //初始化串口配置窗口

   connect(new_win,SIGNAL(newdata()),SLOT(ReadData()));

   connect(new_win,SIGNAL(close_serial()),SLOT(serial_close()));

//初始化退出保存界面
    exit_dialog = new Exit_Dialog(0);

    connect(exit_dialog,SIGNAL(save()),SLOT(on_actionsave_all_triggered()));
//初始化存储名界面
    tab_name=new QString;
    name_dialog = new Dialog(0,tab_name);
   // connect(name_dialog,SIGNAL(returnPressed()),this,SLOT(save_name()));//====================
    connect(name_dialog,SIGNAL(load()),SLOT(save_slot()));//===================
//初始化警告窗口
    warning =new Form(0);
    connect(warning,SIGNAL(warn()),SLOT(stop_excute()));
 //初始化tab1
    tab_1_ini();
//初始化tab2
    tab_2_ini();
//初始化tab3
    tab_3_ini();
//初始化tab4
    tab_4_ini();
//初始化tab5
    tab_5_ini();

//创建定时器
   timer_main_ID=this->startTimer(1000);
    //



////--------------设置图例可以被点击来确定是否显示曲线-----------------------//
//       QwtLegend *legend = new QwtLegend;
//       legend->setDefaultItemMode( QwtLegendData::Checkable );//图例可被点击
//       insertLegend( legend, QwtPlot::RightLegend );
//       connect( legend, SIGNAL( checked( const QVariant &, bool, int ) ),
//           SLOT( showItem( const QVariant &, bool ) ) );//点击图例操作

//       QwtPlotItemList items = itemList( QwtPlotItem::Rtti_PlotCurve );//获取画了多少条曲线,如果为获取其他形状，注意改变参数
//      //  qDebug()<<items;
//       for ( int i = 0; i < items.size(); i++ )
//       {

//           if ( i == 0 )
//           {
//               const QVariant itemInfo = itemToInfo( items[i] );

//               QwtLegendLabel *legendLabel =
//                   qobject_cast<QwtLegendLabel *>( legend->legendWidget( itemInfo ) );
//               if ( legendLabel )
//                   legendLabel->setChecked( true );//

//               items[i]->setVisible( true );
//           }
//           else
//           {
//               items[i]->setVisible( false );
//           }
//       }
 //ui->tabWidget->setTabText(0,s);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::delaymsec(int msec)
{

    QTime n=QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime()<n)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }
}

void MainWindow::save_data(float timedata,float voltdata,float voltdata2,int current_valid,int loopcount,int hour)//================================================
{
    //int change_page=9999;
    if (current_page==change_page)
    {
        ;
    }
    else
    {
        pre_hour=1;
    }
 switch (current_page)
 {
 case 0:
 {
     QString title=ui->tabWidget->tabText(current_page);
     QString m_hour=QString::number(hour,10);

     //========================================
     if (exce_1==2||exce_1==0)//重复执行,
     {
         QString fileName_s=QDir::currentPath()+"/" + title +"_" + m_hour +".txt";
         if(QFile::exists(fileName_s)) //若存在目标文件
             QFile::remove(fileName_s);//删除文件夹
         exce_1=1;
     }

     //===================================
     QString fileName=QDir::currentPath()+"/" + title +"_" + m_hour +".txt";
     QFile file_1(fileName);
     if ((hour-pre_hour)!=0)
     {
         if (!file_1.open(QIODevice::WriteOnly | QIODevice::Text))
             return;
         timedata=(qRound(timedata*10.0))/10.0;//========================================
         QTextStream out_1(&file_1);
         out_1.setFieldWidth(8);
         out_1<<"\r\n";
         out_1<<"    time       valid-volt       error-volt    current    loop\r\n";
         out_1<<"================================================\r\n";
         out_1<<timedata<<" "<<voltdata<<" "<<voltdata2<<" "<<current_valid<<" "<<loopcount;//============================================
         pre_hour=hour;
         file_1.close();
     }
     else
     {
         if (!file_1.open(QIODevice::WriteOnly | QIODevice::Append))
             return;
         timedata=(qRound(timedata*10.0))/10.0;//========================================
         QTextStream out_1(&file_1);
         out_1.setFieldWidth(8);
         out_1<<"\r\n";
         out_1<<timedata<<" "<<voltdata<<" "<<voltdata2<<" "<<current_valid<<" "<<loopcount;

         file_1.close();
     }
change_page=current_page;//===================================================
     break;
 }
 case 1:
 {
     QString title=ui->tabWidget->tabText(current_page);
     QString m_hour=QString::number(hour,10);
     if (exce_2==2||exce_2==0)//重复执行或初次执行,
     {
         QString fileName_s=QDir::currentPath()+"/" + title +"_" + m_hour +".txt";
         if(QFile::exists(fileName_s)) //若存在目标文件
             QFile::remove(fileName_s);//删除文件夹
         exce_2=1;
     }
     QString fileName2=QDir::currentPath()+"/" + title +"_" + m_hour +".txt";
     QFile file_2(fileName2);
     if ((hour-pre_hour)!=0)
     {
         if (!file_2.open(QIODevice::WriteOnly | QIODevice::Text))
             return;
         timedata=(qRound(timedata*10.0))/10.0;//========================================
         QTextStream out_2(&file_2);
         out_2.setFieldWidth(8);
         out_2<<"\r\n";
         out_2<<"    time       valid-volt      error-volt     current     loop\r\n";
         out_2<<"================================================\r\n";
         out_2<<timedata<<" "<<voltdata<<" "<<voltdata2<<" "<<current_valid<<" "<<loopcount;//第一次时零数据已经进来，必须先存一下===============
         pre_hour=hour;
         file_2.close();
     }
     else
     {
         if (!file_2.open(QIODevice::WriteOnly | QIODevice::Append))
                     return;
         timedata=(qRound(timedata*10.0))/10.0;//存的应该是0.1s开始的数据==================================
         QTextStream out_2(&file_2);
         out_2.setFieldWidth(8);

         out_2<<"\r\n";
         out_2<<timedata<<"\n"<<voltdata<<"\n"<<voltdata2<<"\n"<<current_valid<<"\n"<<loopcount;
         file_2.close();
     }
change_page=current_page;
     break;
 }
 case 2:
 {
     QString title=ui->tabWidget->tabText(current_page);
     QString m_hour=QString::number(hour,10);
     if (exce_3==2||exce_3==0)//重复执行或初次执行,
     {
         QString fileName_s=QDir::currentPath()+"/" + title +"_" + m_hour +".txt";
         if(QFile::exists(fileName_s)) //若存在目标文件
             QFile::remove(fileName_s);//删除文件夹
         exce_3=1;
     }
     QString fileName3=QDir::currentPath()+"/" + title +"_" + m_hour +".txt";
     QFile file_3(fileName3);
     if ((hour-pre_hour)!=0)
     {
         if (!file_3.open(QIODevice::WriteOnly | QIODevice::Text))
             return;
         timedata=(qRound(timedata*10.0))/10.0;//========================================
         QTextStream out_3(&file_3);
         out_3.setFieldWidth(8);
         out_3<<"\r\n";
         out_3<<"    time       valid-volt      error-volt     cuurent     loop\r\n";
         out_3<<"=================================================\r\n";
         out_3<<timedata<<" "<<voltdata<<" "<<voltdata2<<" "<<current_valid<<" "<<loopcount;
         pre_hour=hour;
         file_3.close();
     }
     else
     {
         if (!file_3.open(QIODevice::WriteOnly | QIODevice::Append))
                     return;
         timedata=(qRound(timedata*10.0))/10.0;//========================================
         QTextStream out_3(&file_3);
         out_3.setFieldWidth(8);
         out_3<<"\r\n";
         out_3<<timedata<<"\n"<<voltdata<<"\n"<<voltdata2<<"\n"<<current_valid<<"\n"<<loopcount;
         file_3.close();
     }
     //pre_hour=1;
change_page=current_page;
     break;
 }
 case 3:
 {
     QString title=ui->tabWidget->tabText(current_page);
     QString m_hour=QString::number(hour,10);
     if (exce_4==2||exce_4==0)//同理,
     {
         QString fileName_s=QDir::currentPath()+"/" + title +"_" + m_hour +".txt";
         if(QFile::exists(fileName_s)) //若存在目标文件
             QFile::remove(fileName_s);//删除文件夹
         exce_4=1;
     }
     QString fileName4=QDir::currentPath()+"/" + title +"_" + m_hour +".txt";
     QFile file_4(fileName4);
     if ((hour-pre_hour)!=0)
     {
         if (!file_4.open(QIODevice::WriteOnly | QIODevice::Text))
             return;
         timedata=(qRound(timedata*10.0))/10.0;//========================================
         QTextStream out_4(&file_4);
         out_4.setFieldWidth(8);
         out_4<<"\r\n";
         out_4<<"    time       valid-volt      error-volt     cuurent    loop\r\n";
         out_4<<"================================================\r\n";
         out_4<<timedata<<" "<<voltdata<<" "<<voltdata2<<" "<<current_valid<<" "<<loopcount;
         pre_hour=hour;
         file_4.close();
     }
     else
     {
         if (!file_4.open(QIODevice::WriteOnly | QIODevice::Append))
                     return;
         timedata=(qRound(timedata*10.0))/10.0;//========================================
         QTextStream out_4(&file_4);
         out_4.setFieldWidth(8);
         out_4<<"\r\n";
         out_4<<timedata<<"\n"<<voltdata<<"\n"<<voltdata2<<"\n"<<current_valid<<"\n"<<loopcount;
         file_4.close();
     }
//     pre_hour=1;
change_page=current_page;
     break;
 }
 case 4:
 {
     QString title=ui->tabWidget->tabText(current_page);
     QString m_hour=QString::number(hour,10);
     if (exce_5==2||exce_5==0)//同理,
     {
         QString fileName_s=QDir::currentPath()+"/" + title +"_" + m_hour +".txt";
         if(QFile::exists(fileName_s)) //若存在目标文件
             QFile::remove(fileName_s);//删除文件夹
         exce_5=1;
     }

     QString fileName5=QDir::currentPath()+"/" + title +"_" + m_hour +".txt";
     QFile file_5(fileName5);
     if ((hour-pre_hour)!=0)
     {
         if (!file_5.open(QIODevice::WriteOnly | QIODevice::Text))
             return;
         timedata=(qRound(timedata*10.0))/10.0;//========================================
         QTextStream out_5(&file_5);
         out_5.setFieldWidth(8);
         out_5<<"\r\n";
         out_5<<"    time       valid-volt      error-volt     current    loop\r\n";
         out_5<<"================================================\r\n";
         out_5<<timedata<<" "<<voltdata<<" "<<voltdata2<<" "<<current_valid<<" "<<loopcount;
         pre_hour=hour;
         file_5.close();
     }
     else
     {
         if (!file_5.open(QIODevice::WriteOnly | QIODevice::Append))
                     return;
         timedata=(qRound(timedata*10.0))/10.0;//========================================
         QTextStream out_5(&file_5);
         out_5.setFieldWidth(8);
         out_5<<"\r\n";
         out_5<<timedata<<"\n"<<voltdata<<"\n"<<voltdata2<<"\n"<<current_valid<<"\n"<<loopcount;

         file_5.close();
     }
  //   pre_hour=1;
change_page=current_page;
     break;
 }
 default:
     break;
 }
}
void MainWindow::on_download_clicked()    //download按钮功能
{
    if(local_state!=AUTO)
    {
        QModelIndex indextemp;
        float temp;
        int i=0;
        int j=0;
        for( i = 0;i<ui->tableView->model()->rowCount();i++)
        {
            for( j=0;j<3;j++)
            {
                indextemp = ui->tableView->model()->index(i,j);
                temp=ui->tableView->model()->data(indextemp).toFloat();
                datatemp[i][j]=temp;
            }
            if((datatemp[i][0]==-1)&&(datatemp[i][1]==-1)&&(datatemp[i][2]==-1))
            {
                data_temp[0].down_valid_row_num=i;  //下传数据有效行数
               //valid_row_number=i;
                break;
            }
        }
        char ctemp[0];
        if(!serial->isOpen())
        {
            new_win->serial_check();
            new_win->show();
        }
        else            //串口发送有效数据
        {
            sprintf(&ctemp[0],"%d;",data_temp[0].down_valid_row_num);
            serial->write(&ctemp[0]);
            for(i=0;i<data_temp[0].down_valid_row_num;i++)
            {
               data_temp[0].last_time[i]=datatemp[i][0];
               sprintf(&ctemp[0],"%d;",(int)(datatemp[i][0]*1000));//==========================
               serial->write(&ctemp[0]);
               data_temp[0].volt[i]=datatemp[i][1];
               sprintf(&ctemp[0],"%d;",(int)(datatemp[i][1]*1000));//=====================
               serial->write(&ctemp[0]);
               data_temp[0].loop_times[i]=datatemp[i][2];
               sprintf(&ctemp[0],"%d;",(int)(datatemp[i][2]));
               serial->write(&ctemp[0]);
            }
            serial_end();//发送给单片机的终止符

            graph_para[0].state=INITIAL;
            graph_para[0].gra_scan_row=0;  //扫描行数回到开头
            graph_para[0].gra_node_num=0;     //图中节点数
            graph_para[0].final_x=0;          //最后一点坐标
            graph_para[0].gra_loop_times=0;  //记录各循环节点剩余次数
            graph_para[0].gra_scan_row=0;            //图像扫描的行数
            memset(graph_para[0].gra_last_time,0,100*sizeof(float));  //准备显示电压持续时间
            memset(graph_para[0].gra_volt,0,100*sizeof(float));  //准备电压值
            graph_para[0].gra_loop_head_row=0;   //循环的开头行数
            design_curve_t->setVisible(true);
            //清空实际图像
            graph_para[1].state=RESET;
            graph_down_replot();
            //设计图形初始化完成
            graph_para[0].state=DOWN_FINISH;

            //复原坐标轴
            ui->data_plot->setAxisAutoScale(QwtPlot::xBottom, false);
            ui->data_plot->setAxisScale(QwtPlot::xBottom,0,10,1);
            ui->data_plot->replot();
            ui->error_plot->setAxisAutoScale(QwtPlot::xBottom, false);
            ui->error_plot->setAxisScale(QwtPlot::xBottom,0,10,1);
            ui->error_plot->replot();
            //补充中间参考线长度
            QVector<QPointF> rule_a;
            for(int i =0;i<101;i++){
                QPointF point;
                point.setX(i);
                int y = 0;
                point.setY(y);
                rule_a.append(point);
            }
            QwtPointSeriesData* rule_r = new QwtPointSeriesData(rule_a);
            rule_t->setData(rule_r);
            ui->data_plot->replot();
            QVector<QPointF> rule_error_a;
            for(int i =0;i<101;i++){
                QPointF point;
                point.setX(i);
                int y = 0;
                point.setY(y);
                rule_error_a.append(point);
            }
            QwtPointSeriesData* rule_error_r = new QwtPointSeriesData(rule_error_a);
            rule_error_t->setData(rule_error_r);
            ui->error_plot->replot();
            //隐藏实际曲线
            valid_curve_t->setVisible(false);
            error_curve_t->setVisible(false);
            ui->data_plot->replot();
        }
    }

   //

}

void MainWindow::on_actionExit_triggered()      //退出菜单
{
    if(serial->isOpen())
    {
        serial->clear();
        serial->close();
        serial->deleteLater();
    }
    QApplication::quit();
}

void MainWindow::on_actionSetting_triggered()
{

    if(!serial->isOpen())
    {
        new_win->serial_check();
        new_win->show();
    }
    else
    {
        new_win->show();
    }
}

QByteArray  intToByte(int i)
{
    QByteArray abyte0;
    abyte0.resize(4);
    abyte0[0] = (uchar)  (0x000000ff & i);
    abyte0[1] = (uchar) ((0x0000ff00 & i) >> 8);
    abyte0[2] = (uchar) ((0x00ff0000 & i) >> 16);
    abyte0[3] = (uchar) ((0xff000000 & i) >> 24);
    return abyte0;
}

void MainWindow::save_slot()
{
    ui->tabWidget->setTabText(current_page,*tab_name);
    switch (current_page)
    {
    case 0:
        on_save_clicked_after();
        break;
    case 1:
        on_save_2_clicked_after();
        break;
    case 2:
        on_save_3_clicked_after();
        break;
    case 3:
        on_save_4_clicked_after();
        break;
    case 4:
        on_save_5_clicked_after();
        break;
    default:
        break;
    }

    //save_name_finish=true;
}
void MainWindow::on_save_clicked()
{
    if (local_state!=SAVE_ALL&&local_state!=BACKUP_ALL)//==============================
    {
       // QString s1;
        //*s1=*tab_name;//===============================================
        //if (ui->tabWidget->tabText(0)=="Tab 1")//===============================
        //{//

       //while(save_name_finish==false)
      // {
        //name_dialog->
           name_dialog->show();
      // }
    }
}

void MainWindow::on_save_clicked_after()      //保存操作函数
{

    //backup操作
        if(local_state==BACKUP_ALL)
        {
            bool ok=dataBackup();
             qDebug()<<ok;
        }


    //save操作
        float temp;//======================================
        QModelIndex indextemp;
        if(local_state==IDLE||local_state==SAVE_ALL)
        {
            QFile file("save.txt");
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                return;
            for(int i=0;i<ui->tableView->model()->rowCount();i++)
            {
                for(int j=0;j<3;j++)
                {
                    indextemp = ui->tableView->model()->index(i,j);
                    //datatemp[i][j] = ui->tableView->model()->data(indextemp).toInt();
                    temp=ui->tableView->model()->data(indextemp).toFloat();
                    datatemp[i][j]=temp;
                   // out << datatemp[i][j] << " ";
                }
                if((datatemp[i][0]==-1)&&(datatemp[i][1]==-1)&&(datatemp[i][2]==-1))
                {
                    data_temp[0].save_valid_row_num=i;   //保存数据的有效行数
                    break;
                }
               // out<<"\n";
            }
            //============================================================
            //memset(data_temp[0].last_time,0,100*sizeof(float));
            //memset(data_temp[0].volt,0,100*sizeof(float));
            //memset(data_temp[0].loop_times,0,100*sizeof(int));
            //==================================================

            //==================================================================
            QTextStream out(&file);
            out.setFieldWidth(8);
            out<<data_temp[0].save_valid_row_num;
            out<<"\n";
            out << ui->tabWidget->tabText(0);//=============================================
            out << "\n";//==================================================================
            out<<"    time      volt      loop\n";
            out<<"====================\n";

            for(int i=0;i<data_temp[0].save_valid_row_num;i++)
            {
                for(int j=0;j<3;j++)
                {
                  out << datatemp[i][j] << " ";
                }
                out<<"\n";
            }
    //        out << s1;//==================================================================
            file.close();
        }
        //memset(datatemp,0,sizeof(float)*100*3);
        //memset(datatemp[][1],0,100*sizeof(float));
        //memset(datatemp[][2],0,100*sizeof(float));
        test_text();
        on_load_clicked();

}

void MainWindow::on_load_clicked()//load槽函数
{
//先执行clear
    on_clear_clicked();

//读取save数据
    if(local_state==IDLE||local_state==LOAD_ALL||local_state==SAVE_ALL)
    {
        int is=0;
        int js=0;
        std::ifstream file_t("save.txt");
        std::string p;
        std::string q;
        QString name;
        file_t>>data_temp[0].load_valid_row_num;   //保存数据的有效行数;
        file_t>>q;//读取本行字放进q中，字属于string类
        name=QString::fromStdString(q);//转换为QString类型，方便下面写入
        ui->tabWidget->setTabText(0,name);//load时成功写入名字
        std::getline(file_t,p);
        std::getline(file_t,p);
        std::getline(file_t,p);
        //std::getline(file_t,q);//=================================
        while(!file_t.eof())        //按行读取
        {
            file_t>>datatemp[is][js++];
            file_t>>datatemp[is][js++];
            file_t>>datatemp[is++][js];
            js=0;
        }
        //ui->tabWidget->setTabText(0,q);//=================================
        file_t.close();
        QModelIndex indextemp;
        for(int i=0;i<data_temp[0].load_valid_row_num+1;i++)        //读取文档中的数据显示在表格中
        {
            for(int j=0;j<3;j++)
            {
                indextemp = ui->tableView->model()->index(i,j);
                ui->tableView->model()->setData(indextemp,qRound(datatemp[i][j]*1000.0)/1000.0);
                if(i == data_temp[0].load_valid_row_num)
                    ui->tableView->model()->setData(indextemp,-1.0);
            }

        }

    }else if(local_state==RESTORE_ALL||local_state==BACKUP_ALL)
    {
    //读取save_backup数据
        int is_back=0;
        int js_back=0;
        std::ifstream file_t_back("save_backup.txt");
        std::string p;
        std::string q;
        QString name;
        file_t_back>>data_temp[0].load_valid_row_num;   //保存数据的有效行数;
        file_t_back>>q;//读取本行字放进q中，字属于string类
        name=QString::fromStdString(q);//转换为QString类型，方便下面写入=============================
        ui->tabWidget->setTabText(0,name);//load时成功写入名字
        std::getline(file_t_back,p);
        std::getline(file_t_back,p);
        std::getline(file_t_back,p);
        while(!file_t_back.eof())        //按行读取
        {
            file_t_back>>datatemp[is_back][js_back++];
            file_t_back>>datatemp[is_back][js_back++];
            file_t_back>>datatemp[is_back++][js_back];
            js_back=0;
        }
        file_t_back.close();
        QModelIndex indextemp_back;
        for(int i=0;i<data_temp[0].load_valid_row_num+1;i++)        //读取文档中的数据显示在表格中
        {
            for(int j=0;j<3;j++)
            {
                indextemp_back = ui->tableView->model()->index(i,j);
                ui->tableView->model()->setData(indextemp_back,qRound(datatemp[i][j]*1000.0)/1000.0);
                if(i == data_temp[0].load_valid_row_num)
                    ui->tableView->model()->setData(indextemp_back,-1.0);
            }

        }
    }
    if(local_state!=AUTO)       //不在AUTO模式下，允许画图
    {
        design_curve_t->setVisible(true);
        //绘制图像
        QModelIndex indextemp_1;
        float temp;
        int i=0;
        int j=0;
        for( i = 0;i<ui->tableView->model()->rowCount();i++)
        {
            for( j=0;j<3;j++)
            {
                indextemp_1 = ui->tableView->model()->index(i,j);
                temp=ui->tableView->model()->data(indextemp_1).toFloat();
                datatemp[i][j]=temp;
            }
            if((datatemp[i][0]==-1)&&(datatemp[i][1]==-1)&&(datatemp[i][2]==-1))
            {
                data_temp[0].down_valid_row_num=i;  //下传数据有效行数
                break;
            }
        }
        for(i=0;i<data_temp[0].down_valid_row_num;i++)
        {
           data_temp[0].last_time[i]=datatemp[i][0];
           data_temp[0].volt[i]=datatemp[i][1];
           data_temp[0].loop_times[i]=datatemp[i][2];
        }
        graph_para[0].state=INITIAL;
        graph_para[0].gra_scan_row=0;  //扫描行数回到开头
        graph_para[0].gra_node_num=0;     //图中节点数
        graph_para[0].final_x=0;          //最后一点坐标
        graph_para[0].gra_loop_times=0;  //记录各循环节点剩余次数
        graph_para[0].gra_scan_row=0;            //图像扫描的行数
        memset(graph_para[0].gra_last_time,0,100*sizeof(float));  //准备显示电压持续时间
        memset(graph_para[0].gra_volt,0,100*sizeof(float));  //准备电压值
        graph_para[0].gra_loop_head_row=0;   //循环的开头行数
        //隐藏实际曲线
        //valid_curve_t->setPen(QColor(0,0,0),2,Qt::SolidLine);
        valid_curve_t->setVisible(false);
        ui->data_plot->replot();
        //清空实际图像
        graph_para[1].state=RESET;
        graph_down_replot();
        //load时显示最后一个坐标，变换坐标轴
        if(graph_para[0].final_x>10)
        {
            ui->data_plot->setAxisAutoScale(QwtPlot::xBottom, false);
            ui->data_plot->setAxisScale(QwtPlot::xBottom,0,graph_para[0].final_x,graph_para[0].final_x/10);
            ui->data_plot->replot();
        }
        else
        {
            ui->data_plot->setAxisAutoScale(QwtPlot::xBottom, false);
            ui->data_plot->setAxisScale(QwtPlot::xBottom,0,100,10);
            ui->data_plot->replot();
        }
        //补充中间参考线长度
        QVector<QPointF> rule_a;
        if(graph_para[0].final_x>100)
        {
            for(int i =0;i<(int)graph_para[0].final_x+100;i++){
                QPointF point;
                point.setX(i);
                int y = 0;
                point.setY(y);
                rule_a.append(point);
            }
        }
        else
        {
            for(int i =0;i<100;i++){
                QPointF point;
                point.setX(i);
                int y = 0;
                point.setY(y);
                rule_a.append(point);
            }
        }
        QwtPointSeriesData* rule_r = new QwtPointSeriesData(rule_a);
        rule_t->setData(rule_r);
        ui->data_plot->replot();
    }

}

//定时器事件
void MainWindow::timerEvent( QTimerEvent * ) {

    //显示串口设置界面
    if(new_win_initial==OFF)
    {
       new_win->show();
       new_win_initial=1;
    }else
        this->killTimer(timer_main_ID);
    //rule_extend();
    //graph_down_replot();        //绘制设计图

}

void MainWindow::graph_down_replot(){               //绘制图像
//准备数据
    int j;
    int j_1;
    if(graph_para[0].state==1)//第一张图数据准备
    {
        for(j=0;j<100;j++)
        {
            if(graph_para[0].gra_scan_row==data_temp[0].down_valid_row_num)//数据读完
            {
                if(graph_para[0].gra_loop_times>0)             //若最后一行为循环头
                {
                    if(graph_para[0].gra_loop_times!=9999)
                        graph_para[0].gra_loop_times--;
                    if(graph_para[0].gra_loop_times!=0) //避免非9999的循环体无限循环
                    {
                        graph_para[0]. gra_scan_row =   graph_para[0].gra_loop_head_row;   //扫描行数回到循环头的行数
                        graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                        graph_para[0].gra_last_time[j]=graph_para[0].final_x;
                        graph_para[0].gra_volt[j]=data_temp[0].volt[graph_para[0].gra_scan_row];
                        graph_para[0]. gra_scan_row++;
                    }
                    else
                    {
                        graph_para[0].gra_node_num=j;   //记录点数
                        graph_para[0].state=0;
                        break;
                    }
                }
                else                //若最后一行为正常数据
                {
                    graph_para[0].gra_node_num=j;   //记录点数
                    graph_para[0].state=0;
                    break;
                }
            }
            else
            {
                if(data_temp[0].loop_times[graph_para[0].gra_scan_row]==0)  //非循环数据，直接存储,扫描行数+1
                {
                   graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                   graph_para[0].gra_last_time[j]=graph_para[0].final_x;
                   graph_para[0].gra_volt[j]=data_temp[0].volt[graph_para[0].gra_scan_row];
                   graph_para[0]. gra_scan_row++;
                   if(graph_para[0].gra_loop_times==0)     //循环结束，记录循环次数
                   {
                       graph_para[0].gra_loop_times=data_temp[0].loop_times[graph_para[0].gra_scan_row];
                   }
                }
                else if(data_temp[0].loop_times[graph_para[0].gra_scan_row] != -1)//循环数据头，存储数据,记录循环次数
                {
                    graph_para[0].gra_loop_head_row=graph_para[0].gra_scan_row;//记录循环体头的行数
                    if(graph_para[0].gra_loop_times==0)
                        graph_para[0].gra_loop_times=data_temp[0].loop_times[graph_para[0].gra_scan_row];
                    graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                    graph_para[0].gra_last_time[j]=graph_para[0].final_x;
                    graph_para[0].gra_volt[j]=data_temp[0].volt[graph_para[0].gra_scan_row];
                    graph_para[0]. gra_scan_row++;
                }else if(data_temp[0].loop_times[graph_para[0].gra_scan_row] == -1)     //循环数据，记录数据，行数+1
                {
                    graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                    graph_para[0].gra_last_time[j]=graph_para[0].final_x;
                    graph_para[0].gra_volt[j]=data_temp[0].volt[graph_para[0].gra_scan_row];
                    graph_para[0]. gra_scan_row++;
                    if(graph_para[0].gra_scan_row==data_temp[0].down_valid_row_num)  //如果到了文件末尾
                    {
                        if(graph_para[0].gra_loop_times!=0) //循环未结束，记录循环次数
                        {
                            if(graph_para[0].gra_loop_times!=9999)
                                graph_para[0].gra_loop_times--;
                            if(graph_para[0].gra_loop_times==0)
                            {
                                graph_para[0].gra_node_num=j+1;   //记录点数
                               // graph_para[0].initial=1;
                                break;
                            }
                            graph_para[0]. gra_scan_row =   graph_para[0].gra_loop_head_row;   //扫描行数回到循环头的行数
                        }
                        else //如果是最后一行，且循环结束，那么就跳出
                        {
                            graph_para[0].gra_node_num=j+1;   //记录点数
                           //graph_para[0].initial=1;
                            break;
                        }
                    }
                    else if(data_temp[0].loop_times[graph_para[0].gra_scan_row] !=-1)    //执行到循环最后一行
                    {
                        if(graph_para[0].gra_loop_times!=0) //循环未结束，记录循环次数
                        {
                            graph_para[0].gra_loop_times--;
                            if(graph_para[0].gra_loop_times!=0)
                                graph_para[0]. gra_scan_row =   graph_para[0].gra_loop_head_row;   //扫描行数回到循环头的行数
                        }
                    }
                    else            //如果仍然是-1，判断是否是文件结尾
                    {
                        if(graph_para[0].gra_scan_row==data_temp[0].down_valid_row_num)
                        {
                            graph_para[0].gra_node_num=j+1;   //记录点数
                           // graph_para[0].state=0;
                            break;
                        }
                    }

                }
                else//错误数据，之后需补充检测数据输入的函数，错误报告异常
                {
                    ;
                }
            }
            graph_para[0].gra_node_num++;//正常循环个数+1
        }
        //自动开启刷新未写完的数据
//        if(graph_para[0].gra_node_num==100)
//            graph_para[0].state=2;
         graph_para[0].state=INITIAL_FINISH;//第一次刷新完成
       //  graph_para[1].state=1;//允许接受单片机数据
    }else       //后续点移入移出
    {
        if(graph_para[0].gra_scan_row==data_temp[0].down_valid_row_num)//数据读完
        {
            if(graph_para[0].gra_loop_times!=0)             //若最后一行为循环头
            {
                if(graph_para[0].state==2)
                {
                    for(j_1=0;j_1<99;j_1++){
                        graph_para[0].gra_last_time[j_1]=graph_para[0].gra_last_time[j_1+1];
                        graph_para[0].gra_volt[j_1]=graph_para[0].gra_volt[j_1+1];
                    }
                    if(graph_para[0].gra_loop_times!=9999)
                    {
                        graph_para[0].gra_loop_times--;
                    }
                    if(graph_para[0].gra_loop_times!=0)
                    {
                        graph_para[0]. gra_scan_row =   graph_para[0].gra_loop_head_row;   //扫描行数回到循环头的行数
                        graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                        graph_para[0].gra_last_time[99]=graph_para[0].final_x;
                        graph_para[0].gra_volt[99]=data_temp[0].volt[graph_para[0].gra_scan_row];
                        graph_para[0].gra_scan_row++;
                    }
                }
            }
            else                //若最后一行为正常数据
            {
                graph_para[0].state=0;
            }
        }
        else if(graph_para[0].state==2)          //继续执行
        {
            for(j_1=0;j_1<99;j_1++){
                graph_para[0].gra_last_time[j_1]=graph_para[0].gra_last_time[j_1+1];
                graph_para[0].gra_volt[j_1]=graph_para[0].gra_volt[j_1+1];
            }
            if(data_temp[0].loop_times[graph_para[0].gra_scan_row]==0)  //非循环数据，直接存储,扫描行数+1
            {
               graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
               graph_para[0].gra_last_time[99]=graph_para[0].final_x;
               graph_para[0].gra_volt[99]=data_temp[0].volt[graph_para[0].gra_scan_row];
               graph_para[0]. gra_scan_row++;
               if(graph_para[0].gra_loop_times==0)     //循环结束，记录循环次数
               {
                   graph_para[0].gra_loop_times=data_temp[0].loop_times[graph_para[0].gra_scan_row];
               }
            }
            else if(data_temp[0].loop_times[graph_para[0].gra_scan_row] != -1)//循环数据头，存储数据,记录循环次数
            {
                graph_para[0].gra_loop_head_row=graph_para[0].gra_scan_row;//记录循环体头的行数
                if(graph_para[0].gra_loop_times==0)
                    graph_para[0].gra_loop_times=data_temp[0].loop_times[graph_para[0].gra_scan_row];
                graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                graph_para[0].gra_last_time[99]=graph_para[0].final_x;
                graph_para[0].gra_volt[99]=data_temp[0].volt[graph_para[0].gra_scan_row];
                graph_para[0]. gra_scan_row++;
            }else if(data_temp[0].loop_times[graph_para[0].gra_scan_row] == -1)     //循环数据，记录数据，行数+1
            {
                graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                graph_para[0].gra_last_time[99]=graph_para[0].final_x;
                graph_para[0].gra_volt[99]=data_temp[0].volt[graph_para[0].gra_scan_row];
                graph_para[0]. gra_scan_row++;
                if(graph_para[0].gra_scan_row==data_temp[0].down_valid_row_num)  //如果文件结尾，跳出
                {
                    if(graph_para[0].gra_loop_times!=0) //循环未结束，记录循环次数
                    {
                        graph_para[0].gra_loop_times--;
                        if(graph_para[0].gra_loop_times!=0)
                        {
                            graph_para[0]. gra_scan_row =   graph_para[0].gra_loop_head_row;   //扫描行数回到循环头的行数
                        }
                    }
                }
                else if(data_temp[0].loop_times[graph_para[0].gra_scan_row] !=-1)    //执行到循环最后一行
                {
                    if(graph_para[0].gra_loop_times!=0) //循环未结束，记录循环次数
                    {
                        graph_para[0].gra_loop_times--;
                        if(graph_para[0].gra_loop_times!=0)
                            graph_para[0]. gra_scan_row =   graph_para[0].gra_loop_head_row;   //扫描行数回到循环头的行数
                    }
                }
            }
        }
        else if(graph_para[0].state==0)//无需刷新
        {
            ;
        }
        else if(graph_para[0].state==3)//初始化结束
        {
              graph_para[0].state=0;  //暂停刷新
        }
    }
//清零实际图像
    if(graph_para[1].state==RESET)
    {
    //清零实际图像
            QVector<QPointF> valid_curve;
            QVector<QPointF> error_curve;       //误差曲线
            QPointF point;
            QPointF point_error;
            for(int i =0;i<100;i++){
                   // graph_para[0].final_x += (float)graph_para[0].gra_last_time[i]/1000.0;
                    point.setX(i);
                    point.setY(0);
                    valid_curve.append(point);
            }
            for(int i =0;i<100;i++){//误差曲线
                   // graph_para[0].final_x += (float)graph_para[0].gra_last_time[i]/1000.0;
                    point_error.setX(i);
                    point_error.setY(0);
                    error_curve.append(point_error);
            }
            QwtPointSeriesData* valid_curve_r = new QwtPointSeriesData(valid_curve);
            QwtPointSeriesData* error_curve_r = new QwtPointSeriesData(error_curve);//误差曲线
            valid_curve_t->setData(valid_curve_r);
            error_curve_t->setData(error_curve_r);//误差曲线
            if(graph_para[1].gra_node_num==100)
            {
                ui->data_plot->setAxisAutoScale(QwtPlot::xBottom, false);
                ui->data_plot->setAxisScale(QwtPlot::xBottom,0,10,1);
                ui->data_plot->replot();
                ui->error_plot->setAxisAutoScale(QwtPlot::xBottom, false);//误差曲线
                ui->error_plot->setAxisScale(QwtPlot::xBottom,0,10,1);
                ui->error_plot->replot();
            }
            ui->data_plot->replot();
            ui->error_plot->replot();
            graph_para[1].state=STOP;
    }
//显示数据
    if(graph_para[0].state)
    {
        //rule_extend();
        QVector<QPointF> design_curve;
        QPointF point;
        //int cishu=0;//===============
        for(int i =0;i<graph_para[0].gra_node_num;i++){
                point.setX(float(graph_para[0].gra_last_time[i]));
                point.setY((float)graph_para[0].gra_volt[i]);
                //cishu=graph_para[0].gra_loop_times;//==================================
                design_curve.append(point);
        }
        QwtPointSeriesData* design_curve_r = new QwtPointSeriesData(design_curve);
        design_curve_t->setData(design_curve_r);
        ui->data_plot->replot();
        //graph_para[1].gra_loop_times=cishu;//====================================================
        //qDebug()<<design_curve.data()->x()<<""<<design_curve.data()->y();
    }

}

void MainWindow::ReadData()         //接受数据
{
    int t1=0;//时
    int t2=0;//分
    int t3=0;//秒==================================
    int current=0;
    int loop_save=0;
    if(graph_para[1].state)     //在程序执行期间先准备数据，再绘制图像
    {
        float valid_temp;//=========================================================================
        float error_temp;
        int loop_temp;
        int current_temp;

        QString valid;
        QString error;
        QString loops;
        QString currents;
//
        QByteArray buf;
        QString buf_s;
        buf = serial->readAll();
        buf_s=buf;
        QStringList parts = buf_s.split(";");
        if(parts.size()==5)
        {
            valid = parts.at(0);
            error= parts.at(1);
            loops=parts.at(2);
            currents=parts.at(3);
            //
            valid_temp=valid.toFloat();
            error_temp=error.toFloat();
            loop_temp=loops.toInt();
            current_temp=currents.toInt();
        }else           //数据出错，此时数据全部为0
        {
            valid_temp=0;
            error_temp=0;
            loop_temp=0;
            current_temp=0;
        }

        //========================================================
        //qDebug()<<valid_temp<<""<<error_temp;
        //int x_temp=0;
        if (current_temp>500)
            warning->show();
        //===========================================================
        if(graph_para[1].gra_node_num==100)     //graph1为实际波形，graph2为误差波形，这两幅图像同步出现同步消失
        {
            for(int i = 0;i<99;i++)
            {
                graph_para[1].gra_last_time[i]=graph_para[1].gra_last_time[i+1];
                graph_para[1].gra_volt[i]=graph_para[1].gra_volt[i+1];
                graph_para[2].gra_volt[i]=graph_para[2].gra_volt[i+1];  //给误差点赋值
            }
            graph_para[1].gra_volt[99]=valid_temp;
            graph_para[2].gra_volt[99]=error_temp;          //给误差点赋值
           //x_temp=(int)((int)()+1);

            graph_para[1].final_x=(qRound(graph_para[1].final_x*10))/10.0;
            graph_para[1].gra_last_time[99]=graph_para[1].final_x;
             graph_para[1].final_x+=0.1;
        }
        else
        {
            graph_para[1].gra_last_time[graph_para[1].gra_node_num]=graph_para[1].final_x;
            graph_para[1].gra_volt[graph_para[1].gra_node_num]=valid_temp;
            graph_para[2].gra_volt[graph_para[1].gra_node_num]=error_temp;//给误差点赋值
           // x_temp=(int)((int)(graph_para[1].final_x*10)+1);
            graph_para[1].final_x+=0.1;
            graph_para[1].final_x=(qRound(graph_para[1].final_x*10))/10.0;
            graph_para[1].gra_node_num++;
        }
        if(current_page==0)
        {
            valid_curve_t->setVisible(true);            //显示实际曲线
            error_curve_t->setVisible(true);            //显示误差曲线
            if(graph_para[1].final_x<(float)graph_para[0].final_x)          //设计波形的延伸
            {
                 if((float)(graph_para[0].final_x)-graph_para[1].final_x<1)//如果设计波形结尾和实际波形末尾差值小于1s，设计波形继续变化
                {
                    graph_para[0].state=2;
                    graph_down_replot();
                }
                 else
                    graph_para[0].state=0;
            }
            else
            {
                graph_para[0].state=2;
                graph_down_replot();
            }
    //绘制实际图像
            QVector<QPointF> valid_curve;
            QVector<QPointF> error_curve;//误差点
            QPointF point;//实际电压值点
            QPointF point_error;//误差点
            int i=0;
            //int loop_save=0;//当然是存储循环次数书啦========================================
            float shiji_t=0;
            float shiji_v=0;
            float wucha_v=0;
            for(i =0;i<graph_para[1].gra_node_num;i++){
                   // graph_para[0].final_x += (float)graph_para[0].gra_last_time[i]/1000.0;
                    point.setX(graph_para[1].gra_last_time[i]);
                    point_error.setX(graph_para[1].gra_last_time[i]);//误差曲线赋值
                    point.setY((float)graph_para[1].gra_volt[i]/1000.0);
                    point_error.setY((float)graph_para[2].gra_volt[i]/1000.0);//误差曲线赋值
                    valid_curve.append(point);
                    error_curve.append(point_error);//误差曲线赋值

                    t1=floor(graph_para[1].gra_last_time[i]/3600);
                    t2=floor((graph_para[1].gra_last_time[i]-t1*3600)/60);
                    t3=floor(graph_para[1].gra_last_time[i]-t1*3600-t2*60);//====================
                    shiji_v=(float)graph_para[1].gra_volt[i]/1000.0;
                    wucha_v=(float)graph_para[2].gra_volt[i]/1000.0;
                    shiji_t=(float)(graph_para[1].gra_last_time[i]);
                   // ui->lcdNumber_3->display(t3);
            }

            loop_save=loop_temp;
            current=current_temp;

          ui->lcdNumber->display(t1);
          ui->lcdNumber_2->display(t2);
          ui->lcdNumber_3->display(t3);
          ui->lcdNumber_18->display(shiji_v);
          ui->lcdNumber_19->display(wucha_v);
          ui->lcdNumber_16->display(current);
         //loop_save=graph_para[1].gra_loop_times;//=================
           //timedata=graph_para[1].gra_last_time[i];
           save_data(shiji_t,shiji_v,wucha_v,current,loop_save,t1);
           //=====================================================

            QwtPointSeriesData* error_curve_r = new QwtPointSeriesData(error_curve);//误差曲线
            QwtPointSeriesData* valid_curve_r = new QwtPointSeriesData(valid_curve);
            valid_curve_t->setData(valid_curve_r);
            error_curve_t->setData(error_curve_r);//误差曲线
            if(graph_para[1].gra_node_num==100)         //坐标轴变化
            {
                //data数据坐标轴变化
                ui->data_plot->setAxisAutoScale(QwtPlot::xBottom, false);
                ui->data_plot->setAxisScale(QwtPlot::xBottom,(float)graph_para[1].gra_last_time[0],(float)graph_para[1].gra_last_time[99],1);
                ui->data_plot->replot();
                //error数据坐标轴变化
                ui->error_plot->setAxisAutoScale(QwtPlot::xBottom, false);
                ui->error_plot->setAxisScale(QwtPlot::xBottom,(float)graph_para[1].gra_last_time[0],(float)graph_para[1].gra_last_time[99],1);
                ui->error_plot->replot();
            }
            ui->data_plot->replot();
            ui->error_plot->replot();
            //qDebug()<<buf;
            buf.clear();
            //save_data();
         }
        else if(current_page==1)
        {
            valid_curve_t_2->setVisible(true);            //显示实际曲线
            error_curve_t_2->setVisible(true);            //显示误差曲线
            if(graph_para[1].final_x<(float)graph_para[0].final_x)          //设计波形的延伸
            {
                 if((float)(graph_para[0].final_x)-graph_para[1].final_x<1)//如果设计波形结尾和实际波形末尾差值小于1s，设计波形继续变化
                {
                    graph_para[0].state=2;
                    graph_down_replot_2();
                }
                 else
                    graph_para[0].state=0;
            }
            else
            {
                graph_para[0].state=2;
                graph_down_replot_2();
            }
    //绘制实际图像
            QVector<QPointF> valid_curve;
            QVector<QPointF> error_curve;//误差点
            QPointF point;//实际电压值点
            QPointF point_error;//误差点
            //int loop_save=0;//========================================
            float shiji_t=0;
            float shiji_v=0;
            float wucha_v=0;
            for(int i =0;i<graph_para[1].gra_node_num;i++){
                   // graph_para[0].final_x += (float)graph_para[0].gra_last_time[i]/1000.0;
                    point.setX(graph_para[1].gra_last_time[i]);
                    point_error.setX(graph_para[1].gra_last_time[i]);//误差曲线赋值
                    point.setY((float)graph_para[1].gra_volt[i]/1000.0);
                    point_error.setY((float)graph_para[2].gra_volt[i]/1000.0);//误差曲线赋值
                    valid_curve.append(point);
                    error_curve.append(point_error);//误差曲线赋值

                    t1=floor(graph_para[1].gra_last_time[i]/3600);
                    t2=floor((graph_para[1].gra_last_time[i]-t1*3600)/60);
                    t3=floor(graph_para[1].gra_last_time[i]-t1*3600-t2*60);//====================
                    shiji_v=(float)graph_para[1].gra_volt[i]/1000.0;
                    wucha_v=(float)graph_para[2].gra_volt[i]/1000.0;
                    shiji_t=(float)(graph_para[1].gra_last_time[i]);
                   // ui->lcdNumber_3->display(t3);
            }
            loop_save=loop_temp;
            current=current_temp;

            ui->lcdNumber_6->display(t1);
            ui->lcdNumber_4->display(t2);
            ui->lcdNumber_5->display(t3);
            ui->lcdNumber_20->display(shiji_v);
            ui->lcdNumber_21->display(wucha_v);
            ui->lcdNumber_17->display(current);//=================
            //loop_save=graph_para[1].gra_loop_times;//=================
             //timedata=graph_para[1].gra_last_time[i];
             save_data(shiji_t,shiji_v,wucha_v,current,loop_save,t1);
             //=====================================================

            QwtPointSeriesData* error_curve_r = new QwtPointSeriesData(error_curve);//误差曲线
            QwtPointSeriesData* valid_curve_r = new QwtPointSeriesData(valid_curve);
            valid_curve_t_2->setData(valid_curve_r);
            error_curve_t_2->setData(error_curve_r);//误差曲线
            if(graph_para[1].gra_node_num==100)         //坐标轴变化
            {
                //data数据坐标轴变化
                ui->data_plot_2->setAxisAutoScale(QwtPlot::xBottom, false);
                ui->data_plot_2->setAxisScale(QwtPlot::xBottom,(float)graph_para[1].gra_last_time[0],(float)graph_para[1].gra_last_time[99],1);
                ui->data_plot_2->replot();
                //error数据坐标轴变化
                ui->error_plot_2->setAxisAutoScale(QwtPlot::xBottom, false);
                ui->error_plot_2->setAxisScale(QwtPlot::xBottom,(float)graph_para[1].gra_last_time[0],(float)graph_para[1].gra_last_time[99],1);
                ui->error_plot_2->replot();
            }
            ui->data_plot_2->replot();
            ui->error_plot_2->replot();
            //qDebug()<<buf;
            buf.clear();

        }
        else if(current_page==2)
        {
            valid_curve_t_3->setVisible(true);            //显示实际曲线
            error_curve_t_3->setVisible(true);            //显示误差曲线
            if(graph_para[1].final_x<(float)graph_para[0].final_x)          //设计波形的延伸
            {
                 if((float)(graph_para[0].final_x)-graph_para[1].final_x<1)//如果设计波形结尾和实际波形末尾差值小于1s，设计波形继续变化
                {
                    graph_para[0].state=2;
                    graph_down_replot_3();
                }
                 else
                    graph_para[0].state=0;
            }
            else
            {
                graph_para[0].state=2;
                graph_down_replot_3();
            }
    //绘制实际图像
            QVector<QPointF> valid_curve;
            QVector<QPointF> error_curve;//误差点
            QPointF point;//实际电压值点
            QPointF point_error;//误差点
            //int loop_save=0;//======================================
            float shiji_t=0;
            float shiji_v=0;
            float wucha_v=0;
            for(int i =0;i<graph_para[1].gra_node_num;i++){
                   // graph_para[0].final_x += (float)graph_para[0].gra_last_time[i]/1000.0;
                    point.setX(graph_para[1].gra_last_time[i]);
                    point_error.setX(graph_para[1].gra_last_time[i]);//误差曲线赋值
                    point.setY((float)graph_para[1].gra_volt[i]/1000.0);
                    point_error.setY((float)graph_para[2].gra_volt[i]/1000.0);//误差曲线赋值
                    valid_curve.append(point);
                    error_curve.append(point_error);//误差曲线赋值

                    t1=floor(graph_para[1].gra_last_time[i]/3600);
                    t2=floor((graph_para[1].gra_last_time[i]-t1*3600)/60);
                    t3=floor(graph_para[1].gra_last_time[i]-t1*3600-t2*60);//====================
                    shiji_v=(float)graph_para[1].gra_volt[i]/1000.0;
                    wucha_v=(float)graph_para[2].gra_volt[i]/1000.0;
                    shiji_t=(float)(graph_para[1].gra_last_time[i]);
                   // ui->lcdNumber_3->display(t3);
            }
            loop_save=loop_temp;
            current=current_temp;

            ui->lcdNumber_9->display(t1);
            ui->lcdNumber_7->display(t2);
            ui->lcdNumber_8->display(t3);
            ui->lcdNumber_22->display(shiji_v);
            ui->lcdNumber_23->display(wucha_v);
            ui->lcdNumber_28->display(current);//=================
           // loop_save=graph_para[1].gra_loop_times;//=================
             //timedata=graph_para[1].gra_last_time[i];
             save_data(shiji_t,shiji_v,wucha_v,current,loop_save,t1);
             //=====================================================

            QwtPointSeriesData* error_curve_r = new QwtPointSeriesData(error_curve);//误差曲线
            QwtPointSeriesData* valid_curve_r = new QwtPointSeriesData(valid_curve);
            valid_curve_t_3->setData(valid_curve_r);
            error_curve_t_3->setData(error_curve_r);//误差曲线
            if(graph_para[1].gra_node_num==100)         //坐标轴变化
            {
                //data数据坐标轴变化
                ui->data_plot_3->setAxisAutoScale(QwtPlot::xBottom, false);
                ui->data_plot_3->setAxisScale(QwtPlot::xBottom,(float)graph_para[1].gra_last_time[0],(float)graph_para[1].gra_last_time[99],1);
                ui->data_plot_3->replot();
                //error数据坐标轴变化
                ui->error_plot_3->setAxisAutoScale(QwtPlot::xBottom, false);
                ui->error_plot_3->setAxisScale(QwtPlot::xBottom,(float)graph_para[1].gra_last_time[0],(float)graph_para[1].gra_last_time[99],1);
                ui->error_plot_3->replot();
            }
            ui->data_plot_3->replot();
            ui->error_plot_3->replot();
            //qDebug()<<buf;
            buf.clear();

        }
        else if(current_page==3)
        {
            valid_curve_t_4->setVisible(true);            //显示实际曲线
            error_curve_t_4->setVisible(true);            //显示误差曲线
            if(graph_para[1].final_x<(float)graph_para[0].final_x)          //设计波形的延伸
            {
                 if((float)(graph_para[0].final_x)-graph_para[1].final_x<1)//如果设计波形结尾和实际波形末尾差值小于1s，设计波形继续变化
                {
                    graph_para[0].state=2;
                    graph_down_replot_4();
                }
                 else
                    graph_para[0].state=0;
            }
            else
            {
                graph_para[0].state=2;
                graph_down_replot_4();
            }
    //绘制实际图像
            QVector<QPointF> valid_curve;
            QVector<QPointF> error_curve;//误差点
            QPointF point;//实际电压值点
            QPointF point_error;//误差点
            float shiji_t=0;
            float shiji_v=0;
            float wucha_v=0;
            //int loop_save=0;//======================================
            for(int i =0;i<graph_para[1].gra_node_num;i++){
                   // graph_para[0].final_x += (float)graph_para[0].gra_last_time[i]/1000.0;
                    point.setX(graph_para[1].gra_last_time[i]);
                    point_error.setX(graph_para[1].gra_last_time[i]);//误差曲线赋值
                    point.setY((float)graph_para[1].gra_volt[i]/1000.0);
                    point_error.setY((float)graph_para[2].gra_volt[i]/1000.0);//误差曲线赋值
                    valid_curve.append(point);
                    error_curve.append(point_error);//误差曲线赋值

                    t1=floor(graph_para[1].gra_last_time[i]/3600);
                    t2=floor((graph_para[1].gra_last_time[i]-t1*3600)/60);
                    t3=floor(graph_para[1].gra_last_time[i]-t1*3600-t2*60);//====================
                    shiji_v=(float)graph_para[1].gra_volt[i]/1000.0;
                    wucha_v=(float)graph_para[2].gra_volt[i]/1000.0;
                    shiji_t=(float)(graph_para[1].gra_last_time[i]);
                   // ui->lcdNumber_3->display(t3);
            }
            loop_save=loop_temp;
            current=current_temp;

            ui->lcdNumber_10->display(t1);
            ui->lcdNumber_12->display(t2);
            ui->lcdNumber_11->display(t3);
            ui->lcdNumber_24->display(shiji_v);
            ui->lcdNumber_25->display(wucha_v);
            ui->lcdNumber_29->display(current);//=================
            //loop_save=graph_para[1].gra_loop_times;//=================
             //timedata=graph_para[1].gra_last_time[i];
             save_data(shiji_t,shiji_v,wucha_v,current,loop_save,t1);
             //=====================================================

            QwtPointSeriesData* error_curve_r = new QwtPointSeriesData(error_curve);//误差曲线
            QwtPointSeriesData* valid_curve_r = new QwtPointSeriesData(valid_curve);
            valid_curve_t_4->setData(valid_curve_r);
            error_curve_t_4->setData(error_curve_r);//误差曲线
            if(graph_para[1].gra_node_num==100)         //坐标轴变化
            {
                //data数据坐标轴变化
                ui->data_plot_4->setAxisAutoScale(QwtPlot::xBottom, false);
                ui->data_plot_4->setAxisScale(QwtPlot::xBottom,(float)graph_para[1].gra_last_time[0],(float)graph_para[1].gra_last_time[99],1);
                ui->data_plot_4->replot();
                //error数据坐标轴变化
                ui->error_plot_4->setAxisAutoScale(QwtPlot::xBottom, false);
                ui->error_plot_4->setAxisScale(QwtPlot::xBottom,(float)graph_para[1].gra_last_time[0],(float)graph_para[1].gra_last_time[99],1);
                ui->error_plot_4->replot();
            }
            ui->data_plot_4->replot();
            ui->error_plot_4->replot();
            //qDebug()<<buf;
            buf.clear();

        }
        else if(current_page==4)
        {
            valid_curve_t_5->setVisible(true);            //显示实际曲线
            error_curve_t_5->setVisible(true);            //显示误差曲线
            if(graph_para[1].final_x<(float)graph_para[0].final_x)          //设计波形的延伸
            {
                 if((float)(graph_para[0].final_x)-graph_para[1].final_x<1)//如果设计波形结尾和实际波形末尾差值小于1s，设计波形继续变化
                {
                    graph_para[0].state=2;
                    graph_down_replot_5();
                }
                 else
                    graph_para[0].state=0;
            }
            else
            {
                graph_para[0].state=2;
                graph_down_replot_5();
            }
    //绘制实际图像
            QVector<QPointF> valid_curve;
            QVector<QPointF> error_curve;//误差点
            QPointF point;//实际电压值点
            QPointF point_error;//误差点
            float shiji_t=0;
            float shiji_v=0;
            float wucha_v=0;
           // int loop_save=0;//==============================================================
            for(int i =0;i<graph_para[1].gra_node_num;i++){
                   // graph_para[0].final_x += (float)graph_para[0].gra_last_time[i]/1000.0;
                    point.setX(graph_para[1].gra_last_time[i]);
                    point_error.setX(graph_para[1].gra_last_time[i]);//误差曲线赋值
                    point.setY((float)graph_para[1].gra_volt[i]/1000.0);
                    point_error.setY((float)graph_para[2].gra_volt[i]/1000.0);//误差曲线赋值
                    valid_curve.append(point);
                    error_curve.append(point_error);//误差曲线赋值

                    t1=floor(graph_para[1].gra_last_time[i]/3600);
                    t2=floor((graph_para[1].gra_last_time[i]-t1*3600)/60);
                    t3=floor(graph_para[1].gra_last_time[i]-t1*3600-t2*60);//====================
                    shiji_v=(float)graph_para[1].gra_volt[i]/1000.0;
                    wucha_v=(float)graph_para[2].gra_volt[i]/1000.0;
                    shiji_t=(float)(graph_para[1].gra_last_time[i]);
                   // ui->lcdNumber_3->display(t3);
            }
            loop_save=loop_temp;
            current=current_temp;

            ui->lcdNumber_13->display(t1);
            ui->lcdNumber_15->display(t2);
            ui->lcdNumber_14->display(t3);
            ui->lcdNumber_27->display(shiji_v);
            ui->lcdNumber_26->display(wucha_v);
            ui->lcdNumber_30->display(current);//=================
           // loop_save=graph_para[1].gra_loop_times;//=================
             //timedata=graph_para[1].gra_last_time[i];
             save_data(shiji_t,shiji_v,wucha_v,current,loop_save,t1);
             //=====================================================

            QwtPointSeriesData* error_curve_r = new QwtPointSeriesData(error_curve);//误差曲线
            QwtPointSeriesData* valid_curve_r = new QwtPointSeriesData(valid_curve);
            valid_curve_t_5->setData(valid_curve_r);
            error_curve_t_5->setData(error_curve_r);//误差曲线
            if(graph_para[1].gra_node_num==100)         //坐标轴变化
            {
                //data数据坐标轴变化
                ui->data_plot_5->setAxisAutoScale(QwtPlot::xBottom, false);
                ui->data_plot_5->setAxisScale(QwtPlot::xBottom,(float)graph_para[1].gra_last_time[0],(float)graph_para[1].gra_last_time[99],1);
                ui->data_plot_5->replot();
                //error数据坐标轴变化
                ui->error_plot_5->setAxisAutoScale(QwtPlot::xBottom, false);
                ui->error_plot_5->setAxisScale(QwtPlot::xBottom,(float)graph_para[1].gra_last_time[0],(float)graph_para[1].gra_last_time[99],1);
                ui->error_plot_5->replot();
            }
            ui->data_plot_5->replot();
            ui->error_plot_5->replot();
            //qDebug()<<buf;
            buf.clear();

        }
        rule_extend();
     }
    else       //清空在未执行时得到的数据
        {
          QByteArray buf;
          buf = serial->readAll();
          buf.clear();
        }
}

void MainWindow::on_excute_clicked()          //tab1 执行按键功能
{
    //清空实际图像
    if(graph_para[0].state==DOWN_FINISH)     //download完成
    {
        graph_para[1].state=INITIAL;
        graph_para[1].gra_scan_row=0;  //扫描行数回到开头
        graph_para[1].gra_node_num=0;     //图中节点数
        graph_para[1].final_x=0;          //最后一点坐标
        graph_para[1].gra_loop_times=0;  //记录各循环节点剩余次数
        graph_para[1].gra_scan_row=0;            //图像扫描的行数
        memset(graph_para[1].gra_last_time,0,100*sizeof(float));  //准备显示电压持续时间
        memset(graph_para[1].gra_volt,0,100*sizeof(float));  //准备电压值
        graph_para[1].gra_loop_head_row=0;   //循环的开头行数

        if(!serial->isOpen())
        {
            new_win->serial_check();
            new_win->show();
        }
        else            //串口发送有效数据
        {
            serial->write("start");
            serial_end();
        }
    }
    //====================================================================
    pre_hour=1;
    //个人认为每一次点击运行都等同于换页，所有的数据都应当归于一开始。
   if (exce_1!=0)
   exce_1=exce_1+1;
  //exce_2=0;
   //exce_3=0;
  // exce_4=0;
   //exce_5=0;
}

void MainWindow::test_text()
{
    qDebug()<<"NO.1 chart:";
    QModelIndex indextemp_0;
    float temp;//=============================================
    int i=0;
    int j=0;
    for( i = 0;i<ui->tableView->model()->rowCount();i++)
    {
        for( j=0;j<3;j++)
        {
            indextemp_0 = ui->tableView->model()->index(i,j);
            temp=ui->tableView->model()->data(indextemp_0).toFloat();
            datatemp[i][j]=temp;
        }
        if((datatemp[i][0]==-1)&&(datatemp[i][1]==-1)&&(datatemp[i][2]==-1))
        {
            data_temp[0].down_valid_row_num=i;  //下传数据有效行数
            break;
        }
    }
    //

    int k=0;
    int flag=0;
    //第一个循环用来寻找第一个循环头
    for(int i=0;i<data_temp[0].down_valid_row_num;i++)
    {
        if (datatemp[i][2]<=-1)
        {
                qDebug()<<"there is a mistake:"<<i+1;
                flag=1;
                break;
        }
        else
        {
            if (datatemp[i][2]>0)
            {
                k=i;
                break;
            }
        }
    }

    //以下才是真正的判断循环体的正确性。当然，零的存在打断了部分循环。而且为了简化最后一个else导致复杂化了仲间的两个if判断
    for(i=k+1;i<data_temp[0].down_valid_row_num;i++)
    {
        if(flag) break;
        else
        {
            if (i==k+1)
            {
                if (datatemp[k][2]>0)
                {
                    if (datatemp[i][2]>=0||datatemp[i][2]<-1)
                    {
                        qDebug()<<"there is a mistake:"<<i+1;
                        break;
                    }
                    else if (i==data_temp[0].down_valid_row_num-1)
                    {
                        qDebug()<<"all is correct";
                    }
                }
                else
                {
                    for (i=k+1;i<data_temp[0].down_valid_row_num;i++)
                    {
                        if (datatemp[i][2]<=-1)
                        {
                                qDebug()<<"there is a mistake:"<<i+1;
                                flag=1;
                                break;
                        }
                        else if (datatemp[i][2]>0)
                        {
                            if (i!=data_temp[0].down_valid_row_num-1)
                            {
                                k=i;
                                break;
                            }
                            else
                            {
                                qDebug()<<"the last row is wrong and so pity";
                                flag=1;
                                break;
                            }
                        }
                        else if(datatemp[i][2]==0&&i==data_temp[0].down_valid_row_num-1)
                        qDebug()<<"all is correct";
                    }
                }

            }
            else if (i==data_temp[0].down_valid_row_num-1)
            {
                if (datatemp[i-1][2]==-1)
                {
                    if (datatemp[i][2]==-1||datatemp[i][2]==0) qDebug()<<"all is correct";
                    else
                    {
                        qDebug()<<"the last row is wrong and so pity";
                        break;
                    }
                }
                else if (datatemp[i-1][2]==0)
                {
                    if (datatemp[i][2]==0) qDebug()<<"all is correct";
                    else
                    {
                        qDebug()<<"the last row is wrong and so pity";
                        break;
                    }
                }
                else
                {
                    if (datatemp[i][2]==-1) qDebug()<<"all is correct";
                    else
                    {
                        qDebug()<<"the last row is wrong and so pity";
                        break;
                    }
                }
            }
            else
            {
                if(datatemp[i][2]<-1)
                {
                    qDebug()<<"there is a mistake:"<<i+1;
                    break;
                }
                else
                {
                    if(datatemp[i][2]!=-1)
                    {
                        k=i;
                        qDebug()<<"current circle is correct"<<i;
                    }
                }
            }
        }
    }
}

void MainWindow::serial_close(){
   // serial->close();
   // serial->clear();
}

void MainWindow::setpage(int index)
{
    current_page=index;  //得到当前页码
    local_state=IDLE;//翻页重置状态
    valid_curve_t->setVisible(false);            //隐藏实际曲线
    error_curve_t->setVisible(false);            //隐藏误差曲线
    valid_curve_t_2->setVisible(false);            //隐藏实际曲线
    error_curve_t_2->setVisible(false);            //隐藏误差曲线
    valid_curve_t_3->setVisible(false);            //隐藏实际曲线
    error_curve_t_3->setVisible(false);            //隐藏误差曲线
    valid_curve_t_4->setVisible(false);            //隐藏实际曲线
    error_curve_t_4->setVisible(false);            //隐藏误差曲线
    valid_curve_t_5->setVisible(false);            //隐藏实际曲线
    error_curve_t_5->setVisible(false);            //隐藏误差曲线
    ui->data_plot->replot();
    ui->error_plot->replot();
    ui->data_plot_2->replot();
    ui->error_plot_2->replot();
    ui->data_plot_3->replot();
    ui->error_plot_3->replot();
    ui->data_plot_4->replot();
    ui->error_plot_4->replot();
    ui->data_plot_5->replot();
    ui->error_plot_5->replot();
    stop_excute();
    if(ui->actionAuto->isChecked())       //翻页时判断如果处于自动状态，自动执行
    {
        switch(current_page){
         case 0://执行第一页的操作
            on_download_clicked();
            delaymsec(800);
            on_download_clicked();
            delaymsec(800);
            on_excute_clicked();
            break;
         case 1:
            on_download_2_clicked();
            delaymsec(800);
            on_download_2_clicked();
            delaymsec(800);
            on_excute_2_clicked();
            break;
         case 2:
            on_download_3_clicked();
            delaymsec(800);
            on_download_3_clicked();
            delaymsec(800);
            on_excute_3_clicked();
            break;
         case 3:
            on_download_4_clicked();
            delaymsec(800);
            on_download_4_clicked();
            delaymsec(800);
            on_excute_4_clicked();
            break;
         case 4:
            on_download_5_clicked();
            delaymsec(800);
            on_download_5_clicked();
            delaymsec(800);
            on_excute_5_clicked();
            break;
         default:
            break;
            }
        local_state=AUTO;           //AUTO模式下不允许手动download
    }else if(auto_state!=READY)
    {
        switch(current_page){
         case 0://执行第一页的操作
            on_download_clicked();
            break;
         case 1:
            on_download_2_clicked();
            break;
         case 2:
            on_download_3_clicked();
            break;
         case 3:
            on_download_4_clicked();
            break;
         case 4:
            on_download_5_clicked();
            break;
         default:
            break;
            }
        local_state=IDLE;
    }

}


void MainWindow::on_actionsave_all_triggered()
{
    local_state=SAVE_ALL;
//执行第一页的save

    on_save_clicked_after();
//执行第一页的save

    on_save_2_clicked_after();
//同理

    on_save_3_clicked_after();

    on_save_4_clicked_after();

    on_save_5_clicked_after();
    local_state=IDLE;  //执行结束 返回空闲
}

void MainWindow::on_actionRestore_All_triggered()
{
     local_state=RESTORE_ALL;
    //执行第一页的restore
     on_load_clicked();
     //执行第二页的restore
      on_load_2_clicked();
      //同理
      on_load_3_clicked();
      on_load_4_clicked();
      on_load_5_clicked();

     local_state=IDLE;
     auto_state=READY;   //auto状态准备完毕
}

void MainWindow::on_actionBackup_ALL_triggered()
{
     local_state=BACKUP_ALL;
    //执行backup但不执行save
     on_save_clicked_after();
     //执行backup但不执行save
      on_save_2_clicked_after();
     //同理
      on_save_3_clicked_after();
      on_save_4_clicked_after();
      on_save_5_clicked_after();

     local_state=IDLE;
}

void MainWindow::on_actionLoad_ALL_triggered()
{
    local_state=LOAD_ALL;
    //执行第一页的save
    on_load_clicked();
    //执行第二页的save
     on_load_2_clicked();
     //同理
     on_load_3_clicked();
     on_load_4_clicked();
     on_load_5_clicked();

    local_state=IDLE;
    auto_state=READY;   //auto状态准备完毕
}

void MainWindow::on_actionAuto_triggered()          //AUTO开关
{
    if(ui->actionAuto->isChecked())
    {
        if(!serial->isOpen())
        {
            new_win->show();
            ui->actionAuto->setChecked(false);
        }else
        {
            if(auto_state==READY)       //如果load_all完成，执行auto操作
            {
                switch(current_page){
                 case 0://执行第一页的操作
                    on_download_clicked();
                    delaymsec(800);             //必须延时
                    on_download_clicked();
                    delaymsec(800);             //必须延时
                    on_excute_clicked();
                    break;
                 case 1:
                    on_download_2_clicked();
                    delaymsec(800);
                    on_download_2_clicked();
                    delaymsec(800);
                    on_excute_2_clicked();
                    break;
                 case 2:
                    on_download_3_clicked();
                    delaymsec(800);
                    on_download_3_clicked();
                    delaymsec(800);
                    on_excute_3_clicked();
                    break;
                 case 3:
                    on_download_4_clicked();
                    delaymsec(800);
                    on_download_4_clicked();
                    delaymsec(800);
                    on_excute_4_clicked();
                    break;
                 case 4:
                    on_download_5_clicked();
                    delaymsec(800);
                    on_download_5_clicked();
                    delaymsec(800);
                    on_excute_5_clicked();
                 default:
                    break;
                }
                local_state=AUTO;   //执行完进入AUTO模式
            }

        }
    }else
    {
        local_state=IDLE;
        auto_state=OFF;
        graph_para[1].state=STOP;//停止接收数据
    }

}

void MainWindow::on_clear_clicked()
{
    auto_state = OFF;
    QModelIndex indextemp;
    for(int i=0;i<ui->tableView->model()->rowCount();i++)        //清零表格中的数据
    {
        for(int j=0;j<3;j++)
        {
            indextemp = ui->tableView->model()->index(i,j);
            ui->tableView->model()->setData(indextemp,0.0);
        }
    }
    memset(graph_para[0].gra_volt, 0, 100*sizeof(float) );
    memset(graph_para[1].gra_volt, 0, 100*sizeof(float) );//=======================================
    graph_para[0].state=INITIAL;
    graph_para[0].gra_scan_row=0;
    data_temp[0].down_valid_row_num=0;
    //修复bug，清除上一幅图像的数据和本地缓存
    memset(data_temp[0].loop_times,0,100*sizeof(int));

    //先清除曲线名
    ui->tabWidget->setTabText(0,"Tab 1");

    //清除LCDnumber
    ui->lcdNumber->display(0);
    ui->lcdNumber_2->display(0);
    ui->lcdNumber_3->display(0);
    ui->lcdNumber_18->display(0);
    ui->lcdNumber_19->display(0);
    ui->lcdNumber_16->display(0);//=================

    graph_para[1].state=RESET;
    graph_para[1].final_x=0;
    graph_down_replot();

    //数据图坐标更新回复
    ui->data_plot->setAxisScale(QwtPlot::yLeft,-5,5,1);
    ui->data_plot->setAxisScale(QwtPlot::xBottom,0,10,1);
    QVector<QPointF> rule;                   //数据参考线
    for(int i =-1;i<101;i++){
        QPointF point;
        point.setX(i);
        int y = 0;
        point.setY(y);
        rule.append(point);
    }
    QwtPointSeriesData* rule_c = new QwtPointSeriesData(rule);
    rule_t = new QwtPlotCurve("Curve 0");
    rule_t->setData(rule_c);
    rule_t->attach(ui->data_plot);

    QVector<QPointF> rule_error;             //误差参考线
    for(int i =-1;i<101;i++){
        QPointF point_error;
        point_error.setX(i);
        int y = 0;
        point_error.setY(y);
        rule_error.append(point_error);
    }
    QwtPointSeriesData* rule_error_c = new QwtPointSeriesData(rule_error);
    rule_error_t = new QwtPlotCurve("Curve 0");
    rule_error_t->setData(rule_error_c);
    rule_error_t->attach(ui->error_plot);

    valid_curve_t->setVisible(false);            //隐藏实际曲线
    error_curve_t->setVisible(false);            //隐藏误差曲线
    design_curve_t->setVisible(false);            //隐藏误差曲线
    QVector<QPointF> rule_a;
    QVector<QPointF> rule_error_a;//误差曲线
    for(int i =0;i<100;i++){
        QPointF point;
        QPointF point_error;            //误差曲线
        point.setX(i);
        point_error.setX(i);            //误差曲线
        point.setY(0);
        point_error.setY(0);            //误差曲线
        rule_a.append(point);
        rule_error_a.append(point_error);//误差曲线
    }
    QwtPointSeriesData* rule_r = new QwtPointSeriesData(rule_a);
    rule_t->setData(rule_r);
    QwtPointSeriesData* rule_error_r = new QwtPointSeriesData(rule_error_a);//误差曲线
    rule_error_t->setData(rule_error_r);//误差曲线
    ui->data_plot->replot();
    ui->error_plot->replot();
    stop_excute();


}

bool MainWindow::dataBackup()//===================================================
{
    if(current_page==0)
    {
        QString sourceFileName = QDir::currentPath()+"/save.txt";
        QString destFileName=QDir::currentPath()+"/save_backup.txt";
    //    创建备份文件夹
        QDir dir(QDir::currentPath());
        if(!dir.exists()){
            dir.mkdir(QDir::currentPath());
        }
        if(QFile::exists(destFileName)) //若存在目标文件
            QFile::remove(destFileName);//删除文件夹
    //检查源数据库文件是否存在
        bool isExist = QFile::exists(QDir::currentPath()+"/save.txt");
        if(!isExist){
          return false;
        }
        else
        {
        //重构当前备份目录及文件名,加入时间字段
            destFileName = QDir::currentPath()+ QString("/save_backup.txt");
        //调用备份命令copy
            if(QFile::copy(sourceFileName,destFileName))
            {
                //return true;
            }
            else
            {
                return false;
            }
        }
    }

    else if(current_page==1)
    {
         QString sourceFileName = QDir::currentPath()+"/save1.txt";
         QString destFileName=QDir::currentPath()+"/save_backup1.txt";
    //    创建备份文件夹
      QDir  dir(QDir::currentPath());
        if(!dir.exists()){
            dir.mkdir(QDir::currentPath());
        }
        if(QFile::exists(destFileName)) //若存在目标文件
            QFile::remove(destFileName);//删除文件夹
    //检查源数据库文件是否存在
        bool isExist = QFile::exists(QDir::currentPath()+"/save1.txt");
        if(!isExist){
          return false;
        }
        else
        {
        //重构当前备份目录及文件名,加入时间字段
            destFileName = QDir::currentPath()+ QString("/save_backup1.txt");
        //调用备份命令copy
            if(QFile::copy(sourceFileName,destFileName))
            {
               // return true;
            }
            else
            {
                return false;
            }
        }
//        return true;//感觉有问题
    }

        //第三表的存储备份
    else if(current_page==2)
    {
        QString sourceFileName = QDir::currentPath()+"/save2.txt";
        QString destFileName=QDir::currentPath()+"/save_backup2.txt";
//    创建备份文件夹
    QDir dir(QDir::currentPath());
        if(!dir.exists()){
            dir.mkdir(QDir::currentPath());
        }
        if(QFile::exists(destFileName)) //若存在目标文件
            QFile::remove(destFileName);//删除文件夹
//检查源数据库文件是否存在
        bool isExist = QFile::exists(QDir::currentPath()+"/save2.txt");
        if(!isExist){
            return false;
        }
        else
        {
//重构当前备份目录及文件名,加入时间字段
            destFileName = QDir::currentPath()+ QString("/save_backup2.txt");
//调用备份命令copy
            if(QFile::copy(sourceFileName,destFileName))
            {
                // return true;
            }
            else
            {
                return false;
            }
        }
//        return true;//==========
     }

        //第四个
    else if(current_page==3)
    {
        QString sourceFileName = QDir::currentPath()+"/save3.txt";
        QString destFileName=QDir::currentPath()+"/save_backup3.txt";
//    创建备份文件夹
    QDir dir(QDir::currentPath());
        if(!dir.exists()){
            dir.mkdir(QDir::currentPath());
        }
        if(QFile::exists(destFileName)) //若存在目标文件
            QFile::remove(destFileName);//删除文件夹
//检查源数据库文件是否存在
        bool isExist = QFile::exists(QDir::currentPath()+"/save3.txt");
        if(!isExist){
            return false;
        }
        else
        {
//重构当前备份目录及文件名,加入时间字段
            destFileName = QDir::currentPath()+ QString("/save_backup3.txt");
//调用备份命令copy
            if(QFile::copy(sourceFileName,destFileName))
            {
                // return true;
            }
            else
            {
                return false;
            }
        }
//        return true;
     }

        //最后一个
     else if(current_page==4)
    {
        QString sourceFileName = QDir::currentPath()+"/save4.txt";
        QString destFileName=QDir::currentPath()+"/save_backup4.txt";
//    创建备份文件夹
    QDir dir(QDir::currentPath());
        if(!dir.exists()){
            dir.mkdir(QDir::currentPath());
        }
        if(QFile::exists(destFileName)) //若存在目标文件
            QFile::remove(destFileName);//删除文件夹
//检查源数据库文件是否存在
        bool isExist = QFile::exists(QDir::currentPath()+"/save4.txt");
        if(!isExist){
            return false;
        }
        else
        {
//重构当前备份目录及文件名,加入时间字段
            destFileName = QDir::currentPath()+ QString("/save_backup4.txt");
//调用备份命令copy
            if(QFile::copy(sourceFileName,destFileName))
            {
                // return true;
            }
            else
            {
                return false;
            }
        }
//      return true;
    }
 return true;
}

void MainWindow::stop_excute(){
    if(!serial->isOpen())
    {
        new_win->serial_check();
        new_win->show();
    }
    else            //串口发送有效数据
    {
       // warning->close(); //刚改的，不知道行不行
        serial->write("stop");
        serial_end();
    }
}

void MainWindow::clear_excute(){
    if(!serial->isOpen())
    {
        new_win->serial_check();
        new_win->show();
    }
    else            //串口发送有效数据
    {
        serial->write("clear\r\n");
        serial_end();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    new_win->close();
    exit_dialog->show();
}

void MainWindow::rule_extend()
{
    if(current_page==0)
    {
        if(graph_para[1].final_x>100)   //补充中间参考线
        {
            QVector<QPointF> rule_a;
            QVector<QPointF> rule_error_a;//误差曲线
            for(int i =(int)graph_para[1].gra_last_time[0];i<(int)graph_para[1].final_x+100;i++){
                QPointF point;
                QPointF point_error;            //误差曲线
                point.setX(i);
                point_error.setX(i);            //误差曲线
                int y = 0;
                point.setY(y);
                point_error.setY(y);            //误差曲线
                rule_a.append(point);
                rule_error_a.append(point_error);//误差曲线
            }
            QwtPointSeriesData* rule_r = new QwtPointSeriesData(rule_a);
            rule_t->setData(rule_r);
            QwtPointSeriesData* rule_error_r = new QwtPointSeriesData(rule_error_a);//误差曲线
            rule_t->setData(rule_r);
            rule_error_t->setData(rule_error_r);//误差曲线
            ui->data_plot->replot();
            ui->error_plot->replot();
        }
    }else if(current_page==1)
    {
        if(graph_para[1].final_x>100)   //补充中间参考线
        {
            QVector<QPointF> rule_a;
            QVector<QPointF> rule_error_a;//误差曲线
            for(int i =(int)graph_para[1].gra_last_time[0];i<(int)graph_para[1].final_x+100;i++){
                QPointF point;
                QPointF point_error;            //误差曲线
                point.setX(i);
                point_error.setX(i);            //误差曲线
                int y = 0;
                point.setY(y);
                point_error.setY(y);            //误差曲线
                rule_a.append(point);
                rule_error_a.append(point_error);//误差曲线
            }
            QwtPointSeriesData* rule_r = new QwtPointSeriesData(rule_a);
            rule_t_2->setData(rule_r);
            QwtPointSeriesData* rule_error_r = new QwtPointSeriesData(rule_error_a);//误差曲线
            rule_t_2->setData(rule_r);
            rule_error_t_2->setData(rule_error_r);//误差曲线
            ui->data_plot_2->replot();
            ui->error_plot_2->replot();
        }
    }else if(current_page==2)
    {
        if(graph_para[1].final_x>100)   //补充中间参考线
        {
            QVector<QPointF> rule_a;
            QVector<QPointF> rule_error_a;//误差曲线
            for(int i =(int)graph_para[1].gra_last_time[0];i<(int)graph_para[1].final_x+100;i++){
                QPointF point;
                QPointF point_error;            //误差曲线
                point.setX(i);
                point_error.setX(i);            //误差曲线
                int y = 0;
                point.setY(y);
                point_error.setY(y);            //误差曲线
                rule_a.append(point);
                rule_error_a.append(point_error);//误差曲线
            }
            QwtPointSeriesData* rule_r = new QwtPointSeriesData(rule_a);
            rule_t_3->setData(rule_r);
            QwtPointSeriesData* rule_error_r = new QwtPointSeriesData(rule_error_a);//误差曲线
            rule_t_3->setData(rule_r);
            rule_error_t_3->setData(rule_error_r);//误差曲线
            ui->data_plot_3->replot();
            ui->error_plot_3->replot();
        }
    }else if(current_page==3)
    {
        if(graph_para[1].final_x>100)   //补充中间参考线
        {
            QVector<QPointF> rule_a;
            QVector<QPointF> rule_error_a;//误差曲线
            for(int i =(int)graph_para[1].gra_last_time[0];i<(int)graph_para[1].final_x+100;i++){
                QPointF point;
                QPointF point_error;            //误差曲线
                point.setX(i);
                point_error.setX(i);            //误差曲线
                int y = 0;
                point.setY(y);
                point_error.setY(y);            //误差曲线
                rule_a.append(point);
                rule_error_a.append(point_error);//误差曲线
            }
            QwtPointSeriesData* rule_r = new QwtPointSeriesData(rule_a);
            rule_t_4->setData(rule_r);
            QwtPointSeriesData* rule_error_r = new QwtPointSeriesData(rule_error_a);//误差曲线
            rule_t_4->setData(rule_r);
            rule_error_t_4->setData(rule_error_r);//误差曲线
            ui->data_plot_4->replot();
            ui->error_plot_4->replot();
        }
    }else if(current_page==4)
    {
        if(graph_para[1].final_x>100)   //补充中间参考线
        {
            QVector<QPointF> rule_a;
            QVector<QPointF> rule_error_a;//误差曲线
            for(int i =(int)graph_para[1].gra_last_time[0];i<(int)graph_para[1].final_x+100;i++){
                QPointF point;
                QPointF point_error;            //误差曲线
                point.setX(i);
                point_error.setX(i);            //误差曲线
                int y = 0;
                point.setY(y);
                point_error.setY(y);            //误差曲线
                rule_a.append(point);
                rule_error_a.append(point_error);//误差曲线
            }
            QwtPointSeriesData* rule_r = new QwtPointSeriesData(rule_a);
            rule_t_5->setData(rule_r);
            QwtPointSeriesData* rule_error_r = new QwtPointSeriesData(rule_error_a);//误差曲线
            rule_t_5->setData(rule_r);
            rule_error_t_5->setData(rule_error_r);//误差曲线
            ui->data_plot_5->replot();
            ui->error_plot_5->replot();
        }
    }
}

void MainWindow::serial_end()
{
    QByteArray d,a;
    d.resize(1);
    d[0]=0x0d;
    a.resize(1);
    a[0]=0x0a;
    serial->write(d);
    serial->write(a);
}

void MainWindow::tab_1_ini()
{
    //设置tableview的model
    QStandardItemModel  *model = new QStandardItemModel();

    model->setColumnCount(3);

    model->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("time(s)"));

    model->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("volt(v)"));

    model->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("loop(times)"));
    for(int i = 0; i < 100; i++)
    {
        model->setItem(i,0,new QStandardItem("0"));

           //设置字符颜色

        model->item(i,0)->setForeground(QBrush(QColor(255, 0, 0)));

           //设置字符位置

        model->item(i,0)->setTextAlignment(Qt::AlignCenter);

        model->setItem(i,1,new QStandardItem("0"));

           //设置字符颜色

        model->item(i,1)->setForeground(QBrush(QColor(255, 0, 0)));

           //设置字符位置

        model->item(i,1)->setTextAlignment(Qt::AlignCenter);

        //model->setItem(i,1,new QStandardItem(QString::fromLocal8Bit("0")));
        model->setItem(i,2,new QStandardItem("0"));

           //设置字符颜色

        model->item(i,2)->setForeground(QBrush(QColor(255, 0, 0)));

           //设置字符位置

        model->item(i,2)->setTextAlignment(Qt::AlignCenter);

    }
   ui->tableView->setModel(model);
   ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
   ui->tableView->setColumnWidth(0,60);

   ui->tableView->setColumnWidth(1,60);

   ui->tableView->setColumnWidth(2,60);
   //qwtplot设置
      //设置标题
      ui->data_plot->setTitle(QObject::tr("输入输出曲线"));
      //设置坐标轴说明
      ui->data_plot->setAxisTitle(QwtPlot::yLeft,QObject::tr("Volt(v)"));
      ui->data_plot->setAxisTitle(QwtPlot::xBottom,QObject::tr("Time(s)"));
      ui->data_plot->setAxisScale(QwtPlot::yLeft,-5,5,1);
      ui->data_plot->setAxisScale(QwtPlot::xBottom,0,10,1);
      ui->data_plot->setAxisAutoScale(QwtPlot::xBottom, false);
   //绘制0参考线
      QVector<QPointF> rule;                   //数据参考线
      for(int i =-1;i<101;i++){
          QPointF point;
          point.setX(i);
          int y = 0;
          point.setY(y);
          rule.append(point);
      }
      QwtPointSeriesData* rule_r = new QwtPointSeriesData(rule);
      rule_t = new QwtPlotCurve("Curve 0");
      rule_t->setData(rule_r);
      rule_t->attach(ui->data_plot);

      QVector<QPointF> rule_error;             //误差参考线
      for(int i =-1;i<101;i++){
          QPointF point_error;
          point_error.setX(i);
          int y = 0;
          point_error.setY(y);
          rule_error.append(point_error);
      }
      QwtPointSeriesData* rule_error_r = new QwtPointSeriesData(rule_error);
      rule_error_t = new QwtPlotCurve("Curve 0");
      rule_error_t->setData(rule_error_r);
      rule_error_t->attach(ui->error_plot);

   //绘制设计曲线
      QVector<QPointF> rule_1;
      for(int i =-1;i<11;i++){
          QPointF point;
          point.setX(i);
          int y = 0;
          point.setY(y);
          rule_1.append(point);
      }
      QwtPointSeriesData* rule_1_r = new QwtPointSeriesData(rule_1);
      design_curve_t= new QwtPlotCurve("Curve 1");
      design_curve_t->setData(rule_1_r);
      design_curve_t->setPen(QColor(255,0,0),2,Qt::SolidLine);
      design_curve_t->attach(ui->data_plot);
      design_curve_t->setVisible(false);
   //绘制实际曲线
         QVector<QPointF> rule_2;
         for(int i =-1;i<11;i++){
             QPointF point;
             point.setX(i);
             int y = 0;
             point.setY(y);
             rule_2.append(point);
         }
         QwtPointSeriesData* rule_2_r = new QwtPointSeriesData(rule_2);
         valid_curve_t= new QwtPlotCurve("Curve 1");
         valid_curve_t->setData(rule_2_r);
         valid_curve_t->setPen(QColor(0,255,0),2,Qt::SolidLine);
         valid_curve_t->attach(ui->data_plot);
         valid_curve_t->setVisible(false);

   //qwtplot2设置
      ui->error_plot->setTitle(QObject::tr("误差曲线"));
      ui->error_plot->setAxisTitle(QwtPlot::yLeft,QObject::tr("Error(v)"));
      ui->error_plot->setAxisTitle(QwtPlot::xBottom,QObject::tr("Time(s)"));
      ui->error_plot->setAxisScale(QwtPlot::yLeft,-0.2,0.2,0.1);
      ui->error_plot->setAxisScale(QwtPlot::xBottom,0,10,1);

   //绘制error曲线
      QVector<QPointF> rule_3;
      for(int i =-1;i<11;i++){
          QPointF point;
          point.setX(i);
          int y = 0;
          point.setY(y);
          rule_3.append(point);
      }
      QwtPointSeriesData* rule_3_r = new QwtPointSeriesData(rule_3);
      error_curve_t= new QwtPlotCurve("Curve 1");
      error_curve_t->setData(rule_3_r);
      error_curve_t->setPen(QColor(0,0,255),2,Qt::SolidLine);
      error_curve_t->attach(ui->error_plot);
      error_curve_t->setVisible(false);
}

//tab2控件管理
void MainWindow::tab_2_ini()
{
    //设置tableview的model
    QStandardItemModel  *model = new QStandardItemModel();

    model->setColumnCount(3);

    model->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("time(s)"));

    model->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("volt(v)"));

    model->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("loop(times)"));
    for(int i = 0; i < 100; i++)
    {
        model->setItem(i,0,new QStandardItem("0"));

           //设置字符颜色

        model->item(i,0)->setForeground(QBrush(QColor(255, 0, 0)));

           //设置字符位置

        model->item(i,0)->setTextAlignment(Qt::AlignCenter);

        model->setItem(i,1,new QStandardItem("0"));

           //设置字符颜色

        model->item(i,1)->setForeground(QBrush(QColor(255, 0, 0)));

           //设置字符位置

        model->item(i,1)->setTextAlignment(Qt::AlignCenter);

        //model->setItem(i,1,new QStandardItem(QString::fromLocal8Bit("0")));
        model->setItem(i,2,new QStandardItem("0"));

           //设置字符颜色

        model->item(i,2)->setForeground(QBrush(QColor(255, 0, 0)));

           //设置字符位置

        model->item(i,2)->setTextAlignment(Qt::AlignCenter);

    }
   ui->tableView_2->setModel(model);
   ui->tableView_2->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
   ui->tableView_2->setColumnWidth(0,60);

   ui->tableView_2->setColumnWidth(1,60);

   ui->tableView_2->setColumnWidth(2,60);
   //qwtplot设置
      //设置标题
      ui->data_plot_2->setTitle(QObject::tr("输入输出曲线"));
      //设置坐标轴说明
      ui->data_plot_2->setAxisTitle(QwtPlot::yLeft,QObject::tr("Volt(v)"));
      ui->data_plot_2->setAxisTitle(QwtPlot::xBottom,QObject::tr("Time(s)"));
      ui->data_plot_2->setAxisScale(QwtPlot::yLeft,-5,5,1);
      ui->data_plot_2->setAxisScale(QwtPlot::xBottom,0,10,1);
      ui->data_plot_2->setAxisAutoScale(QwtPlot::xBottom, false);
   //绘制0参考线
      QVector<QPointF> rule;                   //数据参考线
      for(int i =-1;i<101;i++){
          QPointF point;
          point.setX(i);
          int y = 0;
          point.setY(y);
          rule.append(point);
      }
      QwtPointSeriesData* rule_r = new QwtPointSeriesData(rule);
      rule_t_2 = new QwtPlotCurve("Curve 0");
      rule_t_2->setData(rule_r);
      rule_t_2->attach(ui->data_plot_2);

      QVector<QPointF> rule_error;             //误差参考线
      for(int i =-1;i<101;i++){
          QPointF point_error;
          point_error.setX(i);
          int y = 0;
          point_error.setY(y);
          rule_error.append(point_error);
      }
      QwtPointSeriesData* rule_error_r = new QwtPointSeriesData(rule_error);
      rule_error_t_2 = new QwtPlotCurve("Curve 0");
      rule_error_t_2->setData(rule_error_r);
      rule_error_t_2->attach(ui->error_plot_2);

   //绘制设计曲线
      QVector<QPointF> rule_1;
      for(int i =-1;i<11;i++){
          QPointF point;
          point.setX(i);
          int y = 0;
          point.setY(y);
          rule_1.append(point);
      }
      QwtPointSeriesData* rule_1_r = new QwtPointSeriesData(rule_1);
      design_curve_t_2= new QwtPlotCurve("Curve 1");
      design_curve_t_2->setData(rule_1_r);
      design_curve_t_2->setPen(QColor(255,0,0),2,Qt::SolidLine);
      design_curve_t_2->attach(ui->data_plot_2);
      design_curve_t_2->setVisible(false);
   //绘制实际曲线
         QVector<QPointF> rule_2;
         for(int i =-1;i<11;i++){
             QPointF point;
             point.setX(i);
             int y = 0;
             point.setY(y);
             rule_2.append(point);
         }
         QwtPointSeriesData* rule_2_r = new QwtPointSeriesData(rule_2);
         valid_curve_t_2= new QwtPlotCurve("Curve 1");
         valid_curve_t_2->setData(rule_2_r);
         valid_curve_t_2->setPen(QColor(0,255,0),2,Qt::SolidLine);
         valid_curve_t_2->attach(ui->data_plot_2);
         valid_curve_t_2->setVisible(false);

   //qwtplot2设置
      ui->error_plot_2->setTitle(QObject::tr("误差曲线"));
      ui->error_plot_2->setAxisTitle(QwtPlot::yLeft,QObject::tr("Error(v)"));
      ui->error_plot_2->setAxisTitle(QwtPlot::xBottom,QObject::tr("Time(s)"));
      ui->error_plot_2->setAxisScale(QwtPlot::yLeft,-0.2,0.2,0.1);
      ui->error_plot_2->setAxisScale(QwtPlot::xBottom,0,10,1);
   //绘制error曲线
      QVector<QPointF> rule_3;
      for(int i =-1;i<11;i++){
          QPointF point;
          point.setX(i);
          int y = 0;
          point.setY(y);
          rule_3.append(point);
      }
      QwtPointSeriesData* rule_3_r = new QwtPointSeriesData(rule_3);
      error_curve_t_2= new QwtPlotCurve("Curve 1");
      error_curve_t_2->setData(rule_3_r);
      error_curve_t_2->setPen(QColor(0,0,255),2,Qt::SolidLine);
      error_curve_t_2->attach(ui->error_plot_2);
      error_curve_t_2->setVisible(false);
}

//tab3控件管理
void MainWindow::tab_3_ini()
{

    //设置tableview3的model
    QStandardItemModel  *model = new QStandardItemModel();

    model->setColumnCount(3);

    model->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("time(s)"));

    model->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("volt(v)"));

    model->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("loop(times)"));
    for(int i = 0; i < 100; i++)
    {
        model->setItem(i,0,new QStandardItem("0"));

           //设置字符颜色

        model->item(i,0)->setForeground(QBrush(QColor(255, 0, 0)));

           //设置字符位置

        model->item(i,0)->setTextAlignment(Qt::AlignCenter);

        model->setItem(i,1,new QStandardItem("0"));

           //设置字符颜色

        model->item(i,1)->setForeground(QBrush(QColor(255, 0, 0)));

           //设置字符位置

        model->item(i,1)->setTextAlignment(Qt::AlignCenter);

        //model->setItem(i,1,new QStandardItem(QString::fromLocal8Bit("0")));
        model->setItem(i,2,new QStandardItem("0"));

           //设置字符颜色

        model->item(i,2)->setForeground(QBrush(QColor(255, 0, 0)));

           //设置字符位置

        model->item(i,2)->setTextAlignment(Qt::AlignCenter);

    }
   ui->tableView_3->setModel(model);
   ui->tableView_3->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
   ui->tableView_3->setColumnWidth(0,60);

   ui->tableView_3->setColumnWidth(1,60);

   ui->tableView_3->setColumnWidth(2,60);
   //qwtplot设置
      //设置标题
      ui->data_plot_3->setTitle(QObject::tr("输入输出曲线"));
      //设置坐标轴说明
      ui->data_plot_3->setAxisTitle(QwtPlot::yLeft,QObject::tr("Volt(v)"));
      ui->data_plot_3->setAxisTitle(QwtPlot::xBottom,QObject::tr("Time(s)"));
      ui->data_plot_3->setAxisScale(QwtPlot::yLeft,-5,5,1);
      ui->data_plot_3->setAxisScale(QwtPlot::xBottom,0,10,1);
      ui->data_plot_3->setAxisAutoScale(QwtPlot::xBottom, false);
   //绘制0参考线
      QVector<QPointF> rule;                   //数据参考线
      for(int i =-1;i<101;i++){
          QPointF point;
          point.setX(i);
          int y = 0;
          point.setY(y);
          rule.append(point);
      }
      QwtPointSeriesData* rule_r = new QwtPointSeriesData(rule);
      rule_t_3 = new QwtPlotCurve("Curve 0");
      rule_t_3->setData(rule_r);
      rule_t_3->attach(ui->data_plot_3);

      QVector<QPointF> rule_error;             //误差参考线
      for(int i =-1;i<101;i++){
          QPointF point_error;
          point_error.setX(i);
          int y = 0;
          point_error.setY(y);
          rule_error.append(point_error);
      }
      QwtPointSeriesData* rule_error_r = new QwtPointSeriesData(rule_error);
      rule_error_t_3 = new QwtPlotCurve("Curve 0");
      rule_error_t_3->setData(rule_error_r);
      rule_error_t_3->attach(ui->error_plot_3);

   //绘制设计曲线
      QVector<QPointF> rule_1;
      for(int i =-1;i<11;i++){
          QPointF point;
          point.setX(i);
          int y = 0;
          point.setY(y);
          rule_1.append(point);
      }
      QwtPointSeriesData* rule_1_r = new QwtPointSeriesData(rule_1);
      design_curve_t_3= new QwtPlotCurve("Curve 1");
      design_curve_t_3->setData(rule_1_r);
      design_curve_t_3->setPen(QColor(255,0,0),2,Qt::SolidLine);
      design_curve_t_3->attach(ui->data_plot_3);
      design_curve_t_3->setVisible(false);
   //绘制实际曲线
         QVector<QPointF> rule_2;
         for(int i =-1;i<11;i++){
             QPointF point;
             point.setX(i);
             int y = 0;
             point.setY(y);
             rule_2.append(point);
         }
         QwtPointSeriesData* rule_2_r = new QwtPointSeriesData(rule_2);
         valid_curve_t_3= new QwtPlotCurve("Curve 1");
         valid_curve_t_3->setData(rule_2_r);
         valid_curve_t_3->setPen(QColor(0,255,0),2,Qt::SolidLine);
         valid_curve_t_3->attach(ui->data_plot_3);
         valid_curve_t_3->setVisible(false);

   //qwtplot2设置
      ui->error_plot_3->setTitle(QObject::tr("误差曲线"));
      ui->error_plot_3->setAxisTitle(QwtPlot::yLeft,QObject::tr("Error(v)"));
      ui->error_plot_3->setAxisTitle(QwtPlot::xBottom,QObject::tr("Time(s)"));
      ui->error_plot_3->setAxisScale(QwtPlot::yLeft,-0.2,0.2,0.1);
      ui->error_plot_3->setAxisScale(QwtPlot::xBottom,0,10,1);
   //绘制error曲线
      QVector<QPointF> rule_3;
      for(int i =-1;i<11;i++){
          QPointF point;
          point.setX(i);
          int y = 0;
          point.setY(y);
          rule_3.append(point);
      }
      QwtPointSeriesData* rule_3_r = new QwtPointSeriesData(rule_3);
      error_curve_t_3= new QwtPlotCurve("Curve 1");
      error_curve_t_3->setData(rule_3_r);
      error_curve_t_3->setPen(QColor(0,0,255),2,Qt::SolidLine);
      error_curve_t_3->attach(ui->error_plot_3);
      error_curve_t_3->setVisible(false);
}


//tab4控件管理
void MainWindow::tab_4_ini()
{
    //设置tableview4的model
    QStandardItemModel  *model = new QStandardItemModel();

    model->setColumnCount(3);

    model->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("time(s)"));

    model->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("volt(v)"));

    model->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("loop(times)"));
    for(int i = 0; i < 100; i++)
    {
        model->setItem(i,0,new QStandardItem("0"));

           //设置字符颜色

        model->item(i,0)->setForeground(QBrush(QColor(255, 0, 0)));

           //设置字符位置

        model->item(i,0)->setTextAlignment(Qt::AlignCenter);

        model->setItem(i,1,new QStandardItem("0"));

           //设置字符颜色

        model->item(i,1)->setForeground(QBrush(QColor(255, 0, 0)));

           //设置字符位置

        model->item(i,1)->setTextAlignment(Qt::AlignCenter);

        //model->setItem(i,1,new QStandardItem(QString::fromLocal8Bit("0")));
        model->setItem(i,2,new QStandardItem("0"));

           //设置字符颜色

        model->item(i,2)->setForeground(QBrush(QColor(255, 0, 0)));

           //设置字符位置

        model->item(i,2)->setTextAlignment(Qt::AlignCenter);

    }
   ui->tableView_4->setModel(model);
   ui->tableView_4->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
   ui->tableView_4->setColumnWidth(0,60);

   ui->tableView_4->setColumnWidth(1,60);

   ui->tableView_4->setColumnWidth(2,60);
   //qwtplot设置
      //设置标题
      ui->data_plot_4->setTitle(QObject::tr("输入输出曲线"));
      //设置坐标轴说明
      ui->data_plot_4->setAxisTitle(QwtPlot::yLeft,QObject::tr("Volt(v)"));
      ui->data_plot_4->setAxisTitle(QwtPlot::xBottom,QObject::tr("Time(s)"));
      ui->data_plot_4->setAxisScale(QwtPlot::yLeft,-5,5,1);
      ui->data_plot_4->setAxisScale(QwtPlot::xBottom,0,10,1);
      ui->data_plot_4->setAxisAutoScale(QwtPlot::xBottom, false);
   //绘制0参考线
      QVector<QPointF> rule;                   //数据参考线
      for(int i =-1;i<101;i++){
          QPointF point;
          point.setX(i);
          int y = 0;
          point.setY(y);
          rule.append(point);
      }
      QwtPointSeriesData* rule_r = new QwtPointSeriesData(rule);
      rule_t_4 = new QwtPlotCurve("Curve 0");
      rule_t_4->setData(rule_r);
      rule_t_4->attach(ui->data_plot_4);

      QVector<QPointF> rule_error;             //误差参考线
      for(int i =-1;i<101;i++){
          QPointF point_error;
          point_error.setX(i);
          int y = 0;
          point_error.setY(y);
          rule_error.append(point_error);
      }
      QwtPointSeriesData* rule_error_r = new QwtPointSeriesData(rule_error);
      rule_error_t_4 = new QwtPlotCurve("Curve 0");
      rule_error_t_4->setData(rule_error_r);
      rule_error_t_4->attach(ui->error_plot_4);

   //绘制设计曲线
      QVector<QPointF> rule_1;
      for(int i =-1;i<11;i++){
          QPointF point;
          point.setX(i);
          int y = 0;
          point.setY(y);
          rule_1.append(point);
      }
      QwtPointSeriesData* rule_1_r = new QwtPointSeriesData(rule_1);
      design_curve_t_4= new QwtPlotCurve("Curve 1");
      design_curve_t_4->setData(rule_1_r);
      design_curve_t_4->setPen(QColor(255,0,0),2,Qt::SolidLine);
      design_curve_t_4->attach(ui->data_plot_4);
      design_curve_t_4->setVisible(false);
   //绘制实际曲线
         QVector<QPointF> rule_2;
         for(int i =-1;i<11;i++){
             QPointF point;
             point.setX(i);
             int y = 0;
             point.setY(y);
             rule_2.append(point);
         }
         QwtPointSeriesData* rule_2_r = new QwtPointSeriesData(rule_2);
         valid_curve_t_4= new QwtPlotCurve("Curve 1");
         valid_curve_t_4->setData(rule_2_r);
         valid_curve_t_4->setPen(QColor(0,255,0),2,Qt::SolidLine);
         valid_curve_t_4->attach(ui->data_plot_4);
         valid_curve_t_4->setVisible(false);

   //qwtplot2设置
      ui->error_plot_4->setTitle(QObject::tr("误差曲线"));
      ui->error_plot_4->setAxisTitle(QwtPlot::yLeft,QObject::tr("Error(v)"));
      ui->error_plot_4->setAxisTitle(QwtPlot::xBottom,QObject::tr("Time(s)"));
      ui->error_plot_4->setAxisScale(QwtPlot::yLeft,-0.2,0.2,0.1);
      ui->error_plot_4->setAxisScale(QwtPlot::xBottom,0,10,1);
   //绘制error曲线
      QVector<QPointF> rule_3;
      for(int i =-1;i<11;i++){
          QPointF point;
          point.setX(i);
          int y = 0;
          point.setY(y);
          rule_3.append(point);
      }
      QwtPointSeriesData* rule_3_r = new QwtPointSeriesData(rule_3);
      error_curve_t_4= new QwtPlotCurve("Curve 1");
      error_curve_t_4->setData(rule_3_r);
      error_curve_t_4->setPen(QColor(0,0,255),2,Qt::SolidLine);
      error_curve_t_4->attach(ui->error_plot_4);
      error_curve_t_4->setVisible(false);
}


//tab5控件管理
void MainWindow::tab_5_ini()
{
    //设置tableview5的model
    QStandardItemModel  *model = new QStandardItemModel();

    model->setColumnCount(3);

    model->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("time(s)"));

    model->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("volt(v)"));

    model->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("loop(times)"));
    for(int i = 0; i < 100; i++)
    {
        model->setItem(i,0,new QStandardItem("0"));

           //设置字符颜色

        model->item(i,0)->setForeground(QBrush(QColor(255, 0, 0)));

           //设置字符位置

        model->item(i,0)->setTextAlignment(Qt::AlignCenter);

        model->setItem(i,1,new QStandardItem("0"));

           //设置字符颜色

        model->item(i,1)->setForeground(QBrush(QColor(255, 0, 0)));

           //设置字符位置

        model->item(i,1)->setTextAlignment(Qt::AlignCenter);

        //model->setItem(i,1,new QStandardItem(QString::fromLocal8Bit("0")));
        model->setItem(i,2,new QStandardItem("0"));

           //设置字符颜色

        model->item(i,2)->setForeground(QBrush(QColor(255, 0, 0)));

           //设置字符位置

        model->item(i,2)->setTextAlignment(Qt::AlignCenter);

    }
   ui->tableView_5->setModel(model);
   ui->tableView_5->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
   ui->tableView_5->setColumnWidth(0,60);

   ui->tableView_5->setColumnWidth(1,60);

   ui->tableView_5->setColumnWidth(2,60);
   //qwtplot设置
      //设置标题
      ui->data_plot_5->setTitle(QObject::tr("输入输出曲线"));
      //设置坐标轴说明
      ui->data_plot_5->setAxisTitle(QwtPlot::yLeft,QObject::tr("Volt(v)"));
      ui->data_plot_5->setAxisTitle(QwtPlot::xBottom,QObject::tr("Time(s)"));
      ui->data_plot_5->setAxisScale(QwtPlot::yLeft,-5,5,1);
      ui->data_plot_5->setAxisScale(QwtPlot::xBottom,0,10,1);
      ui->data_plot_5->setAxisAutoScale(QwtPlot::xBottom, false);
   //绘制0参考线
      QVector<QPointF> rule;                   //数据参考线
      for(int i =-1;i<101;i++){
          QPointF point;
          point.setX(i);
          int y = 0;
          point.setY(y);
          rule.append(point);
      }
      QwtPointSeriesData* rule_r = new QwtPointSeriesData(rule);
      rule_t_5 = new QwtPlotCurve("Curve 0");
      rule_t_5->setData(rule_r);
      rule_t_5->attach(ui->data_plot_5);

      QVector<QPointF> rule_error;             //误差参考线
      for(int i =-1;i<101;i++){
          QPointF point_error;
          point_error.setX(i);
          int y = 0;
          point_error.setY(y);
          rule_error.append(point_error);
      }
      QwtPointSeriesData* rule_error_r = new QwtPointSeriesData(rule_error);
      rule_error_t_5 = new QwtPlotCurve("Curve 0");
      rule_error_t_5->setData(rule_error_r);
      rule_error_t_5->attach(ui->error_plot_5);

   //绘制设计曲线
      QVector<QPointF> rule_1;
      for(int i =-1;i<11;i++){
          QPointF point;
          point.setX(i);
          int y = 0;
          point.setY(y);
          rule_1.append(point);
      }
      QwtPointSeriesData* rule_1_r = new QwtPointSeriesData(rule_1);
      design_curve_t_5= new QwtPlotCurve("Curve 1");
      design_curve_t_5->setData(rule_1_r);
      design_curve_t_5->setPen(QColor(255,0,0),2,Qt::SolidLine);
      design_curve_t_5->attach(ui->data_plot_5);
      design_curve_t_5->setVisible(false);
   //绘制实际曲线
         QVector<QPointF> rule_2;
         for(int i =-1;i<11;i++){
             QPointF point;
             point.setX(i);
             int y = 0;
             point.setY(y);
             rule_2.append(point);
         }
         QwtPointSeriesData* rule_2_r = new QwtPointSeriesData(rule_2);
         valid_curve_t_5= new QwtPlotCurve("Curve 1");
         valid_curve_t_5->setData(rule_2_r);
         valid_curve_t_5->setPen(QColor(0,255,0),2,Qt::SolidLine);
         valid_curve_t_5->attach(ui->data_plot_5);
         valid_curve_t_5->setVisible(false);

   //qwtplot2设置
      ui->error_plot_5->setTitle(QObject::tr("误差曲线"));
      ui->error_plot_5->setAxisTitle(QwtPlot::yLeft,QObject::tr("Error(v)"));
      ui->error_plot_5->setAxisTitle(QwtPlot::xBottom,QObject::tr("Time(s)"));
      ui->error_plot_5->setAxisScale(QwtPlot::yLeft,-0.2,0.2,0.1);
      ui->error_plot_5->setAxisScale(QwtPlot::xBottom,0,10,1);
   //绘制error曲线
      QVector<QPointF> rule_3;
      for(int i =-1;i<11;i++){
          QPointF point;
          point.setX(i);
          int y = 0;
          point.setY(y);
          rule_3.append(point);
      }
      QwtPointSeriesData* rule_3_r = new QwtPointSeriesData(rule_3);
      error_curve_t_5= new QwtPlotCurve("Curve 1");
      error_curve_t_5->setData(rule_3_r);
      error_curve_t_5->setPen(QColor(0,0,255),2,Qt::SolidLine);
      error_curve_t_5->attach(ui->error_plot_5);
      error_curve_t_5->setVisible(false);
}


void MainWindow::on_clear_2_clicked()
{
    auto_state = OFF;
    QModelIndex indextemp;
    for(int i=0;i<ui->tableView_2->model()->rowCount();i++)        //清零表格中的数据
    {
        for(int j=0;j<3;j++)
        {
            indextemp = ui->tableView_2->model()->index(i,j);
            ui->tableView_2->model()->setData(indextemp,0.0);
        }
    }
    memset(graph_para[0].gra_volt, 0, 100*sizeof(float) );
    memset(graph_para[1].gra_volt, 0, 100*sizeof(float) );
    graph_para[0].state=INITIAL;
    graph_para[0].gra_scan_row=0;
    data_temp[0].down_valid_row_num=0;
    //修复bug，清除上一幅图像的数据和本地缓存
    memset(data_temp[0].loop_times,0,100*sizeof(int));

//清除曲线名
    ui->tabWidget->setTabText(1,"tab_2");

    //清理LCDnumber
    ui->lcdNumber_6->display(0);
    ui->lcdNumber_4->display(0);
    ui->lcdNumber_5->display(0);
    ui->lcdNumber_20->display(0);
    ui->lcdNumber_21->display(0);
     ui->lcdNumber_17->display(0);//=================

    graph_para[1].state=RESET;
    graph_para[1].final_x=0;
    graph_down_replot_2();

    //数据图坐标更新回复
    ui->data_plot_2->setAxisScale(QwtPlot::yLeft,-5,5,1);
    ui->data_plot_2->setAxisScale(QwtPlot::xBottom,0,10,1);
    QVector<QPointF> rule;                   //数据参考线
    for(int i =-1;i<101;i++){
        QPointF point;
        point.setX(i);
        int y = 0;
        point.setY(y);
        rule.append(point);
    }
    QwtPointSeriesData* rule_c = new QwtPointSeriesData(rule);
    rule_t_2 = new QwtPlotCurve("Curve 0");
    rule_t_2->setData(rule_c);
    rule_t_2->attach(ui->data_plot_2);

    QVector<QPointF> rule_error;             //误差参考线
    for(int i =-1;i<101;i++){
        QPointF point_error;
        point_error.setX(i);
        int y = 0;
        point_error.setY(y);
        rule_error.append(point_error);
    }
    QwtPointSeriesData* rule_error_c = new QwtPointSeriesData(rule_error);
    rule_error_t_2 = new QwtPlotCurve("Curve 0");
    rule_error_t_2->setData(rule_error_c);
    rule_error_t_2->attach(ui->error_plot_2);

    valid_curve_t_2->setVisible(false);            //隐藏实际曲线
    ui->error_plot_2->replot();
    error_curve_t_2->setVisible(false);            //隐藏误差曲线
    ui->error_plot_2->replot();
    design_curve_t_2->setVisible(false);            //隐藏误差曲线
    ui->error_plot_2->replot();
    QVector<QPointF> rule_a;
    QVector<QPointF> rule_error_a;//误差曲线
    for(int i =0;i<100;i++){
        QPointF point;
        QPointF point_error;            //误差曲线
        point.setX(i);
        point_error.setX(i);            //误差曲线
        point.setY(0);
        point_error.setY(0);            //误差曲线
        rule_a.append(point);
        rule_error_a.append(point_error);//误差曲线
    }
    QwtPointSeriesData* rule_r = new QwtPointSeriesData(rule_a);
    rule_t_2->setData(rule_r);
    QwtPointSeriesData* rule_error_r = new QwtPointSeriesData(rule_error_a);//误差曲线
    rule_error_t_2->setData(rule_error_r);//误差曲线
    ui->data_plot_2->replot();
    ui->error_plot_2->replot();
    stop_excute();

}

void MainWindow::on_download_2_clicked()
{
    if(local_state!=AUTO)
    {
        QModelIndex indextemp;
        float temp;
        int i=0;
        int j=0;
        for( i = 0;i<ui->tableView_2->model()->rowCount();i++)
        {
            for( j=0;j<3;j++)
            {
                indextemp = ui->tableView_2->model()->index(i,j);
                temp=ui->tableView_2->model()->data(indextemp).toFloat();
                datatemp[i][j]=temp;
            }
            if((datatemp[i][0]==-1)&&(datatemp[i][1]==-1)&&(datatemp[i][2]==-1))
            {
                data_temp[0].down_valid_row_num=i;  //下传数据有效行数
               //valid_row_number=i;
                break;
            }
        }
        char ctemp[0];
        if(!serial->isOpen())
        {
            new_win->serial_check();
            new_win->show();
        }
        else            //串口发送有效数据
        {
            sprintf(&ctemp[0],"%d;",data_temp[0].down_valid_row_num);
            serial->write(&ctemp[0]);
            for(i=0;i<data_temp[0].down_valid_row_num;i++)
            {
               data_temp[0].last_time[i]=datatemp[i][0];
               sprintf(&ctemp[0],"%d;",(int)(datatemp[i][0]*1000));
               serial->write(&ctemp[0]);
               data_temp[0].volt[i]=datatemp[i][1];
               sprintf(&ctemp[0],"%d;",(int)(datatemp[i][1]*1000));
               serial->write(&ctemp[0]);
               data_temp[0].loop_times[i]=datatemp[i][2];
               sprintf(&ctemp[0],"%d;",(int)(datatemp[i][2]));
               serial->write(&ctemp[0]);
            }
            serial_end();//发送给单片机的终止符

            graph_para[0].state=INITIAL;
            graph_para[0].gra_scan_row=0;  //扫描行数回到开头
            graph_para[0].gra_node_num=0;     //图中节点数
            graph_para[0].final_x=0;          //最后一点坐标
            graph_para[0].gra_loop_times=0;  //记录各循环节点剩余次数
            graph_para[0].gra_scan_row=0;            //图像扫描的行数
            memset(graph_para[0].gra_last_time,0,100*sizeof(float));  //准备显示电压持续时间
            memset(graph_para[0].gra_volt,0,100*sizeof(float));  //准备电压值
            graph_para[0].gra_loop_head_row=0;   //循环的开头行数
            design_curve_t_2->setVisible(true);
            //清空实际图像
            graph_para[1].state=RESET;
            graph_down_replot_2();
            //设计图形初始化完成
            graph_para[0].state=DOWN_FINISH;

            //复原坐标轴
            ui->data_plot_2->setAxisAutoScale(QwtPlot::xBottom, false);
            ui->data_plot_2->setAxisScale(QwtPlot::xBottom,0,10,1);
            ui->data_plot_2->replot();
            ui->error_plot_2->setAxisAutoScale(QwtPlot::xBottom, false);
            ui->error_plot_2->setAxisScale(QwtPlot::xBottom,0,10,1);
            ui->error_plot_2->replot();
            QVector<QPointF> rule_a;
            //补充中间参考线长度
            for(int i =0;i<101;i++){
                QPointF point;
                point.setX(i);
                int y = 0;
                point.setY(y);
                rule_a.append(point);
            }
            QwtPointSeriesData* rule_r = new QwtPointSeriesData(rule_a);
            rule_t_2->setData(rule_r);
            ui->data_plot_2->replot();
            QVector<QPointF> rule_error_a;
            for(int i =0;i<101;i++){
                QPointF point;
                point.setX(i);
                int y = 0;
                point.setY(y);
                rule_error_a.append(point);
            }
            QwtPointSeriesData* rule_error_r = new QwtPointSeriesData(rule_error_a);
            rule_error_t_2->setData(rule_error_r);
            ui->error_plot_2->replot();
            //隐藏实际曲线
            valid_curve_t_2->setVisible(false);
            error_curve_t_2->setVisible(false);
            ui->data_plot_2->replot();
        }
    }
}

void MainWindow::on_load_2_clicked()
{
    //先执行clear
        on_clear_2_clicked();
    //读取save数据
        if(local_state==IDLE||local_state==LOAD_ALL||local_state==SAVE_ALL)
        {
            int is=0;
            int js=0;
            std::ifstream file_t("save1.txt");
            std::string p;
            std::string q;
            QString name;
            file_t>>data_temp[0].load_valid_row_num;   //保存数据的有效行数;
            file_t>>q;//读取本行字放进q中，字属于string类
            name=QString::fromStdString(q);//转换为QString类型，方便下面写入
            ui->tabWidget->setTabText(1,name);//load时成功写入名字
            std::getline(file_t,p);
            std::getline(file_t,p);
            std::getline(file_t,p);
            while(!file_t.eof())        //按行读取
            {
                file_t>>datatemp[is][js++];
                file_t>>datatemp[is][js++];
                file_t>>datatemp[is++][js];
                js=0;
            }
            file_t.close();
            QModelIndex indextemp;
            for(int i=0;i<data_temp[0].load_valid_row_num+1;i++)        //读取文档中的数据显示在表格中
            {
                for(int j=0;j<3;j++)
                {
                    indextemp = ui->tableView_2->model()->index(i,j);
                    ui->tableView_2->model()->setData(indextemp,qRound(datatemp[i][j]*1000.0)/1000.0);
                    if(i == data_temp[0].load_valid_row_num)
                        ui->tableView_2->model()->setData(indextemp,-1.0);
                }

            }
        }else if(local_state==RESTORE_ALL||local_state==BACKUP_ALL)
        {
        //读取save_backup数据
            int is_back=0;
            int js_back=0;
            std::ifstream file_t_back("save_backup1.txt");
            std::string p;
            std::string q;
            QString name;
            file_t_back>>data_temp[0].load_valid_row_num;   //保存数据的有效行数;
            file_t_back>>q;//读取本行字放进q中，字属于string类
            name=QString::fromStdString(q);//转换为QString类型，方便下面写入
            ui->tabWidget->setTabText(1,name);//load时成功写入名字
            std::getline(file_t_back,p);
            std::getline(file_t_back,p);
            std::getline(file_t_back,p);
            while(!file_t_back.eof())        //按行读取
            {
                file_t_back>>datatemp[is_back][js_back++];
                file_t_back>>datatemp[is_back][js_back++];
                file_t_back>>datatemp[is_back++][js_back];
                js_back=0;
            }
            file_t_back.close();
            QModelIndex indextemp_back;
            for(int i=0;i<data_temp[0].load_valid_row_num+1;i++)        //读取文档中的数据显示在表格中
            {
                for(int j=0;j<3;j++)
                {
                    indextemp_back = ui->tableView_2->model()->index(i,j);
                    ui->tableView_2->model()->setData(indextemp_back,qRound(datatemp[i][j]*1000.0)/1000.0);//读取文件的数据有bug，必须这样
                    if(i == data_temp[0].load_valid_row_num)
                        ui->tableView_2->model()->setData(indextemp_back,-1.0);
                }

            }
        }
        if(local_state!=AUTO)       //不在AUTO模式下，允许画图
        {
            design_curve_t_2->setVisible(true);
            //绘制图像
            QModelIndex indextemp_1;
            float temp;//========================================================
            int i=0;
            int j=0;
            for( i = 0;i<ui->tableView_2->model()->rowCount();i++)
            {
                for( j=0;j<3;j++)
                {
                    indextemp_1 = ui->tableView_2->model()->index(i,j);
                    temp=ui->tableView_2->model()->data(indextemp_1).toFloat();
                    datatemp[i][j]=temp;
                }
                if((datatemp[i][0]==-1)&&(datatemp[i][1]==-1)&&(datatemp[i][2]==-1))
                {
                    data_temp[0].down_valid_row_num=i;  //下传数据有效行数
                    break;
                }
            }
            for(i=0;i<data_temp[0].down_valid_row_num;i++)
            {
               data_temp[0].last_time[i]=datatemp[i][0];
               data_temp[0].volt[i]=datatemp[i][1];
               data_temp[0].loop_times[i]=datatemp[i][2];
            }
            graph_para[0].state=INITIAL;
            graph_para[0].gra_scan_row=0;  //扫描行数回到开头
            graph_para[0].gra_node_num=0;     //图中节点数
            graph_para[0].final_x=0;          //最后一点坐标
            graph_para[0].gra_loop_times=0;  //记录各循环节点剩余次数
            graph_para[0].gra_scan_row=0;            //图像扫描的行数
            memset(graph_para[0].gra_last_time,0,100*sizeof(float));  //准备显示电压持续时间
            memset(graph_para[0].gra_volt,0,100*sizeof(float));  //准备电压值
            graph_para[0].gra_loop_head_row=0;   //循环的开头行数
            //隐藏实际曲线
            //valid_curve_t_2->setPen(QColor(0,0,0),2,Qt::SolidLine);
            valid_curve_t_2->setVisible(false);
            ui->data_plot_2->replot();
            //清空实际图像
            graph_para[1].state=RESET;
            graph_down_replot_2();
            //load时显示最后一个坐标，变换坐标轴
            if(graph_para[0].final_x>10)
            {
                ui->data_plot_2->setAxisAutoScale(QwtPlot::xBottom, false);
                ui->data_plot_2->setAxisScale(QwtPlot::xBottom,0,graph_para[0].final_x,graph_para[0].final_x/10);
                ui->data_plot_2->replot();
            }
            else
            {
                ui->data_plot_2->setAxisAutoScale(QwtPlot::xBottom, false);
                ui->data_plot_2->setAxisScale(QwtPlot::xBottom,0,100,10);
                ui->data_plot_2->replot();
            }
            //补充中间参考线长度
            QVector<QPointF> rule_a;
            if(graph_para[0].final_x>100)
            {
                for(int i =0;i<(int)graph_para[0].final_x+100;i++){
                    QPointF point;
                    point.setX(i);
                    int y = 0;
                    point.setY(y);
                    rule_a.append(point);
                }
            }
            else
            {
                for(int i =0;i<100;i++){
                    QPointF point;
                    point.setX(i);
                    int y = 0;
                    point.setY(y);
                    rule_a.append(point);
                }
            }
            QwtPointSeriesData* rule_r = new QwtPointSeriesData(rule_a);
            rule_t_2->setData(rule_r);
            ui->data_plot_2->replot();

        }
}

void MainWindow::on_save_2_clicked()
{
    if (local_state!=SAVE_ALL&&local_state!=BACKUP_ALL)//==============================
    {
            name_dialog->show();//===============================================
    }
}

void MainWindow::on_save_2_clicked_after()
{


    //backup操作
        if(local_state==BACKUP_ALL)
        {
            bool ok=dataBackup();
             qDebug()<<ok;
        }


    //save操作
        float temp;//============================================================
        QModelIndex indextemp;
        if(local_state==IDLE||local_state==SAVE_ALL)
        {
            QFile file("save1.txt");
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                return;
            for(int i=0;i<ui->tableView_2->model()->rowCount();i++)
            {
                for(int j=0;j<3;j++)
                {
                    indextemp = ui->tableView_2->model()->index(i,j);
                    //datatemp[i][j] = ui->tableView->model()->data(indextemp).toInt();
                    temp=ui->tableView_2->model()->data(indextemp).toFloat();
                    datatemp[i][j]=temp;//=======================================================未知
                   // out << datatemp[i][j] << " ";
                }
                if((datatemp[i][0]==-1)&&(datatemp[i][1]==-1)&&(datatemp[i][2]==-1))
                {
                    data_temp[0].save_valid_row_num=i;   //保存数据的有效行数
                    break;
                }
               // out<<"\n";
            }
           //===================================================
            //memset(data_temp[0].last_time,0,100*sizeof(float));
            //memset(data_temp[0].volt,0,100*sizeof(float));
          //  memset(data_temp[0].loop_times,0,100*sizeof(int));
            //==================================================

            //========================================================================
            QTextStream out(&file);
            out.setFieldWidth(8);
            out<<data_temp[0].save_valid_row_num;
            out<<"\n";
            out << ui->tabWidget->tabText(1);//=============================================
            out << "\n";//==================================================================
            out<<"    time      volt      loop\n";
            out<<"====================\n";
            for(int i=0;i<data_temp[0].save_valid_row_num;i++)
            {
                for(int j=0;j<3;j++)
                {
                  out << datatemp[i][j] << " ";
                }
                out<<"\n";
            }
            file.close();
        }
        //memset(datatemp,0,sizeof(float)*100*3);
        //memset(datatemp[][1],0,100*sizeof(float));
       // memset(datatemp[][2],0,100*sizeof(float));
        test_text_2();
        on_load_2_clicked();
}

void MainWindow::on_excute_2_clicked()
{
    //清空实际图像
    if(graph_para[0].state==DOWN_FINISH)     //download完成
    {
        graph_para[1].state=INITIAL;
        graph_para[1].gra_scan_row=0;  //扫描行数回到开头
        graph_para[1].gra_node_num=0;     //图中节点数
        graph_para[1].final_x=0;          //最后一点坐标
        graph_para[1].gra_loop_times=0;  //记录各循环节点剩余次数
        graph_para[1].gra_scan_row=0;            //图像扫描的行数
        memset(graph_para[1].gra_last_time,0,100*sizeof(float));  //准备显示电压持续时间
        memset(graph_para[1].gra_volt,0,100*sizeof(float));  //准备电压值
        graph_para[1].gra_loop_head_row=0;   //循环的开头行数

        if(!serial->isOpen())
        {
            new_win->serial_check();
            new_win->show();
        }
        else            //串口发送有效数据
        {
            serial->write("start");
            serial_end();
        }
    }
    //====================================================================
   //exce_1=0;
    pre_hour=1;
   if (exce_2!=0)
   exce_2=exce_2+1;
  // exce_3=0;
   //exce_4=0;
   //exce_5=0;
}

void MainWindow::graph_down_replot_2()
{
    //准备数据
        int j;
        int j_1;
        if(graph_para[0].state==1)//第一张图数据准备
        {
            for(j=0;j<100;j++)
            {
                if(graph_para[0].gra_scan_row==data_temp[0].down_valid_row_num)//数据读完
                {
                    if(graph_para[0].gra_loop_times>0)             //若最后一行为循环头
                    {
                        if(graph_para[0].gra_loop_times!=9999)
                            graph_para[0].gra_loop_times--;
                        if(graph_para[0].gra_loop_times!=0) //避免非9999的循环体无限循环
                        {
                            graph_para[0]. gra_scan_row =   graph_para[0].gra_loop_head_row;   //扫描行数回到循环头的行数
                            graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                            graph_para[0].gra_last_time[j]=graph_para[0].final_x;
                            graph_para[0].gra_volt[j]=data_temp[0].volt[graph_para[0].gra_scan_row];
                            graph_para[0]. gra_scan_row++;
                        }
                        else
                        {
                            graph_para[0].gra_node_num=j;   //记录点数
                            graph_para[0].state=0;
                            break;
                        }
                    }
                    else                //若最后一行为正常数据
                    {
                        graph_para[0].gra_node_num=j;   //记录点数
                        graph_para[0].state=0;
                        break;
                    }
                }
                else
                {
                    if(data_temp[0].loop_times[graph_para[0].gra_scan_row]==0)  //非循环数据，直接存储,扫描行数+1
                    {
                       graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                       graph_para[0].gra_last_time[j]=graph_para[0].final_x;
                       graph_para[0].gra_volt[j]=data_temp[0].volt[graph_para[0].gra_scan_row];
                       graph_para[0]. gra_scan_row++;
                       if(graph_para[0].gra_loop_times==0)     //循环结束，记录循环次数
                       {
                           graph_para[0].gra_loop_times=data_temp[0].loop_times[graph_para[0].gra_scan_row];
                       }
                    }
                    else if(data_temp[0].loop_times[graph_para[0].gra_scan_row] != -1)//循环数据头，存储数据,记录循环次数
                    {
                        graph_para[0].gra_loop_head_row=graph_para[0].gra_scan_row;//记录循环体头的行数
                        if(graph_para[0].gra_loop_times==0)
                            graph_para[0].gra_loop_times=data_temp[0].loop_times[graph_para[0].gra_scan_row];
                        graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                        graph_para[0].gra_last_time[j]=graph_para[0].final_x;
                        graph_para[0].gra_volt[j]=data_temp[0].volt[graph_para[0].gra_scan_row];
                        graph_para[0]. gra_scan_row++;
                    }else if(data_temp[0].loop_times[graph_para[0].gra_scan_row] == -1)     //循环数据，记录数据，行数+1
                    {
                        graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                        graph_para[0].gra_last_time[j]=graph_para[0].final_x;
                        graph_para[0].gra_volt[j]=data_temp[0].volt[graph_para[0].gra_scan_row];
                        graph_para[0]. gra_scan_row++;
                        if(graph_para[0].gra_scan_row==data_temp[0].down_valid_row_num)  //如果到了文件末尾
                        {
                            if(graph_para[0].gra_loop_times!=0) //循环未结束，记录循环次数
                            {
                                if(graph_para[0].gra_loop_times!=9999)
                                    graph_para[0].gra_loop_times--;
                                if(graph_para[0].gra_loop_times==0)
                                {
                                    graph_para[0].gra_node_num=j+1;   //记录点数
                                   // graph_para[0].initial=1;
                                    break;
                                }
                                graph_para[0]. gra_scan_row =   graph_para[0].gra_loop_head_row;   //扫描行数回到循环头的行数
                            }
                            else //如果是最后一行，且循环结束，那么就跳出
                            {
                                graph_para[0].gra_node_num=j+1;   //记录点数
                               //graph_para[0].initial=1;
                                break;
                            }
                        }
                        else if(data_temp[0].loop_times[graph_para[0].gra_scan_row] !=-1)    //执行到循环最后一行
                        {
                            if(graph_para[0].gra_loop_times!=0) //循环未结束，记录循环次数
                            {
                                graph_para[0].gra_loop_times--;
                                if(graph_para[0].gra_loop_times!=0)
                                    graph_para[0]. gra_scan_row =   graph_para[0].gra_loop_head_row;   //扫描行数回到循环头的行数
                            }
                        }
                        else            //如果仍然是-1，判断是否是文件结尾
                        {
                            if(graph_para[0].gra_scan_row==data_temp[0].down_valid_row_num)
                            {
                                graph_para[0].gra_node_num=j+1;   //记录点数
                               // graph_para[0].state=0;
                                break;
                            }
                        }

                    }
                    else//错误数据，之后需补充检测数据输入的函数，错误报告异常
                    {
                        ;
                    }
                }
                graph_para[0].gra_node_num++;//正常循环个数+1
            }
            //自动开启刷新未写完的数据
    //        if(graph_para[0].gra_node_num==100)
    //            graph_para[0].state=2;
             graph_para[0].state=INITIAL_FINISH;//第一次刷新完成
           //  graph_para[1].state=1;//允许接受单片机数据
        }else       //后续点移入移出
        {
            if(graph_para[0].gra_scan_row==data_temp[0].down_valid_row_num)//数据读完
            {
                if(graph_para[0].gra_loop_times!=0)             //若最后一行为循环头
                {
                    if(graph_para[0].state==2)
                    {
                        for(j_1=0;j_1<99;j_1++){
                            graph_para[0].gra_last_time[j_1]=graph_para[0].gra_last_time[j_1+1];
                            graph_para[0].gra_volt[j_1]=graph_para[0].gra_volt[j_1+1];
                        }
                        if(graph_para[0].gra_loop_times!=9999)
                        {
                            graph_para[0].gra_loop_times--;
                        }
                        if(graph_para[0].gra_loop_times!=0)
                        {
                            graph_para[0]. gra_scan_row =   graph_para[0].gra_loop_head_row;   //扫描行数回到循环头的行数
                            graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                            graph_para[0].gra_last_time[99]=graph_para[0].final_x;
                            graph_para[0].gra_volt[99]=data_temp[0].volt[graph_para[0].gra_scan_row];
                            graph_para[0].gra_scan_row++;
                        }
                    }
                }
                else                //若最后一行为正常数据
                {
                    graph_para[0].state=0;
                }
            }
            else if(graph_para[0].state==2)          //继续执行
            {
                for(j_1=0;j_1<99;j_1++){
                    graph_para[0].gra_last_time[j_1]=graph_para[0].gra_last_time[j_1+1];
                    graph_para[0].gra_volt[j_1]=graph_para[0].gra_volt[j_1+1];
                }
                if(data_temp[0].loop_times[graph_para[0].gra_scan_row]==0)  //非循环数据，直接存储,扫描行数+1
                {
                   graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                   graph_para[0].gra_last_time[99]=graph_para[0].final_x;
                   graph_para[0].gra_volt[99]=data_temp[0].volt[graph_para[0].gra_scan_row];
                   graph_para[0]. gra_scan_row++;
                   if(graph_para[0].gra_loop_times==0)     //循环结束，记录循环次数
                   {
                       graph_para[0].gra_loop_times=data_temp[0].loop_times[graph_para[0].gra_scan_row];
                   }
                }
                else if(data_temp[0].loop_times[graph_para[0].gra_scan_row] != -1)//循环数据头，存储数据,记录循环次数
                {
                    graph_para[0].gra_loop_head_row=graph_para[0].gra_scan_row;//记录循环体头的行数
                    if(graph_para[0].gra_loop_times==0)
                        graph_para[0].gra_loop_times=data_temp[0].loop_times[graph_para[0].gra_scan_row];
                    graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                    graph_para[0].gra_last_time[99]=graph_para[0].final_x;
                    graph_para[0].gra_volt[99]=data_temp[0].volt[graph_para[0].gra_scan_row];
                    graph_para[0]. gra_scan_row++;
                }else if(data_temp[0].loop_times[graph_para[0].gra_scan_row] == -1)     //循环数据，记录数据，行数+1
                {
                    graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                    graph_para[0].gra_last_time[99]=graph_para[0].final_x;
                    graph_para[0].gra_volt[99]=data_temp[0].volt[graph_para[0].gra_scan_row];
                    graph_para[0]. gra_scan_row++;
                    if(graph_para[0].gra_scan_row==data_temp[0].down_valid_row_num)  //如果文件结尾，跳出
                    {
                        if(graph_para[0].gra_loop_times!=0) //循环未结束，记录循环次数
                        {
                            graph_para[0].gra_loop_times--;
                            if(graph_para[0].gra_loop_times!=0)
                            {
                                graph_para[0]. gra_scan_row =   graph_para[0].gra_loop_head_row;   //扫描行数回到循环头的行数
                            }
                        }
                    }
                    else if(data_temp[0].loop_times[graph_para[0].gra_scan_row] !=-1)    //执行到循环最后一行
                    {
                        if(graph_para[0].gra_loop_times!=0) //循环未结束，记录循环次数
                        {
                            graph_para[0].gra_loop_times--;
                            if(graph_para[0].gra_loop_times!=0)
                                graph_para[0]. gra_scan_row =   graph_para[0].gra_loop_head_row;   //扫描行数回到循环头的行数
                        }
                    }
                }
            }
            else if(graph_para[0].state==0)//无需刷新
            {
                ;
            }
            else if(graph_para[0].state==3)//初始化结束
            {
                  graph_para[0].state=0;  //暂停刷新
            }
        }
    //清零实际图像
        if(graph_para[1].state==RESET)
        {
        //清零实际图像
                QVector<QPointF> valid_curve;
                QVector<QPointF> error_curve;       //误差曲线
                QPointF point;
                QPointF point_error;
                for(int i =0;i<100;i++){
                       // graph_para[0].final_x += (float)graph_para[0].gra_last_time[i]/1000.0;
                        point.setX(i);
                        point.setY(0);
                        valid_curve.append(point);
                }
                for(int i =0;i<100;i++){//误差曲线
                       // graph_para[0].final_x += (float)graph_para[0].gra_last_time[i]/1000.0;
                        point_error.setX(i);
                        point_error.setY(0);
                        error_curve.append(point_error);
                }
                QwtPointSeriesData* valid_curve_r = new QwtPointSeriesData(valid_curve);
                QwtPointSeriesData* error_curve_r = new QwtPointSeriesData(error_curve);//误差曲线
                valid_curve_t_2->setData(valid_curve_r);
                error_curve_t_2->setData(error_curve_r);//误差曲线
                if(graph_para[1].gra_node_num==100)
                {
                    ui->data_plot_2->setAxisAutoScale(QwtPlot::xBottom, false);
                    ui->data_plot_2->setAxisScale(QwtPlot::xBottom,0,10,1);
                    ui->data_plot_2->replot();
                    ui->error_plot_2->setAxisAutoScale(QwtPlot::xBottom, false);//误差曲线
                    ui->error_plot_2->setAxisScale(QwtPlot::xBottom,0,10,1);
                    ui->error_plot_2->replot();
                }
                ui->data_plot_2->replot();
                ui->error_plot_2->replot();
                graph_para[1].state=STOP;
        }
    //显示数据
        if(graph_para[0].state)
        {
            //rule_extend();
            QVector<QPointF> design_curve;
            QPointF point;
            for(int i =0;i<graph_para[0].gra_node_num;i++){
                    point.setX(float(graph_para[0].gra_last_time[i]));
                    point.setY((float)graph_para[0].gra_volt[i]);
                    design_curve.append(point);
            }
            QwtPointSeriesData* design_curve_r = new QwtPointSeriesData(design_curve);
            design_curve_t_2->setData(design_curve_r);
            ui->data_plot_2->replot();
            //qDebug()<<design_curve.data()->x()<<""<<design_curve.data()->y();
        }
}

void MainWindow::test_text_2()
{
   qDebug()<<"NO.2 chart:";
    QModelIndex indextemp_2;
    float temp;
    int i=0;
    int j=0;
    for( i = 0;i<ui->tableView_2->model()->rowCount();i++)
    {
        for( j=0;j<3;j++)
        {
            indextemp_2 = ui->tableView_2->model()->index(i,j);
            temp=ui->tableView_2->model()->data(indextemp_2).toFloat();
            datatemp[i][j]=temp;
        }
        if((datatemp[i][0]==-1)&&(datatemp[i][1]==-1)&&(datatemp[i][2]==-1))
        {
            data_temp[0].down_valid_row_num=i;  //下传数据有效行数
            break;
        }
    }
    //

    int k=0;
    int flag=0;
    //第一个循环用来寻找第一个循环头
    for(int i=0;i<data_temp[0].down_valid_row_num;i++)
    {
        if (datatemp[i][2]<=-1)
        {
                qDebug()<<"there is a mistake:"<<i+1;
                flag=1;
                break;
        }
        else
        {
            if (datatemp[i][2]>0)
            {
                k=i;
                break;
            }
        }
    }

    //以下才是真正的判断循环体的正确性。当然，零的存在打断了部分循环。而且为了简化最后一个else导致复杂化了仲间的两个if判断
    for(i=k+1;i<data_temp[0].down_valid_row_num;i++)
    {
        if(flag) break;
        else
        {
            if (i==k+1)
            {
                if (datatemp[k][2]>0)
                {
                    if (datatemp[i][2]>=0||datatemp[i][2]<-1)
                    {
                        qDebug()<<"there is a mistake:"<<i+1;
                        break;
                    }
                    else if (i==data_temp[0].down_valid_row_num-1)
                    {
                        qDebug()<<"all is correct";
                    }
                }
                else
                {
                    for (i=k+1;i<data_temp[0].down_valid_row_num;i++)
                    {
                        if (datatemp[i][2]<=-1)
                        {
                                qDebug()<<"there is a mistake:"<<i+1;
                                flag=1;
                                break;
                        }
                        else if (datatemp[i][2]>0)
                        {
                            if (i!=data_temp[0].down_valid_row_num-1)
                            {
                                k=i;
                                break;
                            }
                            else
                            {
                                qDebug()<<"the last row is wrong and so pity";
                                flag=1;
                                break;
                            }
                        }
                        else if(datatemp[i][2]==0&&i==data_temp[0].down_valid_row_num-1)
                        qDebug()<<"all is correct";
                    }
                }

            }
            else if (i==data_temp[0].down_valid_row_num-1)
            {
                if (datatemp[i-1][2]==-1)
                {
                    if (datatemp[i][2]==-1||datatemp[i][2]==0) qDebug()<<"all is correct";
                    else
                    {
                        qDebug()<<"the last row is wrong and so pity";
                        break;
                    }
                }
                else if (datatemp[i-1][2]==0)
                {
                    if (datatemp[i][2]==0) qDebug()<<"all is correct";
                    else
                    {
                        qDebug()<<"the last row is wrong and so pity";
                        break;
                    }
                }
                else
                {
                    if (datatemp[i][2]==-1) qDebug()<<"all is correct";
                    else
                    {
                        qDebug()<<"the last row is wrong and so pity";
                        break;
                    }
                }
            }
            else
            {
                if(datatemp[i][2]<-1)
                {
                    qDebug()<<"there is a mistake:"<<i+1;
                    break;
                }
                else
                {
                    if(datatemp[i][2]!=-1)
                    {
                        k=i;
                        qDebug()<<"current circle is correct"<<i;
                    }
                }
            }
        }
    }
}

void MainWindow::on_clear_3_clicked()
{
    auto_state = OFF;
    QModelIndex indextemp;
    for(int i=0;i<ui->tableView_3->model()->rowCount();i++)        //清零表格中的数据
    {
        for(int j=0;j<3;j++)
        {
            indextemp = ui->tableView_3->model()->index(i,j);
            ui->tableView_3->model()->setData(indextemp,0.0);
        }
    }
    memset(graph_para[0].gra_volt, 0, 100*sizeof(float) );
    memset(graph_para[1].gra_volt, 0, 100*sizeof(float) );
    graph_para[0].state=INITIAL;
    graph_para[0].gra_scan_row=0;
    data_temp[0].down_valid_row_num=0;
    //修复bug，清除上一幅图像的数据和本地缓存
    memset(data_temp[0].loop_times,0,100*sizeof(int));

    //请曲线名
    ui->tabWidget->setTabText(2,"tab_3");

    //清LCDnumber
    ui->lcdNumber_9->display(0);
    ui->lcdNumber_7->display(0);
    ui->lcdNumber_8->display(0);
    ui->lcdNumber_22->display(0);
    ui->lcdNumber_23->display(0);
    ui->lcdNumber_28->display(0);//=================

    graph_para[1].state=RESET;
    graph_para[1].final_x=0;
    graph_down_replot_3();

    //数据图坐标更新回复
    ui->data_plot_3->setAxisScale(QwtPlot::yLeft,-5,5,1);
    ui->data_plot_3->setAxisScale(QwtPlot::xBottom,0,10,1);
    QVector<QPointF> rule;                   //数据参考线
    for(int i =-1;i<101;i++){
        QPointF point;
        point.setX(i);
        int y = 0;
        point.setY(y);
        rule.append(point);
    }
    QwtPointSeriesData* rule_c = new QwtPointSeriesData(rule);
    rule_t_3 = new QwtPlotCurve("Curve 0");
    rule_t_3->setData(rule_c);
    rule_t_3->attach(ui->data_plot_3);

    QVector<QPointF> rule_error;             //误差参考线
    for(int i =-1;i<101;i++){
        QPointF point_error;
        point_error.setX(i);
        int y = 0;
        point_error.setY(y);
        rule_error.append(point_error);
    }
    QwtPointSeriesData* rule_error_c = new QwtPointSeriesData(rule_error);
    rule_error_t_3 = new QwtPlotCurve("Curve 0");
    rule_error_t_3->setData(rule_error_c);
    rule_error_t_3->attach(ui->error_plot_3);

    valid_curve_t_3->setVisible(false);            //隐藏实际曲线
    ui->error_plot_3->replot();
    error_curve_t_3->setVisible(false);            //隐藏误差曲线
    ui->error_plot_3->replot();
    design_curve_t_3->setVisible(false);            //隐藏误差曲线
    ui->error_plot_3->replot();
    QVector<QPointF> rule_a;
    QVector<QPointF> rule_error_a;//误差曲线
    for(int i =0;i<100;i++){
        QPointF point;
        QPointF point_error;            //误差曲线
        point.setX(i);
        point_error.setX(i);            //误差曲线
        point.setY(0);
        point_error.setY(0);            //误差曲线
        rule_a.append(point);
        rule_error_a.append(point_error);//误差曲线
    }
    QwtPointSeriesData* rule_r = new QwtPointSeriesData(rule_a);
    rule_t_3->setData(rule_r);
    QwtPointSeriesData* rule_error_r = new QwtPointSeriesData(rule_error_a);//误差曲线
    rule_error_t_3->setData(rule_error_r);//误差曲线
    ui->data_plot_3->replot();
    ui->error_plot_3->replot();
    stop_excute();

}

void MainWindow::on_download_3_clicked()
{
    if(local_state!=AUTO)
    {
        QModelIndex indextemp;
        float temp;
        int i=0;
        int j=0;
        for( i = 0;i<ui->tableView_3->model()->rowCount();i++)
        {
            for( j=0;j<3;j++)
            {
                indextemp = ui->tableView_3->model()->index(i,j);
                temp=ui->tableView_3->model()->data(indextemp).toFloat();
                datatemp[i][j]=temp;
            }
            if((datatemp[i][0]==-1)&&(datatemp[i][1]==-1)&&(datatemp[i][2]==-1))
            {
                data_temp[0].down_valid_row_num=i;  //下传数据有效行数
               //valid_row_number=i;
                break;
            }
        }
        char ctemp[0];
        if(!serial->isOpen())
        {
            new_win->serial_check();
            new_win->show();
        }
        else            //串口发送有效数据
        {
            sprintf(&ctemp[0],"%d;",data_temp[0].down_valid_row_num);
            serial->write(&ctemp[0]);
            for(i=0;i<data_temp[0].down_valid_row_num;i++)
            {
               data_temp[0].last_time[i]=datatemp[i][0];
               sprintf(&ctemp[0],"%d;",(int)(datatemp[i][0]*1000));
               serial->write(&ctemp[0]);
               data_temp[0].volt[i]=datatemp[i][1];
               sprintf(&ctemp[0],"%d;",(int)(datatemp[i][1]*1000));
               serial->write(&ctemp[0]);
               data_temp[0].loop_times[i]=datatemp[i][2];
               sprintf(&ctemp[0],"%d;",(int)(datatemp[i][2]));
               serial->write(&ctemp[0]);
            }
            serial_end();//发送给单片机的终止符

            graph_para[0].state=INITIAL;
            graph_para[0].gra_scan_row=0;  //扫描行数回到开头
            graph_para[0].gra_node_num=0;     //图中节点数
            graph_para[0].final_x=0;          //最后一点坐标
            graph_para[0].gra_loop_times=0;  //记录各循环节点剩余次数
            graph_para[0].gra_scan_row=0;            //图像扫描的行数
            memset(graph_para[0].gra_last_time,0,100*sizeof(float));  //准备显示电压持续时间
            memset(graph_para[0].gra_volt,0,100*sizeof(float));  //准备电压值
            graph_para[0].gra_loop_head_row=0;   //循环的开头行数
            design_curve_t_3->setVisible(true);//====================================
            //清空实际图像
            graph_para[1].state=RESET;
            graph_down_replot_3();
            //设计图形初始化完成
            graph_para[0].state=DOWN_FINISH;

            //复原坐标轴
            ui->data_plot_3->setAxisAutoScale(QwtPlot::xBottom, false);
            ui->data_plot_3->setAxisScale(QwtPlot::xBottom,0,10,1);
            ui->data_plot_3->replot();
            ui->error_plot_3->setAxisAutoScale(QwtPlot::xBottom, false);
            ui->error_plot_3->setAxisScale(QwtPlot::xBottom,0,10,1);
            ui->error_plot_3->replot();
            QVector<QPointF> rule_a;
            //补充中间参考线长度
            for(int i =0;i<101;i++){
                QPointF point;
                point.setX(i);
                int y = 0;
                point.setY(y);
                rule_a.append(point);
            }
            QwtPointSeriesData* rule_r = new QwtPointSeriesData(rule_a);
            rule_t_3->setData(rule_r);
            ui->data_plot_3->replot();
            QVector<QPointF> rule_error_a;
            for(int i =0;i<101;i++){
                QPointF point;
                point.setX(i);
                int y = 0;
                point.setY(y);
                rule_error_a.append(point);
            }
            QwtPointSeriesData* rule_error_r = new QwtPointSeriesData(rule_error_a);
            rule_error_t_3->setData(rule_error_r);
            ui->error_plot_3->replot();
            //隐藏实际曲线
            valid_curve_t_3->setVisible(false);
            error_curve_t_3->setVisible(false);
            ui->data_plot_3->replot();
        }
    }
}

void MainWindow::on_save_3_clicked()
{
    if (local_state!=SAVE_ALL&&local_state!=BACKUP_ALL)//==============================
    {
            name_dialog->show();//===============================================
    }
}

void MainWindow::on_save_3_clicked_after()
{


    //backup操作
        if(local_state==BACKUP_ALL)
        {
            bool ok=dataBackup();
             qDebug()<<ok;
        }


    //save操作
        float temp;
        QModelIndex indextemp;
        if(local_state==IDLE||local_state==SAVE_ALL)
        {
            QFile file("save2.txt");
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                return;
            for(int i=0;i<ui->tableView_3->model()->rowCount();i++)
            {
                for(int j=0;j<3;j++)
                {
                    indextemp = ui->tableView_3->model()->index(i,j);
                    //datatemp[i][j] = ui->tableView->model()->data(indextemp).toInt();
                    temp=ui->tableView_3->model()->data(indextemp).toFloat();
                    datatemp[i][j]=temp;
                   // out << datatemp[i][j] << " ";
                }
                if((datatemp[i][0]==-1)&&(datatemp[i][1]==-1)&&(datatemp[i][2]==-1))
                {
                    data_temp[0].save_valid_row_num=i;   //保存数据的有效行数
                    break;
                }
               // out<<"\n";
            }
            //=====================================================
            //memset(data_temp[0].last_time,0,100*sizeof(float));
           //memset(data_temp[0].volt,0,100*sizeof(float));
           // memset(data_temp[0].loop_times,0,100*sizeof(int));
            //==================================================

            //===================================================
            QTextStream out(&file);
            out.setFieldWidth(8);
            out<<data_temp[0].save_valid_row_num;
            out<<"\n";
            out << ui->tabWidget->tabText(2);//=============================================
            out << "\n";//==================================================================
            out<<"    time      volt      loop\n";
            out<<"====================\n";
            for(int i=0;i<data_temp[0].save_valid_row_num;i++)
            {
                for(int j=0;j<3;j++)
                {
                  out << datatemp[i][j] << " ";
                }
                out<<"\n";
            }
            file.close();
        }
        //memset(datatemp,0,sizeof(float)*100*3);
        //memset(datatemp[][1],0,100*sizeof(float));
        //memset(datatemp[][2],0,100*sizeof(float));
        test_text_3();
        on_load_3_clicked();
}

void MainWindow::on_load_3_clicked()
{
    //先执行clear
        on_clear_3_clicked();
    //读取save数据
        if(local_state==IDLE||local_state==LOAD_ALL||local_state==SAVE_ALL)
        {
            int is=0;
            int js=0;
            std::ifstream file_t("save2.txt");
            std::string p;
            std::string q;
            QString name;
            file_t>>data_temp[0].load_valid_row_num;   //保存数据的有效行数;
            file_t>>q;//读取本行字放进q中，字属于string类
            name=QString::fromStdString(q);//转换为QString类型，方便下面写入
            ui->tabWidget->setTabText(2,name);//load时成功写入名字
            std::getline(file_t,p);
            std::getline(file_t,p);
            std::getline(file_t,p);
            while(!file_t.eof())        //按行读取
            {
                file_t>>datatemp[is][js++];
                file_t>>datatemp[is][js++];
                file_t>>datatemp[is++][js];
                js=0;
            }
            file_t.close();
            QModelIndex indextemp;
            for(int i=0;i<data_temp[0].load_valid_row_num+1;i++)        //读取文档中的数据显示在表格中
            {
                for(int j=0;j<3;j++)
                {
                    indextemp = ui->tableView_3->model()->index(i,j);
                    ui->tableView_3->model()->setData(indextemp,qRound(datatemp[i][j]*1000.0)/1000.0);
                    if(i == data_temp[0].load_valid_row_num)
                        ui->tableView_3->model()->setData(indextemp,-1.0);
                }

            }
        }else if(local_state==RESTORE_ALL||local_state==BACKUP_ALL)
        {
        //读取save_backup数据
            int is_back=0;
            int js_back=0;
            std::ifstream file_t_back("save_backup2.txt");
            std::string p;
            std::string q;
            QString name;
            file_t_back>>data_temp[0].load_valid_row_num;   //保存数据的有效行数;
            file_t_back>>q;//读取本行字放进q中，字属于string类
            name=QString::fromStdString(q);//转换为QString类型，方便下面写入
            ui->tabWidget->setTabText(2,name);//load时成功写入名字
            std::getline(file_t_back,p);
            std::getline(file_t_back,p);
            std::getline(file_t_back,p);
            while(!file_t_back.eof())        //按行读取
            {
                file_t_back>>datatemp[is_back][js_back++];
                file_t_back>>datatemp[is_back][js_back++];
                file_t_back>>datatemp[is_back++][js_back];
                js_back=0;
            }
            file_t_back.close();
            QModelIndex indextemp_back;
            for(int i=0;i<data_temp[0].load_valid_row_num+1;i++)        //读取文档中的数据显示在表格中
            {
                for(int j=0;j<3;j++)
                {
                    indextemp_back = ui->tableView_3->model()->index(i,j);
                    ui->tableView_3->model()->setData(indextemp_back,qRound(datatemp[i][j]*1000.0)/1000.0);
                    if(i == data_temp[0].load_valid_row_num)
                        ui->tableView_3->model()->setData(indextemp_back,-1.0);
                }

            }
        }
        if(local_state!=AUTO)       //不在AUTO模式下，允许画图
        {
            design_curve_t_3->setVisible(true);
            //绘制图像
            QModelIndex indextemp_1;
            float temp;
            int i=0;
            int j=0;
            for( i = 0;i<ui->tableView_3->model()->rowCount();i++)
            {
                for( j=0;j<3;j++)
                {
                    indextemp_1 = ui->tableView_3->model()->index(i,j);
                    temp=ui->tableView_3->model()->data(indextemp_1).toFloat();
                    datatemp[i][j]=temp;
                }
                if((datatemp[i][0]==-1)&&(datatemp[i][1]==-1)&&(datatemp[i][2]==-1))
                {
                    data_temp[0].down_valid_row_num=i;  //下传数据有效行数
                    break;
                }
            }
            for(i=0;i<data_temp[0].down_valid_row_num;i++)
            {
               data_temp[0].last_time[i]=datatemp[i][0];
               data_temp[0].volt[i]=datatemp[i][1];
               data_temp[0].loop_times[i]=datatemp[i][2];
            }
            graph_para[0].state=INITIAL;
            graph_para[0].gra_scan_row=0;  //扫描行数回到开头
            graph_para[0].gra_node_num=0;     //图中节点数
            graph_para[0].final_x=0;          //最后一点坐标
            graph_para[0].gra_loop_times=0;  //记录各循环节点剩余次数
            graph_para[0].gra_scan_row=0;            //图像扫描的行数
            memset(graph_para[0].gra_last_time,0,100*sizeof(float));  //准备显示电压持续时间
            memset(graph_para[0].gra_volt,0,100*sizeof(float));  //准备电压值
            graph_para[0].gra_loop_head_row=0;   //循环的开头行数
            //隐藏实际曲线
            //valid_curve_t_3->setPen(QColor(0,0,0),2,Qt::SolidLine);
            valid_curve_t_3->setVisible(false);
            ui->data_plot_3->replot();
            //清空实际图像
            graph_para[1].state=RESET;
            graph_down_replot_3();
            //load时显示最后一个坐标，变换坐标轴
            if(graph_para[0].final_x>10)
            {
                ui->data_plot_3->setAxisAutoScale(QwtPlot::xBottom, false);
                ui->data_plot_3->setAxisScale(QwtPlot::xBottom,0,graph_para[0].final_x,graph_para[0].final_x/10);
                ui->data_plot_3->replot();
            }
            else
            {
                ui->data_plot_3->setAxisAutoScale(QwtPlot::xBottom, false);
                ui->data_plot_3->setAxisScale(QwtPlot::xBottom,0,100,10);
                ui->data_plot_3->replot();
            }
            //补充中间参考线长度
            QVector<QPointF> rule_a;
            if(graph_para[0].final_x>100)
            {
                for(int i =0;i<(int)graph_para[0].final_x+100;i++){
                    QPointF point;
                    point.setX(i);
                    int y = 0;
                    point.setY(y);
                    rule_a.append(point);
                }
            }
            else
            {
                for(int i =0;i<100;i++){
                    QPointF point;
                    point.setX(i);
                    int y = 0;
                    point.setY(y);
                    rule_a.append(point);
                }
            }
            QwtPointSeriesData* rule_r = new QwtPointSeriesData(rule_a);
            rule_t_3->setData(rule_r);
            ui->data_plot_3->replot();

        }
}

void MainWindow::on_excute_3_clicked()
{
    //清空实际图像
    if(graph_para[0].state==DOWN_FINISH)     //download完成
    {
        graph_para[1].state=INITIAL;
        graph_para[1].gra_scan_row=0;  //扫描行数回到开头
        graph_para[1].gra_node_num=0;     //图中节点数
        graph_para[1].final_x=0;          //最后一点坐标
        graph_para[1].gra_loop_times=0;  //记录各循环节点剩余次数
        graph_para[1].gra_scan_row=0;            //图像扫描的行数
        memset(graph_para[1].gra_last_time,0,100*sizeof(float));  //准备显示电压持续时间
        memset(graph_para[1].gra_volt,0,100*sizeof(float));  //准备电压值
        graph_para[1].gra_loop_head_row=0;   //循环的开头行数

        if(!serial->isOpen())
        {
            new_win->serial_check();
            new_win->show();
        }
        else            //串口发送有效数据
        {
            serial->write("start");
            serial_end();
        }
    }
    //====================================================================
   //exce_1=0;
  // exce_2=0;
    pre_hour=1;
    //
    if (exce_3!=0)
  exce_3=exce_3+1;
  // exce_4=0;
   //exce_5=0;
}

void MainWindow::graph_down_replot_3()
{
    //准备数据
        int j;
        int j_1;
        if(graph_para[0].state==1)//第一张图数据准备
        {
            for(j=0;j<100;j++)
            {
                if(graph_para[0].gra_scan_row==data_temp[0].down_valid_row_num)//数据读完
                {
                    if(graph_para[0].gra_loop_times>0)             //若最后一行为循环头
                    {
                        if(graph_para[0].gra_loop_times!=9999)
                            graph_para[0].gra_loop_times--;
                        if(graph_para[0].gra_loop_times!=0) //避免非9999的循环体无限循环
                        {
                            graph_para[0]. gra_scan_row =   graph_para[0].gra_loop_head_row;   //扫描行数回到循环头的行数
                            graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                            graph_para[0].gra_last_time[j]=graph_para[0].final_x;
                            graph_para[0].gra_volt[j]=data_temp[0].volt[graph_para[0].gra_scan_row];
                            graph_para[0]. gra_scan_row++;
                        }
                        else
                        {
                            graph_para[0].gra_node_num=j;   //记录点数
                            graph_para[0].state=0;
                            break;
                        }
                    }
                    else                //若最后一行为正常数据
                    {
                        graph_para[0].gra_node_num=j;   //记录点数
                        graph_para[0].state=0;
                        break;
                    }
                }
                else
                {
                    if(data_temp[0].loop_times[graph_para[0].gra_scan_row]==0)  //非循环数据，直接存储,扫描行数+1
                    {
                       graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                       graph_para[0].gra_last_time[j]=graph_para[0].final_x;
                       graph_para[0].gra_volt[j]=data_temp[0].volt[graph_para[0].gra_scan_row];
                       graph_para[0]. gra_scan_row++;
                       if(graph_para[0].gra_loop_times==0)     //循环结束，记录循环次数
                       {
                           graph_para[0].gra_loop_times=data_temp[0].loop_times[graph_para[0].gra_scan_row];
                       }
                    }
                    else if(data_temp[0].loop_times[graph_para[0].gra_scan_row] != -1)//循环数据头，存储数据,记录循环次数
                    {
                        graph_para[0].gra_loop_head_row=graph_para[0].gra_scan_row;//记录循环体头的行数
                        if(graph_para[0].gra_loop_times==0)
                            graph_para[0].gra_loop_times=data_temp[0].loop_times[graph_para[0].gra_scan_row];
                        graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                        graph_para[0].gra_last_time[j]=graph_para[0].final_x;
                        graph_para[0].gra_volt[j]=data_temp[0].volt[graph_para[0].gra_scan_row];
                        graph_para[0]. gra_scan_row++;
                    }else if(data_temp[0].loop_times[graph_para[0].gra_scan_row] == -1)     //循环数据，记录数据，行数+1
                    {
                        graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                        graph_para[0].gra_last_time[j]=graph_para[0].final_x;
                        graph_para[0].gra_volt[j]=data_temp[0].volt[graph_para[0].gra_scan_row];
                        graph_para[0]. gra_scan_row++;
                        if(graph_para[0].gra_scan_row==data_temp[0].down_valid_row_num)  //如果到了文件末尾
                        {
                            if(graph_para[0].gra_loop_times!=0) //循环未结束，记录循环次数
                            {
                                if(graph_para[0].gra_loop_times!=9999)
                                    graph_para[0].gra_loop_times--;
                                if(graph_para[0].gra_loop_times==0)
                                {
                                    graph_para[0].gra_node_num=j+1;   //记录点数
                                   // graph_para[0].initial=1;
                                    break;
                                }
                                graph_para[0]. gra_scan_row =   graph_para[0].gra_loop_head_row;   //扫描行数回到循环头的行数
                            }
                            else //如果是最后一行，且循环结束，那么就跳出
                            {
                                graph_para[0].gra_node_num=j+1;   //记录点数
                               //graph_para[0].initial=1;
                                break;
                            }
                        }
                        else if(data_temp[0].loop_times[graph_para[0].gra_scan_row] !=-1)    //执行到循环最后一行
                        {
                            if(graph_para[0].gra_loop_times!=0) //循环未结束，记录循环次数
                            {
                                graph_para[0].gra_loop_times--;
                                if(graph_para[0].gra_loop_times!=0)
                                    graph_para[0]. gra_scan_row =   graph_para[0].gra_loop_head_row;   //扫描行数回到循环头的行数
                            }
                        }
                        else            //如果仍然是-1，判断是否是文件结尾
                        {
                            if(graph_para[0].gra_scan_row==data_temp[0].down_valid_row_num)
                            {
                                graph_para[0].gra_node_num=j+1;   //记录点数
                               // graph_para[0].state=0;
                                break;
                            }
                        }

                    }
                    else//错误数据，之后需补充检测数据输入的函数，错误报告异常
                    {
                        ;
                    }
                }
                graph_para[0].gra_node_num++;//正常循环个数+1
            }
            //自动开启刷新未写完的数据
    //        if(graph_para[0].gra_node_num==100)
    //            graph_para[0].state=2;
             graph_para[0].state=INITIAL_FINISH;//第一次刷新完成
           //  graph_para[1].state=1;//允许接受单片机数据
        }else       //后续点移入移出
        {
            if(graph_para[0].gra_scan_row==data_temp[0].down_valid_row_num)//数据读完
            {
                if(graph_para[0].gra_loop_times!=0)             //若最后一行为循环头
                {
                    if(graph_para[0].state==2)
                    {
                        for(j_1=0;j_1<99;j_1++){
                            graph_para[0].gra_last_time[j_1]=graph_para[0].gra_last_time[j_1+1];
                            graph_para[0].gra_volt[j_1]=graph_para[0].gra_volt[j_1+1];
                        }
                        if(graph_para[0].gra_loop_times!=9999)
                        {
                            graph_para[0].gra_loop_times--;
                        }
                        if(graph_para[0].gra_loop_times!=0)
                        {
                            graph_para[0]. gra_scan_row =   graph_para[0].gra_loop_head_row;   //扫描行数回到循环头的行数
                            graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                            graph_para[0].gra_last_time[99]=graph_para[0].final_x;
                            graph_para[0].gra_volt[99]=data_temp[0].volt[graph_para[0].gra_scan_row];
                            graph_para[0].gra_scan_row++;
                        }
                    }
                }
                else                //若最后一行为正常数据
                {
                    graph_para[0].state=0;
                }
            }
            else if(graph_para[0].state==2)          //继续执行
            {
                for(j_1=0;j_1<99;j_1++){
                    graph_para[0].gra_last_time[j_1]=graph_para[0].gra_last_time[j_1+1];
                    graph_para[0].gra_volt[j_1]=graph_para[0].gra_volt[j_1+1];
                }
                if(data_temp[0].loop_times[graph_para[0].gra_scan_row]==0)  //非循环数据，直接存储,扫描行数+1
                {
                   graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                   graph_para[0].gra_last_time[99]=graph_para[0].final_x;
                   graph_para[0].gra_volt[99]=data_temp[0].volt[graph_para[0].gra_scan_row];
                   graph_para[0]. gra_scan_row++;
                   if(graph_para[0].gra_loop_times==0)     //循环结束，记录循环次数
                   {
                       graph_para[0].gra_loop_times=data_temp[0].loop_times[graph_para[0].gra_scan_row];
                   }
                }
                else if(data_temp[0].loop_times[graph_para[0].gra_scan_row] != -1)//循环数据头，存储数据,记录循环次数
                {
                    graph_para[0].gra_loop_head_row=graph_para[0].gra_scan_row;//记录循环体头的行数
                    if(graph_para[0].gra_loop_times==0)
                        graph_para[0].gra_loop_times=data_temp[0].loop_times[graph_para[0].gra_scan_row];
                    graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                    graph_para[0].gra_last_time[99]=graph_para[0].final_x;
                    graph_para[0].gra_volt[99]=data_temp[0].volt[graph_para[0].gra_scan_row];
                    graph_para[0]. gra_scan_row++;
                }else if(data_temp[0].loop_times[graph_para[0].gra_scan_row] == -1)     //循环数据，记录数据，行数+1
                {
                    graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                    graph_para[0].gra_last_time[99]=graph_para[0].final_x;
                    graph_para[0].gra_volt[99]=data_temp[0].volt[graph_para[0].gra_scan_row];
                    graph_para[0]. gra_scan_row++;
                    if(graph_para[0].gra_scan_row==data_temp[0].down_valid_row_num)  //如果文件结尾，跳出
                    {
                        if(graph_para[0].gra_loop_times!=0) //循环未结束，记录循环次数
                        {
                            graph_para[0].gra_loop_times--;
                            if(graph_para[0].gra_loop_times!=0)
                            {
                                graph_para[0]. gra_scan_row =   graph_para[0].gra_loop_head_row;   //扫描行数回到循环头的行数
                            }
                        }
                    }
                    else if(data_temp[0].loop_times[graph_para[0].gra_scan_row] !=-1)    //执行到循环最后一行
                    {
                        if(graph_para[0].gra_loop_times!=0) //循环未结束，记录循环次数
                        {
                            graph_para[0].gra_loop_times--;
                            if(graph_para[0].gra_loop_times!=0)
                                graph_para[0]. gra_scan_row =   graph_para[0].gra_loop_head_row;   //扫描行数回到循环头的行数
                        }
                    }
                }
            }
            else if(graph_para[0].state==0)//无需刷新
            {
                ;
            }
            else if(graph_para[0].state==3)//初始化结束
            {
                  graph_para[0].state=0;  //暂停刷新
            }
        }
    //清零实际图像
        if(graph_para[1].state==RESET)
        {
        //清零实际图像
                QVector<QPointF> valid_curve;
                QVector<QPointF> error_curve;       //误差曲线
                QPointF point;
                QPointF point_error;
                for(int i =0;i<100;i++){
                       // graph_para[0].final_x += (float)graph_para[0].gra_last_time[i]/1000.0;
                        point.setX(i);
                        point.setY(0);
                        valid_curve.append(point);
                }
                for(int i =0;i<100;i++){//误差曲线
                       // graph_para[0].final_x += (float)graph_para[0].gra_last_time[i]/1000.0;
                        point_error.setX(i);
                        point_error.setY(0);
                        error_curve.append(point_error);
                }
                QwtPointSeriesData* valid_curve_r = new QwtPointSeriesData(valid_curve);
                QwtPointSeriesData* error_curve_r = new QwtPointSeriesData(error_curve);//误差曲线
                valid_curve_t_3->setData(valid_curve_r);
                error_curve_t_3->setData(error_curve_r);//误差曲线
                if(graph_para[1].gra_node_num==100)
                {
                    ui->data_plot_3->setAxisAutoScale(QwtPlot::xBottom, false);
                    ui->data_plot_3->setAxisScale(QwtPlot::xBottom,0,10,1);
                    ui->data_plot_3->replot();
                    ui->error_plot_3->setAxisAutoScale(QwtPlot::xBottom, false);//误差曲线
                    ui->error_plot_3->setAxisScale(QwtPlot::xBottom,0,10,1);
                    ui->error_plot_3->replot();
                }
                ui->data_plot_3->replot();
                ui->error_plot_3->replot();
                graph_para[1].state=STOP;
        }
    //显示数据
        if(graph_para[0].state)
        {
            //rule_extend();
            QVector<QPointF> design_curve;
            QPointF point;
            for(int i =0;i<graph_para[0].gra_node_num;i++){
                    point.setX(float(graph_para[0].gra_last_time[i]));
                    point.setY((float)graph_para[0].gra_volt[i]);
                    design_curve.append(point);
            }
            QwtPointSeriesData* design_curve_r = new QwtPointSeriesData(design_curve);
            design_curve_t_3->setData(design_curve_r);
            ui->data_plot_3->replot();
            //qDebug()<<design_curve.data()->x()<<""<<design_curve.data()->y();
        }
}

void MainWindow::test_text_3()
{
    qDebug()<<"NO.3 chart:";
    QModelIndex indextemp_3;
    float temp;
    int i=0;
    int j=0;
    for( i = 0;i<ui->tableView_3->model()->rowCount();i++)
    {
        for( j=0;j<3;j++)
        {
            indextemp_3 = ui->tableView_3->model()->index(i,j);
            temp=ui->tableView_3->model()->data(indextemp_3).toFloat();
            datatemp[i][j]=temp;
        }
        if((datatemp[i][0]==-1)&&(datatemp[i][1]==-1)&&(datatemp[i][2]==-1))
        {
            data_temp[0].down_valid_row_num=i;  //下传数据有效行数
            break;
        }
    }
    //

    int k=0;
    int flag=0;
    //第一个循环用来寻找第一个循环头
    for(int i=0;i<data_temp[0].down_valid_row_num;i++)
    {
        if (datatemp[i][2]<=-1)
        {
                qDebug()<<"there is a mistake:"<<i+1;
                flag=1;
                break;
        }
        else
        {
            if (datatemp[i][2]>0)
            {
                k=i;
                break;
            }
        }
    }

    //以下才是真正的判断循环体的正确性。当然，零的存在打断了部分循环。而且为了简化最后一个else导致复杂化了仲间的两个if判断
    for(i=k+1;i<data_temp[0].down_valid_row_num;i++)
    {
        if(flag) break;
        else
        {
            if (i==k+1)
            {
                if (datatemp[k][2]>0)
                {
                    if (datatemp[i][2]>=0||datatemp[i][2]<-1)
                    {
                        qDebug()<<"there is a mistake:"<<i+1;
                        break;
                    }
                    else if (i==data_temp[0].down_valid_row_num-1)
                    {
                        qDebug()<<"all is correct";
                    }
                }
                else
                {
                    for (i=k+1;i<data_temp[0].down_valid_row_num;i++)
                    {
                        if (datatemp[i][2]<=-1)
                        {
                                qDebug()<<"there is a mistake:"<<i+1;
                                flag=1;
                                break;
                        }
                        else if (datatemp[i][2]>0)
                        {
                            if (i!=data_temp[0].down_valid_row_num-1)
                            {
                                k=i;
                                break;
                            }
                            else
                            {
                                qDebug()<<"the last row is wrong and so pity";
                                flag=1;
                                break;
                            }
                        }
                        else if(datatemp[i][2]==0&&i==data_temp[0].down_valid_row_num-1)
                        qDebug()<<"all is correct";
                    }
                }

            }
            else if (i==data_temp[0].down_valid_row_num-1)
            {
                if (datatemp[i-1][2]==-1)
                {
                    if (datatemp[i][2]==-1||datatemp[i][2]==0) qDebug()<<"all is correct";
                    else
                    {
                        qDebug()<<"the last row is wrong and so pity";
                        break;
                    }
                }
                else if (datatemp[i-1][2]==0)
                {
                    if (datatemp[i][2]==0) qDebug()<<"all is correct";
                    else
                    {
                        qDebug()<<"the last row is wrong and so pity";
                        break;
                    }
                }
                else
                {
                    if (datatemp[i][2]==-1) qDebug()<<"all is correct";
                    else
                    {
                        qDebug()<<"the last row is wrong and so pity";
                        break;
                    }
                }
            }
            else
            {
                if(datatemp[i][2]<-1)
                {
                    qDebug()<<"there is a mistake:"<<i+1;
                    break;
                }
                else
                {
                    if(datatemp[i][2]!=-1)
                    {
                        k=i;
                        qDebug()<<"current circle is correct"<<i;
                    }
                }
            }
        }
    }
}

void MainWindow::on_download_4_clicked()
{
    if(local_state!=AUTO)
    {
        QModelIndex indextemp;
        float temp;
        int i=0;
        int j=0;
        for( i = 0;i<ui->tableView_4->model()->rowCount();i++)
        {
            for( j=0;j<3;j++)
            {
                indextemp = ui->tableView_4->model()->index(i,j);
                temp=ui->tableView_4->model()->data(indextemp).toFloat();
                datatemp[i][j]=temp;
            }
            if((datatemp[i][0]==-1)&&(datatemp[i][1]==-1)&&(datatemp[i][2]==-1))
            {
                data_temp[0].down_valid_row_num=i;  //下传数据有效行数
               //valid_row_number=i;
                break;
            }
        }
        char ctemp[0];
        if(!serial->isOpen())
        {
            new_win->serial_check();
            new_win->show();
        }
        else            //串口发送有效数据
        {
            sprintf(&ctemp[0],"%d;",data_temp[0].down_valid_row_num);
            serial->write(&ctemp[0]);
            for(i=0;i<data_temp[0].down_valid_row_num;i++)
            {
               data_temp[0].last_time[i]=datatemp[i][0];
               sprintf(&ctemp[0],"%d;",(int)(datatemp[i][0]*1000));
               serial->write(&ctemp[0]);
               data_temp[0].volt[i]=datatemp[i][1];
               sprintf(&ctemp[0],"%d;",(int)(datatemp[i][1]*1000));
               serial->write(&ctemp[0]);
               data_temp[0].loop_times[i]=datatemp[i][2];
               sprintf(&ctemp[0],"%d;",(int)(datatemp[i][2]));
               serial->write(&ctemp[0]);
            }
            serial_end();//发送给单片机的终止符

            graph_para[0].state=INITIAL;
            graph_para[0].gra_scan_row=0;  //扫描行数回到开头
            graph_para[0].gra_node_num=0;     //图中节点数
            graph_para[0].final_x=0;          //最后一点坐标
            graph_para[0].gra_loop_times=0;  //记录各循环节点剩余次数
            graph_para[0].gra_scan_row=0;            //图像扫描的行数
            memset(graph_para[0].gra_last_time,0,100*sizeof(float));  //准备显示电压持续时间
            memset(graph_para[0].gra_volt,0,100*sizeof(float));  //准备电压值
            graph_para[0].gra_loop_head_row=0;   //循环的开头行数
            design_curve_t_4->setVisible(true);
            //清空实际图像
            graph_para[1].state=RESET;
            graph_down_replot_4();
            //设计图形初始化完成
            graph_para[0].state=DOWN_FINISH;

            //复原坐标轴
            ui->data_plot_4->setAxisAutoScale(QwtPlot::xBottom, false);
            ui->data_plot_4->setAxisScale(QwtPlot::xBottom,0,10,1);
            ui->data_plot_4->replot();
            ui->error_plot_4->setAxisAutoScale(QwtPlot::xBottom, false);
            ui->error_plot_4->setAxisScale(QwtPlot::xBottom,0,10,1);
            ui->error_plot_4->replot();
            QVector<QPointF> rule_a;
            //补充中间参考线长度
            for(int i =0;i<101;i++){
                QPointF point;
                point.setX(i);
                int y = 0;
                point.setY(y);
                rule_a.append(point);
            }
            QwtPointSeriesData* rule_r = new QwtPointSeriesData(rule_a);
            rule_t_4->setData(rule_r);
            ui->data_plot_4->replot();
            QVector<QPointF> rule_error_a;
            for(int i =0;i<101;i++){
                QPointF point;
                point.setX(i);
                int y = 0;
                point.setY(y);
                rule_error_a.append(point);
            }
            QwtPointSeriesData* rule_error_r = new QwtPointSeriesData(rule_error_a);
            rule_error_t_4->setData(rule_error_r);
            ui->error_plot_4->replot();
            //隐藏实际曲线
            valid_curve_t_4->setVisible(false);
            error_curve_t_4->setVisible(false);
            ui->data_plot_4->replot();
        }
    }
}

void MainWindow::on_save_4_clicked()
{
    if (local_state!=SAVE_ALL&&local_state!=BACKUP_ALL)//==============================
    {
            name_dialog->show();//===============================================
    }
}

void MainWindow::on_save_4_clicked_after()
{


    //backup操作
        if(local_state==BACKUP_ALL)
        {
            bool ok=dataBackup();
             qDebug()<<ok;
        }


    //save操作
        float temp;
        QModelIndex indextemp;
        if(local_state==IDLE||local_state==SAVE_ALL)
        {
            QFile file("save3.txt");
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                return;
            for(int i=0;i<ui->tableView_4->model()->rowCount();i++)
            {
                for(int j=0;j<3;j++)
                {
                    indextemp = ui->tableView_4->model()->index(i,j);
                    //datatemp[i][j] = ui->tableView->model()->data(indextemp).toInt();
                    temp=ui->tableView_4->model()->data(indextemp).toFloat();
                    datatemp[i][j]=temp;
                   // out << datatemp[i][j] << " ";
                }
                if((datatemp[i][0]==-1)&&(datatemp[i][1]==-1)&&(datatemp[i][2]==-1))
                {
                    data_temp[0].save_valid_row_num=i;   //保存数据的有效行数
                    break;
                }
               // out<<"\n";
            }
            //============================================================================
            //memset(data_temp[0].last_time,0,100*sizeof(float));
            //memset(data_temp[0].volt,0,100*sizeof(float));
           // memset(data_temp[0].loop_times,0,100*sizeof(int));
            //==================================================

            //==============================================================
            QTextStream out(&file);
            out.setFieldWidth(8);
            out<<data_temp[0].save_valid_row_num;
            out<<"\n";
            out << ui->tabWidget->tabText(3);//=============================================
            out << "\n";//==================================================================
            out<<"    time      volt      loop\n";
            out<<"====================\n";
            for(int i=0;i<data_temp[0].save_valid_row_num;i++)
            {
                for(int j=0;j<3;j++)
                {
                  out << datatemp[i][j] << " ";
                }
                out<<"\n";
            }
            file.close();
        }
        //memset(datatemp,0,sizeof(float)*100*3);
        //memset(datatemp[][1],0,100*sizeof(float));
        //memset(datatemp[][2],0,100*sizeof(float));
        test_text_4();
        on_load_4_clicked();
}

void MainWindow::on_load_4_clicked()
{
    //先执行clear
        on_clear_4_clicked();
    //读取save数据
        if(local_state==IDLE||local_state==LOAD_ALL||local_state==SAVE_ALL)
        {
            int is=0;
            int js=0;
            std::ifstream file_t("save3.txt");
            std::string p;
            std::string q;
            QString name;
            file_t>>data_temp[0].load_valid_row_num;   //保存数据的有效行数;
            file_t>>q;//读取本行字放进q中，字属于string类
            name=QString::fromStdString(q);//转换为QString类型，方便下面写入
            ui->tabWidget->setTabText(3,name);//load时成功写入名字
            std::getline(file_t,p);
            std::getline(file_t,p);
            std::getline(file_t,p);
            while(!file_t.eof())        //按行读取
            {
                file_t>>datatemp[is][js++];
                file_t>>datatemp[is][js++];
                file_t>>datatemp[is++][js];
                js=0;
            }
            file_t.close();
            QModelIndex indextemp;
            for(int i=0;i<data_temp[0].load_valid_row_num+1;i++)        //读取文档中的数据显示在表格中
            {
                for(int j=0;j<3;j++)
                {
                    indextemp = ui->tableView_4->model()->index(i,j);
                    ui->tableView_4->model()->setData(indextemp,qRound(datatemp[i][j]*1000.0)/1000.0);
                    if(i == data_temp[0].load_valid_row_num)
                        ui->tableView_4->model()->setData(indextemp,-1.0);
                }

            }
        }else if(local_state==RESTORE_ALL||local_state==BACKUP_ALL)
        {
        //读取save_backup数据
            int is_back=0;
            int js_back=0;
            std::ifstream file_t_back("save_backup3.txt");
            std::string p;
            std::string q;
            QString name;
            file_t_back>>data_temp[0].load_valid_row_num;   //保存数据的有效行数;
            file_t_back>>q;//读取本行字放进q中，字属于string类
            name=QString::fromStdString(q);//转换为QString类型，方便下面写入
            ui->tabWidget->setTabText(3,name);//load时成功写入名字
            std::getline(file_t_back,p);
            std::getline(file_t_back,p);
            std::getline(file_t_back,p);
            while(!file_t_back.eof())        //按行读取
            {
                file_t_back>>datatemp[is_back][js_back++];
                file_t_back>>datatemp[is_back][js_back++];
                file_t_back>>datatemp[is_back++][js_back];
                js_back=0;
            }
            file_t_back.close();
            QModelIndex indextemp_back;
            for(int i=0;i<data_temp[0].load_valid_row_num+1;i++)        //读取文档中的数据显示在表格中
            {
                for(int j=0;j<3;j++)
                {
                    indextemp_back = ui->tableView_4->model()->index(i,j);
                    ui->tableView_4->model()->setData(indextemp_back,qRound(datatemp[i][j]*1000.0)/1000.0);
                    if(i == data_temp[0].load_valid_row_num)
                        ui->tableView_4->model()->setData(indextemp_back,-1.0);
                }

            }
        }
        if(local_state!=AUTO)       //不在AUTO模式下，允许画图
        {
            design_curve_t_4->setVisible(true);
            //绘制图像
            QModelIndex indextemp_1;
            float temp;
            int i=0;
            int j=0;
            for( i = 0;i<ui->tableView_4->model()->rowCount();i++)
            {
                for( j=0;j<3;j++)
                {
                    indextemp_1 = ui->tableView_4->model()->index(i,j);
                    temp=ui->tableView_4->model()->data(indextemp_1).toFloat();
                    datatemp[i][j]=temp;
                }
                if((datatemp[i][0]==-1)&&(datatemp[i][1]==-1)&&(datatemp[i][2]==-1))
                {
                    data_temp[0].down_valid_row_num=i;  //下传数据有效行数
                    break;
                }
            }
            for(i=0;i<data_temp[0].down_valid_row_num;i++)
            {
               data_temp[0].last_time[i]=datatemp[i][0];
               data_temp[0].volt[i]=datatemp[i][1];
               data_temp[0].loop_times[i]=datatemp[i][2];
            }
            graph_para[0].state=INITIAL;
            graph_para[0].gra_scan_row=0;  //扫描行数回到开头
            graph_para[0].gra_node_num=0;     //图中节点数
            graph_para[0].final_x=0;          //最后一点坐标
            graph_para[0].gra_loop_times=0;  //记录各循环节点剩余次数
            graph_para[0].gra_scan_row=0;            //图像扫描的行数
            memset(graph_para[0].gra_last_time,0,100*sizeof(float));  //准备显示电压持续时间
            memset(graph_para[0].gra_volt,0,100*sizeof(float));  //准备电压值
            graph_para[0].gra_loop_head_row=0;   //循环的开头行数
            //隐藏实际曲线
            //valid_curve_t_3->setPen(QColor(0,0,0),2,Qt::SolidLine);
            valid_curve_t_4->setVisible(false);
            ui->data_plot_4->replot();
            //清空实际图像
            graph_para[1].state=RESET;
            graph_down_replot_4();
            //load时显示最后一个坐标，变换坐标轴
            if(graph_para[0].final_x>10)
            {
                ui->data_plot_4->setAxisAutoScale(QwtPlot::xBottom, false);
                ui->data_plot_4->setAxisScale(QwtPlot::xBottom,0,graph_para[0].final_x,graph_para[0].final_x/10);
                ui->data_plot_4->replot();
            }
            else
            {
                ui->data_plot_4->setAxisAutoScale(QwtPlot::xBottom, false);
                ui->data_plot_4->setAxisScale(QwtPlot::xBottom,0,100,10);
                ui->data_plot_4->replot();
            }
            //补充中间参考线长度
            QVector<QPointF> rule_a;
            if(graph_para[0].final_x>100)
            {
                for(int i =0;i<(int)graph_para[0].final_x+100;i++){
                    QPointF point;
                    point.setX(i);
                    int y = 0;
                    point.setY(y);
                    rule_a.append(point);
                }
            }
            else
            {
                for(int i =0;i<100;i++){
                    QPointF point;
                    point.setX(i);
                    int y = 0;
                    point.setY(y);
                    rule_a.append(point);
                }
            }
            QwtPointSeriesData* rule_r = new QwtPointSeriesData(rule_a);
            rule_t_4->setData(rule_r);
            ui->data_plot_4->replot();

        }
}

void MainWindow::on_excute_4_clicked()
{
    //清空实际图像
    if(graph_para[0].state==DOWN_FINISH)     //download完成
    {
        graph_para[1].state=INITIAL;
        graph_para[1].gra_scan_row=0;  //扫描行数回到开头
        graph_para[1].gra_node_num=0;     //图中节点数
        graph_para[1].final_x=0;          //最后一点坐标
        graph_para[1].gra_loop_times=0;  //记录各循环节点剩余次数
        graph_para[1].gra_scan_row=0;            //图像扫描的行数
        memset(graph_para[1].gra_last_time,0,100*sizeof(float));  //准备显示电压持续时间
        memset(graph_para[1].gra_volt,0,100*sizeof(float));  //准备电压值
        graph_para[1].gra_loop_head_row=0;   //循环的开头行数

        if(!serial->isOpen())
        {
            new_win->serial_check();
            new_win->show();
        }
        else            //串口发送有效数据
        {
            serial->write("start");
            serial_end();
        }
    }
    //====================================================================
   //exce_1=0;
   //exce_2=0;
  // exce_3=0;
    pre_hour=1;
  if (exce_4!=0)
  exce_4=exce_4+1;
  // exce_5=0;
}

void MainWindow::graph_down_replot_4()
{
    //准备数据
        int j;
        int j_1;
        if(graph_para[0].state==1)//第一张图数据准备
        {
            for(j=0;j<100;j++)
            {
                if(graph_para[0].gra_scan_row==data_temp[0].down_valid_row_num)//数据读完
                {
                    if(graph_para[0].gra_loop_times!=0)             //若最后一行为循环头
                    {
                        if(graph_para[0].gra_loop_times!=9999)
                            graph_para[0].gra_loop_times--;
                        if(graph_para[0].gra_loop_times!=0) //避免非9999的循环体无限循环
                        {
                            graph_para[0]. gra_scan_row =   graph_para[0].gra_loop_head_row;   //扫描行数回到循环头的行数
                            graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                            graph_para[0].gra_last_time[j]=graph_para[0].final_x;
                            graph_para[0].gra_volt[j]=data_temp[0].volt[graph_para[0].gra_scan_row];
                            graph_para[0]. gra_scan_row++;
                        }
                        else
                        {
                            graph_para[0].gra_node_num=j;   //记录点数
                            graph_para[0].state=0;
                            break;
                        }
                    }
                    else                //若最后一行为正常数据
                    {
                        graph_para[0].gra_node_num=j;   //记录点数
                        graph_para[0].state=0;
                        break;
                    }
                }
                else
                {
                    if(data_temp[0].loop_times[graph_para[0].gra_scan_row]==0)  //非循环数据，直接存储,扫描行数+1
                    {
                       graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                       graph_para[0].gra_last_time[j]=graph_para[0].final_x;
                       graph_para[0].gra_volt[j]=data_temp[0].volt[graph_para[0].gra_scan_row];
                       graph_para[0]. gra_scan_row++;
                       if(graph_para[0].gra_loop_times==0)     //循环结束，记录循环次数
                       {
                           graph_para[0].gra_loop_times=data_temp[0].loop_times[graph_para[0].gra_scan_row];
                       }
                    }
                    else if(data_temp[0].loop_times[graph_para[0].gra_scan_row] != -1)//循环数据头，存储数据,记录循环次数
                    {
                        graph_para[0].gra_loop_head_row=graph_para[0].gra_scan_row;//记录循环体头的行数
                        if(graph_para[0].gra_loop_times==0)
                            graph_para[0].gra_loop_times=data_temp[0].loop_times[graph_para[0].gra_scan_row];
                        graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                        graph_para[0].gra_last_time[j]=graph_para[0].final_x;
                        graph_para[0].gra_volt[j]=data_temp[0].volt[graph_para[0].gra_scan_row];
                        graph_para[0]. gra_scan_row++;
                    }else if(data_temp[0].loop_times[graph_para[0].gra_scan_row] == -1)     //循环数据，记录数据，行数+1
                    {
                        graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                        graph_para[0].gra_last_time[j]=graph_para[0].final_x;
                        graph_para[0].gra_volt[j]=data_temp[0].volt[graph_para[0].gra_scan_row];
                        graph_para[0]. gra_scan_row++;
                        if(graph_para[0].gra_scan_row==data_temp[0].down_valid_row_num)  //如果到了文件末尾
                        {
                            if(graph_para[0].gra_loop_times!=0) //循环未结束，记录循环次数
                            {
                                if(graph_para[0].gra_loop_times!=9999)
                                    graph_para[0].gra_loop_times--;
                                if(graph_para[0].gra_loop_times==0)
                                {
                                    graph_para[0].gra_node_num=j+1;   //记录点数
                                   // graph_para[0].initial=1;
                                    break;
                                }
                                graph_para[0]. gra_scan_row =   graph_para[0].gra_loop_head_row;   //扫描行数回到循环头的行数
                            }
                            else //如果是最后一行，且循环结束，那么就跳出
                            {
                                graph_para[0].gra_node_num=j+1;   //记录点数
                               //graph_para[0].initial=1;
                                break;
                            }
                        }
                        else if(data_temp[0].loop_times[graph_para[0].gra_scan_row] !=-1)    //执行到循环最后一行
                        {
                            if(graph_para[0].gra_loop_times!=0) //循环未结束，记录循环次数
                            {
                                graph_para[0].gra_loop_times--;
                                if(graph_para[0].gra_loop_times!=0)
                                    graph_para[0]. gra_scan_row =   graph_para[0].gra_loop_head_row;   //扫描行数回到循环头的行数
                            }
                        }
                        else            //如果仍然是-1，判断是否是文件结尾
                        {
                            if(graph_para[0].gra_scan_row==data_temp[0].down_valid_row_num)
                            {
                                graph_para[0].gra_node_num=j+1;   //记录点数
                               // graph_para[0].state=0;
                                break;
                            }
                        }

                    }
                    else//错误数据，之后需补充检测数据输入的函数，错误报告异常
                    {
                        ;
                    }
                }
                graph_para[0].gra_node_num++;//正常循环个数+1
            }
            //自动开启刷新未写完的数据
    //        if(graph_para[0].gra_node_num==100)
    //            graph_para[0].state=2;
             graph_para[0].state=INITIAL_FINISH;//第一次刷新完成
           //  graph_para[1].state=1;//允许接受单片机数据
        }else       //后续点移入移出
        {
            if(graph_para[0].gra_scan_row==data_temp[0].down_valid_row_num)//数据读完
            {
                if(graph_para[0].gra_loop_times!=0)             //若最后一行为循环头
                {
                    if(graph_para[0].state==2)
                    {
                        for(j_1=0;j_1<99;j_1++){
                            graph_para[0].gra_last_time[j_1]=graph_para[0].gra_last_time[j_1+1];
                            graph_para[0].gra_volt[j_1]=graph_para[0].gra_volt[j_1+1];
                        }
                        if(graph_para[0].gra_loop_times!=9999)
                        {
                            graph_para[0].gra_loop_times--;
                        }
                        if(graph_para[0].gra_loop_times!=0)
                        {
                            graph_para[0]. gra_scan_row =   graph_para[0].gra_loop_head_row;   //扫描行数回到循环头的行数
                            graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                            graph_para[0].gra_last_time[99]=graph_para[0].final_x;
                            graph_para[0].gra_volt[99]=data_temp[0].volt[graph_para[0].gra_scan_row];
                            graph_para[0].gra_scan_row++;
                        }
                    }
                }
                else                //若最后一行为正常数据
                {
                    graph_para[0].state=0;
                }
            }
            else if(graph_para[0].state==2)          //继续执行
            {
                for(j_1=0;j_1<99;j_1++){
                    graph_para[0].gra_last_time[j_1]=graph_para[0].gra_last_time[j_1+1];
                    graph_para[0].gra_volt[j_1]=graph_para[0].gra_volt[j_1+1];
                }
                if(data_temp[0].loop_times[graph_para[0].gra_scan_row]==0)  //非循环数据，直接存储,扫描行数+1
                {
                   graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                   graph_para[0].gra_last_time[99]=graph_para[0].final_x;
                   graph_para[0].gra_volt[99]=data_temp[0].volt[graph_para[0].gra_scan_row];
                   graph_para[0]. gra_scan_row++;
                   if(graph_para[0].gra_loop_times==0)     //循环结束，记录循环次数
                   {
                       graph_para[0].gra_loop_times=data_temp[0].loop_times[graph_para[0].gra_scan_row];
                   }
                }
                else if(data_temp[0].loop_times[graph_para[0].gra_scan_row] != -1)//循环数据头，存储数据,记录循环次数
                {
                    graph_para[0].gra_loop_head_row=graph_para[0].gra_scan_row;//记录循环体头的行数
                    if(graph_para[0].gra_loop_times==0)
                        graph_para[0].gra_loop_times=data_temp[0].loop_times[graph_para[0].gra_scan_row];
                    graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                    graph_para[0].gra_last_time[99]=graph_para[0].final_x;
                    graph_para[0].gra_volt[99]=data_temp[0].volt[graph_para[0].gra_scan_row];
                    graph_para[0]. gra_scan_row++;
                }else if(data_temp[0].loop_times[graph_para[0].gra_scan_row] == -1)     //循环数据，记录数据，行数+1
                {
                    graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                    graph_para[0].gra_last_time[99]=graph_para[0].final_x;
                    graph_para[0].gra_volt[99]=data_temp[0].volt[graph_para[0].gra_scan_row];
                    graph_para[0]. gra_scan_row++;
                    if(graph_para[0].gra_scan_row==data_temp[0].down_valid_row_num)  //如果文件结尾，跳出
                    {
                        if(graph_para[0].gra_loop_times!=0) //循环未结束，记录循环次数
                        {
                            graph_para[0].gra_loop_times--;
                            if(graph_para[0].gra_loop_times!=0)
                            {
                                graph_para[0]. gra_scan_row =   graph_para[0].gra_loop_head_row;   //扫描行数回到循环头的行数
                            }
                        }
                    }
                    else if(data_temp[0].loop_times[graph_para[0].gra_scan_row] !=-1)    //执行到循环最后一行
                    {
                        if(graph_para[0].gra_loop_times!=0) //循环未结束，记录循环次数
                        {
                            graph_para[0].gra_loop_times--;
                            if(graph_para[0].gra_loop_times!=0)
                                graph_para[0]. gra_scan_row =   graph_para[0].gra_loop_head_row;   //扫描行数回到循环头的行数
                        }
                    }
                }
            }
            else if(graph_para[0].state==0)//无需刷新
            {
                ;
            }
            else if(graph_para[0].state==3)//初始化结束
            {
                  graph_para[0].state=0;  //暂停刷新
            }
        }
    //清零实际图像
        if(graph_para[1].state==RESET)
        {
        //清零实际图像
                QVector<QPointF> valid_curve;
                QVector<QPointF> error_curve;       //误差曲线
                QPointF point;
                QPointF point_error;
                for(int i =0;i<100;i++){
                       // graph_para[0].final_x += (float)graph_para[0].gra_last_time[i]/1000.0;
                        point.setX(i);
                        point.setY(0);
                        valid_curve.append(point);
                }
                for(int i =0;i<100;i++){//误差曲线
                       // graph_para[0].final_x += (float)graph_para[0].gra_last_time[i]/1000.0;
                        point_error.setX(i);
                        point_error.setY(0);
                        error_curve.append(point_error);
                }
                QwtPointSeriesData* valid_curve_r = new QwtPointSeriesData(valid_curve);
                QwtPointSeriesData* error_curve_r = new QwtPointSeriesData(error_curve);//误差曲线
                valid_curve_t_4->setData(valid_curve_r);
                error_curve_t_4->setData(error_curve_r);//误差曲线
                if(graph_para[1].gra_node_num==100)
                {
                    ui->data_plot_4->setAxisAutoScale(QwtPlot::xBottom, false);
                    ui->data_plot_4->setAxisScale(QwtPlot::xBottom,0,10,1);
                    ui->data_plot_4->replot();
                    ui->error_plot_4->setAxisAutoScale(QwtPlot::xBottom, false);//误差曲线
                    ui->error_plot_4->setAxisScale(QwtPlot::xBottom,0,10,1);
                    ui->error_plot_4->replot();
                }
                ui->data_plot_4->replot();
                ui->error_plot_4->replot();
                graph_para[1].state=STOP;
        }
    //显示数据
        if(graph_para[0].state)
        {
            //rule_extend();
            QVector<QPointF> design_curve;
            QPointF point;
            for(int i =0;i<graph_para[0].gra_node_num;i++){
                    point.setX(float(graph_para[0].gra_last_time[i]));
                    point.setY((float)graph_para[0].gra_volt[i]);
                    design_curve.append(point);
            }
            QwtPointSeriesData* design_curve_r = new QwtPointSeriesData(design_curve);
            design_curve_t_4->setData(design_curve_r);
            ui->data_plot_4->replot();
            //qDebug()<<design_curve.data()->x()<<""<<design_curve.data()->y();
        }
}

void MainWindow::on_clear_4_clicked()
{
    auto_state = OFF;
    QModelIndex indextemp;
    for(int i=0;i<ui->tableView_4->model()->rowCount();i++)        //清零表格中的数据
    {
        for(int j=0;j<3;j++)
        {
            indextemp = ui->tableView_4->model()->index(i,j);
            ui->tableView_4->model()->setData(indextemp,0.0);
        }
    }
    memset(graph_para[0].gra_volt, 0, 100*sizeof(float) );
    memset(graph_para[1].gra_volt, 0, 100*sizeof(float) );
    graph_para[0].state=INITIAL;
    graph_para[0].gra_scan_row=0;
    data_temp[0].down_valid_row_num=0;
    //修复bug，清除上一幅图像的数据和本地缓存
    memset(data_temp[0].loop_times,0,100*sizeof(int));

    //曲线名清除
    ui->tabWidget->setTabText(3,"tab_4");

    //LCDnumber clear
    ui->lcdNumber_10->display(0);
    ui->lcdNumber_12->display(0);
    ui->lcdNumber_11->display(0);
    ui->lcdNumber_24->display(0);
    ui->lcdNumber_25->display(0);
    ui->lcdNumber_29->display(0);//=================

    graph_para[1].state=RESET;
    graph_para[1].final_x=0;
    graph_down_replot_4();

    //数据图坐标更新回复
    ui->data_plot_4->setAxisScale(QwtPlot::yLeft,-5,5,1);
    ui->data_plot_4->setAxisScale(QwtPlot::xBottom,0,10,1);
    QVector<QPointF> rule;                   //数据参考线
    for(int i =-1;i<101;i++){
        QPointF point;
        point.setX(i);
        int y = 0;
        point.setY(y);
        rule.append(point);
    }
    QwtPointSeriesData* rule_c = new QwtPointSeriesData(rule);
    rule_t_4 = new QwtPlotCurve("Curve 0");
    rule_t_4->setData(rule_c);
    rule_t_4->attach(ui->data_plot_4);

    QVector<QPointF> rule_error;             //误差参考线
    for(int i =-1;i<101;i++){
        QPointF point_error;
        point_error.setX(i);
        int y = 0;
        point_error.setY(y);
        rule_error.append(point_error);
    }
    QwtPointSeriesData* rule_error_c = new QwtPointSeriesData(rule_error);
    rule_error_t_4 = new QwtPlotCurve("Curve 0");
    rule_error_t_4->setData(rule_error_c);
    rule_error_t_4->attach(ui->error_plot_4);

    valid_curve_t_4->setVisible(false);            //隐藏实际曲线
    ui->error_plot_4->replot();
    error_curve_t_4->setVisible(false);            //隐藏误差曲线
    ui->error_plot_4->replot();
    design_curve_t_4->setVisible(false);            //隐藏误差曲线
    ui->error_plot_4->replot();
    QVector<QPointF> rule_a;
    QVector<QPointF> rule_error_a;//误差曲线
    for(int i =0;i<100;i++){
        QPointF point;
        QPointF point_error;            //误差曲线
        point.setX(i);
        point_error.setX(i);            //误差曲线
        point.setY(0);
        point_error.setY(0);            //误差曲线
        rule_a.append(point);
        rule_error_a.append(point_error);//误差曲线
    }
    QwtPointSeriesData* rule_r = new QwtPointSeriesData(rule_a);
    rule_t_4->setData(rule_r);
    QwtPointSeriesData* rule_error_r = new QwtPointSeriesData(rule_error_a);//误差曲线
    rule_error_t_4->setData(rule_error_r);//误差曲线
    ui->data_plot_4->replot();
    ui->error_plot_4->replot();
    stop_excute();
}

void MainWindow::test_text_4()
{
    qDebug()<<"NO.4 chart:";
    QModelIndex indextemp_4;
    float temp;
    int i=0;
    int j=0;
    for( i = 0;i<ui->tableView_4->model()->rowCount();i++)
    {
        for( j=0;j<3;j++)
        {
            indextemp_4 = ui->tableView_4->model()->index(i,j);
            temp=ui->tableView_4->model()->data(indextemp_4).toFloat();
            datatemp[i][j]=temp;
        }
        if((datatemp[i][0]==-1)&&(datatemp[i][1]==-1)&&(datatemp[i][2]==-1))
        {
            data_temp[0].down_valid_row_num=i;  //下传数据有效行数
            break;
        }
    }
    //

    int k=0;
    int flag=0;
    //第一个循环用来寻找第一个循环头
    for(int i=0;i<data_temp[0].down_valid_row_num;i++)
    {
        if (datatemp[i][2]<=-1)
        {
                qDebug()<<"there is a mistake:"<<i+1;
                flag=1;
                break;
        }
        else
        {
            if (datatemp[i][2]>0)
            {
                k=i;
                break;
            }
        }
    }

    //以下才是真正的判断循环体的正确性。当然，零的存在打断了部分循环。而且为了简化最后一个else导致复杂化了仲间的两个if判断
    for(i=k+1;i<data_temp[0].down_valid_row_num;i++)
    {
        if(flag) break;
        else
        {
            if (i==k+1)
            {
                if (datatemp[k][2]>0)
                {
                    if (datatemp[i][2]>=0||datatemp[i][2]<-1)
                    {
                        qDebug()<<"there is a mistake:"<<i+1;
                        break;
                    }
                    else if (i==data_temp[0].down_valid_row_num-1)
                    {
                        qDebug()<<"all is correct";
                    }
                }
                else
                {
                    for (i=k+1;i<data_temp[0].down_valid_row_num;i++)
                    {
                        if (datatemp[i][2]<=-1)
                        {
                                qDebug()<<"there is a mistake:"<<i+1;
                                flag=1;
                                break;
                        }
                        else if (datatemp[i][2]>0)
                        {
                            if (i!=data_temp[0].down_valid_row_num-1)
                            {
                                k=i;
                                break;
                            }
                            else
                            {
                                qDebug()<<"the last row is wrong and so pity";
                                flag=1;
                                break;
                            }
                        }
                        else if(datatemp[i][2]==0&&i==data_temp[0].down_valid_row_num-1)
                        qDebug()<<"all is correct";
                    }
                }

            }
            else if (i==data_temp[0].down_valid_row_num-1)
            {
                if (datatemp[i-1][2]==-1)
                {
                    if (datatemp[i][2]==-1||datatemp[i][2]==0) qDebug()<<"all is correct";
                    else
                    {
                        qDebug()<<"the last row is wrong and so pity";
                        break;
                    }
                }
                else if (datatemp[i-1][2]==0)
                {
                    if (datatemp[i][2]==0) qDebug()<<"all is correct";
                    else
                    {
                        qDebug()<<"the last row is wrong and so pity";
                        break;
                    }
                }
                else
                {
                    if (datatemp[i][2]==-1) qDebug()<<"all is correct";
                    else
                    {
                        qDebug()<<"the last row is wrong and so pity";
                        break;
                    }
                }
            }
            else
            {
                if(datatemp[i][2]<-1)
                {
                    qDebug()<<"there is a mistake:"<<i+1;
                    break;
                }
                else
                {
                    if(datatemp[i][2]!=-1)
                    {
                        k=i;
                        qDebug()<<"current circle is correct"<<i;
                    }
                }
            }
        }
    }
}

void MainWindow::on_download_5_clicked()
{
    if(local_state!=AUTO)
    {
        QModelIndex indextemp;
        float temp;
        int i=0;
        int j=0;
        for( i = 0;i<ui->tableView_5->model()->rowCount();i++)
        {
            for( j=0;j<3;j++)
            {
                indextemp = ui->tableView_5->model()->index(i,j);
                temp=ui->tableView_5->model()->data(indextemp).toFloat();
                datatemp[i][j]=temp;
            }
            if((datatemp[i][0]==-1)&&(datatemp[i][1]==-1)&&(datatemp[i][2]==-1))
            {
                data_temp[0].down_valid_row_num=i;  //下传数据有效行数
               //valid_row_number=i;
                break;
            }
        }
        char ctemp[0];
        if(!serial->isOpen())
        {
            new_win->serial_check();
            new_win->show();
        }
        else            //串口发送有效数据
        {
            sprintf(&ctemp[0],"%d;",data_temp[0].down_valid_row_num);
            serial->write(&ctemp[0]);
            for(i=0;i<data_temp[0].down_valid_row_num;i++)
            {
               data_temp[0].last_time[i]=datatemp[i][0];
               sprintf(&ctemp[0],"%d;",(int)(datatemp[i][0]*1000));
               serial->write(&ctemp[0]);
               data_temp[0].volt[i]=datatemp[i][1];
               sprintf(&ctemp[0],"%d;",(int)(datatemp[i][1]*1000));
               serial->write(&ctemp[0]);
               data_temp[0].loop_times[i]=datatemp[i][2];
               sprintf(&ctemp[0],"%d;",(int)(datatemp[i][2]));
               serial->write(&ctemp[0]);
            }
            serial_end();//发送给单片机的终止符

            graph_para[0].state=INITIAL;
            graph_para[0].gra_scan_row=0;  //扫描行数回到开头
            graph_para[0].gra_node_num=0;     //图中节点数
            graph_para[0].final_x=0;          //最后一点坐标
            graph_para[0].gra_loop_times=0;  //记录各循环节点剩余次数
            graph_para[0].gra_scan_row=0;            //图像扫描的行数
            memset(graph_para[0].gra_last_time,0,100*sizeof(float));  //准备显示电压持续时间
            memset(graph_para[0].gra_volt,0,100*sizeof(float));  //准备电压值
            graph_para[0].gra_loop_head_row=0;   //循环的开头行数
            design_curve_t_5->setVisible(true);//===============================================
            //清空实际图像
            graph_para[1].state=RESET;
            graph_down_replot_5();
            //设计图形初始化完成
            graph_para[0].state=DOWN_FINISH;

            //复原坐标轴
            ui->data_plot_5->setAxisAutoScale(QwtPlot::xBottom, false);
            ui->data_plot_5->setAxisScale(QwtPlot::xBottom,0,10,1);
            ui->data_plot_5->replot();
            ui->error_plot_5->setAxisAutoScale(QwtPlot::xBottom, false);
            ui->error_plot_5->setAxisScale(QwtPlot::xBottom,0,10,1);
            ui->error_plot_5->replot();
            QVector<QPointF> rule_a;
            //补充中间参考线长度
            for(int i =0;i<101;i++){
                QPointF point;
                point.setX(i);
                int y = 0;
                point.setY(y);
                rule_a.append(point);
            }
            QwtPointSeriesData* rule_r = new QwtPointSeriesData(rule_a);
            rule_t_5->setData(rule_r);
            ui->data_plot_5->replot();
            QVector<QPointF> rule_error_a;
            for(int i =0;i<101;i++){
                QPointF point;
                point.setX(i);
                int y = 0;
                point.setY(y);
                rule_error_a.append(point);
            }
            QwtPointSeriesData* rule_error_r = new QwtPointSeriesData(rule_error_a);
            rule_error_t_5->setData(rule_error_r);
            ui->error_plot_5->replot();
            //隐藏实际曲线
            valid_curve_t_5->setVisible(false);
            error_curve_t_5->setVisible(false);
            ui->data_plot_5->replot();
        }
    }
}

void MainWindow::on_save_5_clicked()
{
    if (local_state!=SAVE_ALL&&local_state!=BACKUP_ALL)//=============================
    {
            name_dialog->show();//===========================
    }
}

void MainWindow::on_save_5_clicked_after()
{


    //backup操作
        if(local_state==BACKUP_ALL)
        {
            bool ok=dataBackup();
             qDebug()<<ok;
        }


    //save操作
        float temp;
        QModelIndex indextemp;
        if(local_state==IDLE||local_state==SAVE_ALL)
        {
            QFile file("save4.txt");
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                return;
            for(int i=0;i<ui->tableView_5->model()->rowCount();i++)
            {
                for(int j=0;j<3;j++)
                {
                    indextemp = ui->tableView_5->model()->index(i,j);
                    //datatemp[i][j] = ui->tableView->model()->data(indextemp).toInt();
                    temp=ui->tableView_5->model()->data(indextemp).toFloat();
                    datatemp[i][j]=temp;
                   // out << datatemp[i][j] << " ";
                }
                if((datatemp[i][0]==-1)&&(datatemp[i][1]==-1)&&(datatemp[i][2]==-1))
                {
                    data_temp[0].save_valid_row_num=i;   //保存数据的有效行数
                    break;
                }
               // out<<"\n";
            }
            //==========================================================
           // memset(data_temp[0].last_time,0,100*sizeof(float));
            //memset(data_temp[0].volt,0,100*sizeof(float));
           // memset(data_temp[0].loop_times,0,100*sizeof(int));
            //==================================================

            //===============================================================================
            QTextStream out(&file);
            out.setFieldWidth(8);
            out<<data_temp[0].save_valid_row_num;
            out<<"\n";
            out << ui->tabWidget->tabText(4);//=============================================
            out << "\n";//==================================================================
            out<<"    time      volt      loop\n";
            out<<"====================\n";
            for(int i=0;i<data_temp[0].save_valid_row_num;i++)
            {
                for(int j=0;j<3;j++)
                {
                  out << datatemp[i][j] << " ";
                }
                out<<"\n";
            }
            file.close();
        }
       // memset(datatemp,0,sizeof(float)*100*3);
        //memset(datatemp[][1],0,100*sizeof(float));
        //memset(datatemp[][2],0,100*sizeof(float));
        test_text_5();
        on_load_5_clicked();
}

void MainWindow::on_load_5_clicked()
{
    //先执行clear
        on_clear_5_clicked();
    //读取save数据
        if(local_state==IDLE||local_state==LOAD_ALL||local_state==SAVE_ALL)
        {
            int is=0;
            int js=0;
            std::ifstream file_t("save4.txt");
            std::string p;
            std::string q;
            QString name;
            file_t>>data_temp[0].load_valid_row_num;   //保存数据的有效行数;
            file_t>>q;//读取本行字放进q中，字属于string类
            name=QString::fromStdString(q);//转换为QString类型，方便下面写入
            ui->tabWidget->setTabText(4,name);//load时成功写入名字
            std::getline(file_t,p);
            std::getline(file_t,p);
            std::getline(file_t,p);
            while(!file_t.eof())        //按行读取
            {
                file_t>>datatemp[is][js++];
                file_t>>datatemp[is][js++];
                file_t>>datatemp[is++][js];
                js=0;
            }
            file_t.close();
            QModelIndex indextemp;
            for(int i=0;i<data_temp[0].load_valid_row_num+1;i++)        //读取文档中的数据显示在表格中
            {
                for(int j=0;j<3;j++)
                {
                    indextemp = ui->tableView_5->model()->index(i,j);
                    ui->tableView_5->model()->setData(indextemp,qRound(datatemp[i][j]*1000.0)/1000.0);
                    if(i == data_temp[0].load_valid_row_num)
                        ui->tableView_5->model()->setData(indextemp,-1.0);
                }

            }
        }else if(local_state==RESTORE_ALL||local_state==BACKUP_ALL)
        {
        //读取save_backup数据
            int is_back=0;
            int js_back=0;
            std::ifstream file_t_back("save_backup4.txt");
            std::string p;
            std::string q;
            QString name;
            file_t_back>>data_temp[0].load_valid_row_num;   //保存数据的有效行数;
            file_t_back>>q;//读取本行字放进q中，字属于string类
            name=QString::fromStdString(q);//转换为QString类型，方便下面写入
            ui->tabWidget->setTabText(4,name);//load时成功写入名字
            std::getline(file_t_back,p);
            std::getline(file_t_back,p);
            std::getline(file_t_back,p);
            while(!file_t_back.eof())        //按行读取
            {
                file_t_back>>datatemp[is_back][js_back++];
                file_t_back>>datatemp[is_back][js_back++];
                file_t_back>>datatemp[is_back++][js_back];
                js_back=0;
            }
            file_t_back.close();
            QModelIndex indextemp_back;
            for(int i=0;i<data_temp[0].load_valid_row_num+1;i++)        //读取文档中的数据显示在表格中
            {
                for(int j=0;j<3;j++)
                {
                    indextemp_back = ui->tableView_5->model()->index(i,j);
                    ui->tableView_5->model()->setData(indextemp_back,qRound(datatemp[i][j]*1000.0)/1000.0);
                    if(i == data_temp[0].load_valid_row_num)
                        ui->tableView_5->model()->setData(indextemp_back,-1.0);
                }

            }
        }
        if(local_state!=AUTO)       //不在AUTO模式下，允许画图
        {
            design_curve_t_5->setVisible(true);
            //绘制图像
            QModelIndex indextemp_1;
            float temp;
            int i=0;
            int j=0;
            for( i = 0;i<ui->tableView_5->model()->rowCount();i++)
            {
                for( j=0;j<3;j++)
                {
                    indextemp_1 = ui->tableView_5->model()->index(i,j);
                    temp=ui->tableView_5->model()->data(indextemp_1).toFloat();
                    datatemp[i][j]=temp;
                }
                if((datatemp[i][0]==-1)&&(datatemp[i][1]==-1)&&(datatemp[i][2]==-1))
                {
                    data_temp[0].down_valid_row_num=i;  //下传数据有效行数
                    break;
                }
            }
            for(i=0;i<data_temp[0].down_valid_row_num;i++)
            {
               data_temp[0].last_time[i]=datatemp[i][0];
               data_temp[0].volt[i]=datatemp[i][1];
               data_temp[0].loop_times[i]=datatemp[i][2];
            }
            graph_para[0].state=INITIAL;
            graph_para[0].gra_scan_row=0;  //扫描行数回到开头
            graph_para[0].gra_node_num=0;     //图中节点数
            graph_para[0].final_x=0;          //最后一点坐标
            graph_para[0].gra_loop_times=0;  //记录各循环节点剩余次数
            graph_para[0].gra_scan_row=0;            //图像扫描的行数
            memset(graph_para[0].gra_last_time,0,100*sizeof(float));  //准备显示电压持续时间
            memset(graph_para[0].gra_volt,0,100*sizeof(float));  //准备电压值
            graph_para[0].gra_loop_head_row=0;   //循环的开头行数
            //隐藏实际曲线
            //valid_curve_t_3->setPen(QColor(0,0,0),2,Qt::SolidLine);
            valid_curve_t_5->setVisible(false);
            ui->data_plot_5->replot();
            //清空实际图像
            graph_para[1].state=RESET;
            graph_down_replot_5();
            //load时显示最后一个坐标，变换坐标轴
            if(graph_para[0].final_x>10)
            {
                ui->data_plot_5->setAxisAutoScale(QwtPlot::xBottom, false);
                ui->data_plot_5->setAxisScale(QwtPlot::xBottom,0,graph_para[0].final_x,graph_para[0].final_x/10);
                ui->data_plot_5->replot();
            }
            else
            {
                ui->data_plot_5->setAxisAutoScale(QwtPlot::xBottom, false);
                ui->data_plot_5->setAxisScale(QwtPlot::xBottom,0,100,10);
                ui->data_plot_5->replot();
            }
            //补充中间参考线长度
            QVector<QPointF> rule_a;
            if(graph_para[0].final_x>100)
            {
                for(int i =0;i<(int)graph_para[0].final_x+100;i++){
                    QPointF point;
                    point.setX(i);
                    int y = 0;
                    point.setY(y);
                    rule_a.append(point);
                }
            }
            else
            {
                for(int i =0;i<100;i++){
                    QPointF point;
                    point.setX(i);
                    int y = 0;
                    point.setY(y);
                    rule_a.append(point);
                }
            }
            QwtPointSeriesData* rule_r = new QwtPointSeriesData(rule_a);
            rule_t_5->setData(rule_r);
            ui->data_plot_5->replot();

        }
}

void MainWindow::on_excute_5_clicked()
{
    //清空实际图像
    if(graph_para[0].state==DOWN_FINISH)     //download完成
    {
        graph_para[1].state=INITIAL;
        graph_para[1].gra_scan_row=0;  //扫描行数回到开头
        graph_para[1].gra_node_num=0;     //图中节点数
        graph_para[1].final_x=0;          //最后一点坐标
        graph_para[1].gra_loop_times=0;  //记录各循环节点剩余次数
        graph_para[1].gra_scan_row=0;            //图像扫描的行数
        memset(graph_para[1].gra_last_time,0,100*sizeof(float));  //准备显示电压持续时间
        memset(graph_para[1].gra_volt,0,100*sizeof(float));  //准备电压值
        graph_para[1].gra_loop_head_row=0;   //循环的开头行数

        if(!serial->isOpen())
        {
            new_win->serial_check();
            new_win->show();
        }
        else            //串口发送有效数据
        {
            serial->write("start");
            serial_end();
        }
    }
    //====================================================================
   //exce_1=0;
  // exce_2=0;
  // exce_3=0;
   //exce_4=0;
    pre_hour=1;
    //=====================
   if (exce_5!=0)
   exce_5=exce_5+1;
}

void MainWindow::graph_down_replot_5()
{
    //准备数据
        int j;
        int j_1;
        if(graph_para[0].state==1)//第一张图数据准备
        {
            for(j=0;j<100;j++)
            {
                if(graph_para[0].gra_scan_row==data_temp[0].down_valid_row_num)//数据读完
                {
                    if(graph_para[0].gra_loop_times>0)             //若最后一行为循环头
                    {
                        if(graph_para[0].gra_loop_times!=9999)
                            graph_para[0].gra_loop_times--;
                        if(graph_para[0].gra_loop_times!=0) //避免非9999的循环体无限循环
                        {
                            graph_para[0]. gra_scan_row =   graph_para[0].gra_loop_head_row;   //扫描行数回到循环头的行数
                            graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                            graph_para[0].gra_last_time[j]=graph_para[0].final_x;
                            graph_para[0].gra_volt[j]=data_temp[0].volt[graph_para[0].gra_scan_row];
                            graph_para[0]. gra_scan_row++;
                        }
                        else
                        {
                            graph_para[0].gra_node_num=j;   //记录点数
                            graph_para[0].state=0;
                            break;
                        }
                    }
                    else                //若最后一行为正常数据
                    {
                        graph_para[0].gra_node_num=j;   //记录点数
                        graph_para[0].state=0;
                        break;
                    }
                }
                else
                {
                    if(data_temp[0].loop_times[graph_para[0].gra_scan_row]==0)  //非循环数据，直接存储,扫描行数+1
                    {
                       graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                       graph_para[0].gra_last_time[j]=graph_para[0].final_x;
                       graph_para[0].gra_volt[j]=data_temp[0].volt[graph_para[0].gra_scan_row];
                       graph_para[0]. gra_scan_row++;
                       if(graph_para[0].gra_loop_times==0)     //循环结束，记录循环次数
                       {
                           graph_para[0].gra_loop_times=data_temp[0].loop_times[graph_para[0].gra_scan_row];
                       }
                    }
                    else if(data_temp[0].loop_times[graph_para[0].gra_scan_row] != -1)//循环数据头，存储数据,记录循环次数
                    {
                        graph_para[0].gra_loop_head_row=graph_para[0].gra_scan_row;//记录循环体头的行数
                        if(graph_para[0].gra_loop_times==0)
                            graph_para[0].gra_loop_times=data_temp[0].loop_times[graph_para[0].gra_scan_row];
                        graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                        graph_para[0].gra_last_time[j]=graph_para[0].final_x;
                        graph_para[0].gra_volt[j]=data_temp[0].volt[graph_para[0].gra_scan_row];
                        graph_para[0]. gra_scan_row++;
                    }else if(data_temp[0].loop_times[graph_para[0].gra_scan_row] == -1)     //循环数据，记录数据，行数+1
                    {
                        graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                        graph_para[0].gra_last_time[j]=graph_para[0].final_x;
                        graph_para[0].gra_volt[j]=data_temp[0].volt[graph_para[0].gra_scan_row];
                        graph_para[0]. gra_scan_row++;
                        if(graph_para[0].gra_scan_row==data_temp[0].down_valid_row_num)  //如果到了文件末尾
                        {
                            if(graph_para[0].gra_loop_times!=0) //循环未结束，记录循环次数
                            {
                                if(graph_para[0].gra_loop_times!=9999)
                                    graph_para[0].gra_loop_times--;
                                if(graph_para[0].gra_loop_times==0)
                                {
                                    graph_para[0].gra_node_num=j+1;   //记录点数
                                   // graph_para[0].initial=1;
                                    break;
                                }
                                graph_para[0]. gra_scan_row =   graph_para[0].gra_loop_head_row;   //扫描行数回到循环头的行数
                            }
                            else //如果是最后一行，且循环结束，那么就跳出
                            {
                                graph_para[0].gra_node_num=j+1;   //记录点数
                               //graph_para[0].initial=1;
                                break;
                            }
                        }
                        else if(data_temp[0].loop_times[graph_para[0].gra_scan_row] !=-1)    //执行到循环最后一行
                        {
                            if(graph_para[0].gra_loop_times!=0) //循环未结束，记录循环次数
                            {
                                graph_para[0].gra_loop_times--;
                                if(graph_para[0].gra_loop_times!=0)
                                    graph_para[0]. gra_scan_row =   graph_para[0].gra_loop_head_row;   //扫描行数回到循环头的行数
                            }
                        }
                        else            //如果仍然是-1，判断是否是文件结尾
                        {
                            if(graph_para[0].gra_scan_row==data_temp[0].down_valid_row_num)
                            {
                                graph_para[0].gra_node_num=j+1;   //记录点数
                               // graph_para[0].state=0;
                                break;
                            }
                        }

                    }
                    else//错误数据，之后需补充检测数据输入的函数，错误报告异常
                    {
                        ;
                    }
                }
                graph_para[0].gra_node_num++;//正常循环个数+1
            }
            //自动开启刷新未写完的数据
    //        if(graph_para[0].gra_node_num==100)
    //            graph_para[0].state=2;
             graph_para[0].state=INITIAL_FINISH;//第一次刷新完成
           //  graph_para[1].state=1;//允许接受单片机数据
        }else       //后续点移入移出
        {
            if(graph_para[0].gra_scan_row==data_temp[0].down_valid_row_num)//数据读完
            {
                if(graph_para[0].gra_loop_times!=0)             //若最后一行为循环头
                {
                    if(graph_para[0].state==2)
                    {
                        for(j_1=0;j_1<99;j_1++){
                            graph_para[0].gra_last_time[j_1]=graph_para[0].gra_last_time[j_1+1];
                            graph_para[0].gra_volt[j_1]=graph_para[0].gra_volt[j_1+1];
                        }
                        if(graph_para[0].gra_loop_times!=9999)
                        {
                            graph_para[0].gra_loop_times--;
                        }
                        if(graph_para[0].gra_loop_times!=0)
                        {
                            graph_para[0]. gra_scan_row =   graph_para[0].gra_loop_head_row;   //扫描行数回到循环头的行数
                            graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                            graph_para[0].gra_last_time[99]=graph_para[0].final_x;
                            graph_para[0].gra_volt[99]=data_temp[0].volt[graph_para[0].gra_scan_row];
                            graph_para[0].gra_scan_row++;
                        }
                    }
                }
                else                //若最后一行为正常数据
                {
                    graph_para[0].state=0;
                }
            }
            else if(graph_para[0].state==2)          //继续执行
            {
                for(j_1=0;j_1<99;j_1++){
                    graph_para[0].gra_last_time[j_1]=graph_para[0].gra_last_time[j_1+1];
                    graph_para[0].gra_volt[j_1]=graph_para[0].gra_volt[j_1+1];
                }
                if(data_temp[0].loop_times[graph_para[0].gra_scan_row]==0)  //非循环数据，直接存储,扫描行数+1
                {
                   graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                   graph_para[0].gra_last_time[99]=graph_para[0].final_x;
                   graph_para[0].gra_volt[99]=data_temp[0].volt[graph_para[0].gra_scan_row];
                   graph_para[0]. gra_scan_row++;
                   if(graph_para[0].gra_loop_times==0)     //循环结束，记录循环次数
                   {
                       graph_para[0].gra_loop_times=data_temp[0].loop_times[graph_para[0].gra_scan_row];
                   }
                }
                else if(data_temp[0].loop_times[graph_para[0].gra_scan_row] != -1)//循环数据头，存储数据,记录循环次数
                {
                    graph_para[0].gra_loop_head_row=graph_para[0].gra_scan_row;//记录循环体头的行数
                    if(graph_para[0].gra_loop_times==0)
                        graph_para[0].gra_loop_times=data_temp[0].loop_times[graph_para[0].gra_scan_row];
                    graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                    graph_para[0].gra_last_time[99]=graph_para[0].final_x;
                    graph_para[0].gra_volt[99]=data_temp[0].volt[graph_para[0].gra_scan_row];
                    graph_para[0]. gra_scan_row++;
                }else if(data_temp[0].loop_times[graph_para[0].gra_scan_row] == -1)     //循环数据，记录数据，行数+1
                {
                    graph_para[0].final_x += (float)data_temp[0].last_time[graph_para[0].gra_scan_row];
                    graph_para[0].gra_last_time[99]=graph_para[0].final_x;
                    graph_para[0].gra_volt[99]=data_temp[0].volt[graph_para[0].gra_scan_row];
                    graph_para[0]. gra_scan_row++;
                    if(graph_para[0].gra_scan_row==data_temp[0].down_valid_row_num)  //如果文件结尾，跳出
                    {
                        if(graph_para[0].gra_loop_times!=0) //循环未结束，记录循环次数
                        {
                            graph_para[0].gra_loop_times--;
                            if(graph_para[0].gra_loop_times!=0)
                            {
                                graph_para[0]. gra_scan_row =   graph_para[0].gra_loop_head_row;   //扫描行数回到循环头的行数
                            }
                        }
                    }
                    else if(data_temp[0].loop_times[graph_para[0].gra_scan_row] !=-1)    //执行到循环最后一行
                    {
                        if(graph_para[0].gra_loop_times!=0) //循环未结束，记录循环次数
                        {
                            graph_para[0].gra_loop_times--;
                            if(graph_para[0].gra_loop_times!=0)
                                graph_para[0]. gra_scan_row =   graph_para[0].gra_loop_head_row;   //扫描行数回到循环头的行数
                        }
                    }
                }
            }
            else if(graph_para[0].state==0)//无需刷新
            {
                ;
            }
            else if(graph_para[0].state==3)//初始化结束
            {
                  graph_para[0].state=0;  //暂停刷新
            }
        }
    //清零实际图像
        if(graph_para[1].state==RESET)
        {
        //清零实际图像
                QVector<QPointF> valid_curve;
                QVector<QPointF> error_curve;       //误差曲线
                QPointF point;
                QPointF point_error;
                for(int i =0;i<100;i++){
                       // graph_para[0].final_x += (float)graph_para[0].gra_last_time[i]/1000.0;
                        point.setX(i);
                        point.setY(0);
                        valid_curve.append(point);
                }
                for(int i =0;i<100;i++){//误差曲线
                       // graph_para[0].final_x += (float)graph_para[0].gra_last_time[i]/1000.0;
                        point_error.setX(i);
                        point_error.setY(0);
                        error_curve.append(point_error);
                }
                QwtPointSeriesData* valid_curve_r = new QwtPointSeriesData(valid_curve);
                QwtPointSeriesData* error_curve_r = new QwtPointSeriesData(error_curve);//误差曲线
                valid_curve_t_5->setData(valid_curve_r);
                error_curve_t_5->setData(error_curve_r);//误差曲线
                if(graph_para[1].gra_node_num==100)
                {
                    ui->data_plot_5->setAxisAutoScale(QwtPlot::xBottom, false);
                    ui->data_plot_5->setAxisScale(QwtPlot::xBottom,0,10,1);
                    ui->data_plot_5->replot();
                    ui->error_plot_5->setAxisAutoScale(QwtPlot::xBottom, false);//误差曲线
                    ui->error_plot_5->setAxisScale(QwtPlot::xBottom,0,10,1);
                    ui->error_plot_5->replot();
                }
                ui->data_plot_5->replot();
                ui->error_plot_5->replot();
                graph_para[1].state=STOP;
        }
    //显示数据
        if(graph_para[0].state)
        {
            //rule_extend();
            QVector<QPointF> design_curve;
            QPointF point;
            for(int i =0;i<graph_para[0].gra_node_num;i++){
                    point.setX(float(graph_para[0].gra_last_time[i]));
                    point.setY((float)graph_para[0].gra_volt[i]);
                    design_curve.append(point);
            }
            QwtPointSeriesData* design_curve_r = new QwtPointSeriesData(design_curve);
            design_curve_t_5->setData(design_curve_r);
            ui->data_plot_5->replot();
            //qDebug()<<design_curve.data()->x()<<""<<design_curve.data()->y();
        }
}

void MainWindow::on_clear_5_clicked()
{
    auto_state = OFF;
    QModelIndex indextemp;
    for(int i=0;i<ui->tableView_5->model()->rowCount();i++)        //清零表格中的数据
    {
        for(int j=0;j<3;j++)
        {
            indextemp = ui->tableView_5->model()->index(i,j);
            ui->tableView_5->model()->setData(indextemp,0.0);
        }
    }
    memset(graph_para[0].gra_volt, 0, 100*sizeof(float) );
    memset(graph_para[1].gra_volt, 0, 100*sizeof(float) );
    graph_para[0].state=INITIAL;
    graph_para[0].gra_scan_row=0;
    data_temp[0].down_valid_row_num=0;
    //修复bug，清除上一幅图像的数据和本地缓存
    memset(data_temp[0].loop_times,0,100*sizeof(int));

    //清除曲线名
    ui->tabWidget->setTabText(4,"tab_5");

    //清除LCDnumber
    ui->lcdNumber_13->display(0);
    ui->lcdNumber_15->display(0);
    ui->lcdNumber_14->display(0);
    ui->lcdNumber_27->display(0);
    ui->lcdNumber_26->display(0);
    ui->lcdNumber_30->display(0);//=================

    graph_para[1].state=RESET;
    graph_para[1].final_x=0;
    graph_down_replot_5();

    //数据图坐标更新回复
    ui->data_plot_5->setAxisScale(QwtPlot::yLeft,-5,5,1);
    ui->data_plot_5->setAxisScale(QwtPlot::xBottom,0,10,1);
    QVector<QPointF> rule;                   //数据参考线
    for(int i =-1;i<101;i++){
        QPointF point;
        point.setX(i);
        int y = 0;
        point.setY(y);
        rule.append(point);
    }
    QwtPointSeriesData* rule_c = new QwtPointSeriesData(rule);
    rule_t_5 = new QwtPlotCurve("Curve 0");
    rule_t_5->setData(rule_c);
    rule_t_5->attach(ui->data_plot_5);

    QVector<QPointF> rule_error;             //误差参考线
    for(int i =-1;i<101;i++){
        QPointF point_error;
        point_error.setX(i);
        int y = 0;
        point_error.setY(y);
        rule_error.append(point_error);
    }
    QwtPointSeriesData* rule_error_c = new QwtPointSeriesData(rule_error);
    rule_error_t_5 = new QwtPlotCurve("Curve 0");
    rule_error_t_5->setData(rule_error_c);
    rule_error_t_5->attach(ui->error_plot_5);

    valid_curve_t_5->setVisible(false);            //隐藏实际曲线
    ui->error_plot_5->replot();
    error_curve_t_5->setVisible(false);            //隐藏误差曲线
    ui->error_plot_5->replot();
    design_curve_t_5->setVisible(false);            //隐藏误差曲线
    ui->error_plot_5->replot();
    QVector<QPointF> rule_a;
    QVector<QPointF> rule_error_a;//误差曲线
    for(int i =0;i<100;i++){
        QPointF point;
        QPointF point_error;            //误差曲线
        point.setX(i);
        point_error.setX(i);            //误差曲线
        point.setY(0);
        point_error.setY(0);            //误差曲线
        rule_a.append(point);
        rule_error_a.append(point_error);//误差曲线
    }
    QwtPointSeriesData* rule_r = new QwtPointSeriesData(rule_a);
    rule_t_5->setData(rule_r);
    QwtPointSeriesData* rule_error_r = new QwtPointSeriesData(rule_error_a);//误差曲线
    rule_error_t_5->setData(rule_error_r);//误差曲线
    ui->data_plot_5->replot();
    ui->error_plot_5->replot();
    stop_excute();
}

void MainWindow::test_text_5()
{
    qDebug()<<"NO.5 chart:";
    QModelIndex indextemp_5;
    float temp;
    int i=0;
    int j=0;
    for( i = 0;i<ui->tableView_5->model()->rowCount();i++)
    {
        for( j=0;j<3;j++)
        {
            indextemp_5 = ui->tableView_5->model()->index(i,j);
            temp=ui->tableView_5->model()->data(indextemp_5).toFloat();
            datatemp[i][j]=temp;
        }
        if((datatemp[i][0]==-1)&&(datatemp[i][1]==-1)&&(datatemp[i][2]==-1))
        {
            data_temp[0].down_valid_row_num=i;  //下传数据有效行数
            break;
        }
    }
    //

    int k=0;
    int flag=0;
    //第一个循环用来寻找第一个循环头
    for(int i=0;i<data_temp[0].down_valid_row_num;i++)
    {
        if (datatemp[i][2]<=-1)
        {
                qDebug()<<"there is a mistake:"<<i+1;
                flag=1;
                break;
        }
        else
        {
            if (datatemp[i][2]>0)
            {
                k=i;
                break;
            }
        }
    }

    //以下才是真正的判断循环体的正确性。当然，零的存在打断了部分循环。而且为了简化最后一个else导致复杂化了仲间的两个if判断
    for(i=k+1;i<data_temp[0].down_valid_row_num;i++)
    {
        if(flag) break;
        else
        {
            if (i==k+1)
            {
                if (datatemp[k][2]>0)
                {
                    if (datatemp[i][2]>=0||datatemp[i][2]<-1)
                    {
                        qDebug()<<"there is a mistake:"<<i+1;
                        break;
                    }
                    else if (i==data_temp[0].down_valid_row_num-1)
                    {
                        qDebug()<<"all is correct";
                    }
                }
                else
                {
                    for (i=k+1;i<data_temp[0].down_valid_row_num;i++)
                    {
                        if (datatemp[i][2]<=-1)
                        {
                                qDebug()<<"there is a mistake:"<<i+1;
                                flag=1;
                                break;
                        }
                        else if (datatemp[i][2]>0)
                        {
                            if (i!=data_temp[0].down_valid_row_num-1)
                            {
                                k=i;
                                break;
                            }
                            else
                            {
                                qDebug()<<"the last row is wrong and so pity";
                                flag=1;
                                break;
                            }
                        }
                        else if(datatemp[i][2]==0&&i==data_temp[0].down_valid_row_num-1)
                        qDebug()<<"all is correct";
                    }
                }

            }
            else if (i==data_temp[0].down_valid_row_num-1)
            {
                if (datatemp[i-1][2]==-1)
                {
                    if (datatemp[i][2]==-1||datatemp[i][2]==0) qDebug()<<"all is correct";
                    else
                    {
                        qDebug()<<"the last row is wrong and so pity";
                        break;
                    }
                }
                else if (datatemp[i-1][2]==0)
                {
                    if (datatemp[i][2]==0) qDebug()<<"all is correct";
                    else
                    {
                        qDebug()<<"the last row is wrong and so pity";
                        break;
                    }
                }
                else
                {
                    if (datatemp[i][2]==-1) qDebug()<<"all is correct";
                    else
                    {
                        qDebug()<<"the last row is wrong and so pity";
                        break;
                    }
                }
            }
            else
            {
                if(datatemp[i][2]<-1)
                {
                    qDebug()<<"there is a mistake:"<<i+1;
                    break;
                }
                else
                {
                    if(datatemp[i][2]!=-1)
                    {
                        k=i;
                        qDebug()<<"current circle is correct"<<i;
                    }
                }
            }
        }
    }
}

void MainWindow::on_action_triggered()
{
    about->show();
}
