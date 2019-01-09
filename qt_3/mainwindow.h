#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "name_dialog.h" //==========================================
#include "newwindow.h"
#include "exit_dialog.h"
#include <QVector>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_legend.h>
#include <qwt_point_data.h>
#include <QCloseEvent>
#include "warning.h"
#include <acc.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    newwindow *new_win;                 //串口设置界面
    Exit_Dialog *exit_dialog;           //退出询问界面
    Dialog *name_dialog;//存储名界面=================================================================
    Form *warning;
    acc *about;

    QSerialPort *serial=nullptr;
    QString *tab_name=nullptr;
//tab1
    QwtPlotCurve* design_curve_t;       //设计曲线
    QwtPlotCurve* valid_curve_t;        //有效曲线
    QwtPlotCurve* error_curve_t;        //误差曲线
    QwtPlotCurve* rule_t;               //数据参考线
    QwtPlotCurve* rule_error_t;               //数据参考线
//tab2
    QwtPlotCurve* design_curve_t_2;       //设计曲线
    QwtPlotCurve* valid_curve_t_2;        //有效曲线
    QwtPlotCurve* error_curve_t_2;        //误差曲线
    QwtPlotCurve* rule_t_2;               //数据参考线
    QwtPlotCurve* rule_error_t_2;               //数据参考线
//tab3
    QwtPlotCurve* design_curve_t_3;       //设计曲线
    QwtPlotCurve* valid_curve_t_3;        //有效曲线
    QwtPlotCurve* error_curve_t_3;        //误差曲线
    QwtPlotCurve* rule_t_3;               //数据参考线
    QwtPlotCurve* rule_error_t_3;               //数据参考线
//tab4
    QwtPlotCurve* design_curve_t_4;       //设计曲线
    QwtPlotCurve* valid_curve_t_4;        //有效曲线
    QwtPlotCurve* error_curve_t_4;        //误差曲线
    QwtPlotCurve* rule_t_4;               //数据参考线
    QwtPlotCurve* rule_error_t_4;               //数据参考线
//tab5
    QwtPlotCurve* design_curve_t_5;       //设计曲线
    QwtPlotCurve* valid_curve_t_5;        //有效曲线
    QwtPlotCurve* error_curve_t_5;        //误差曲线
    QwtPlotCurve* rule_t_5;               //数据参考线
    QwtPlotCurve* rule_error_t_5;               //数据参考线

    ~MainWindow();
signals:
    void WriteTimestop();
private slots:
    void delaymsec(int msec);

    void timerEvent(QTimerEvent *);

    void on_actionExit_triggered();

    void on_actionSetting_triggered();      //串口设置

    void setpage(int index);

    void on_actionsave_all_triggered();

    void on_actionRestore_All_triggered();

    void on_actionBackup_ALL_triggered();

    void on_actionLoad_ALL_triggered();

    void on_actionAuto_triggered();

    bool dataBackup();                  // 文件备份

    void closeEvent(QCloseEvent *event);    //关闭窗口事件

    void serial_end();

    void rule_extend();         //延长参考线

    void ReadData();        //串口读取数据

    void serial_close();

    void save_data(float timedata,float voltdata,float voltdata2,int current_valid,int loopcount,int hour);

    void save_slot();//键入曲线名，并使after运行保存==================================
//tab1控件管理
    void on_download_clicked();           //download

    void on_save_clicked_after();         //保存

    void on_save_clicked();           // 弹窗

    void on_load_clicked();         //load

    void on_excute_clicked();         //excute

    void graph_down_replot();//设计曲线绘制

    void on_clear_clicked();            //clear按键

    void test_text();        //检测1
//tab2控件管理
    void on_download_2_clicked();           //download

    void on_save_2_clicked();         //弹窗

    void on_save_2_clicked_after();   //保存

    void on_load_2_clicked();         //load

    void on_excute_2_clicked();         //excute

    void graph_down_replot_2();//设计曲线绘制

    void on_clear_2_clicked();            //clear按键

    void test_text_2();        //检测2
//tab3控件管理
    void on_download_3_clicked();           //download

    void on_save_3_clicked();         //弹窗曲线名实现

    void on_save_3_clicked_after();   //保存

    void on_load_3_clicked();         //load

    void on_excute_3_clicked();         //excute

    void graph_down_replot_3();//设计曲线绘制

    void on_clear_3_clicked();            //clear按键

    void test_text_3();        //检测3
//tab4控件管理
    void on_download_4_clicked();           //download

    void on_save_4_clicked();         //弹窗曲线名实现

    void on_save_4_clicked_after();   //保存

    void on_load_4_clicked();         //load

    void on_excute_4_clicked();         //excute

    void graph_down_replot_4();//设计曲线绘制

    void on_clear_4_clicked();            //clear按键

    void test_text_4();        //检测4
//tab4控件管理
    void on_download_5_clicked();           //download

    void on_save_5_clicked();         //弹窗曲线名实现

    void on_save_5_clicked_after();   //保存

    void on_load_5_clicked();         //load

    void on_excute_5_clicked();         //excute

    void graph_down_replot_5();//设计曲线绘制

    void on_clear_5_clicked();            //clear按键

    void test_text_5();        //检测5
//界面初始化
    void tab_1_ini();

    void tab_2_ini();

    void tab_3_ini();

    void tab_4_ini();

    void tab_5_ini();
//预留
    void stop_excute();     //停止执行

    void clear_excute();    //清除单片机内缓存

    void on_action_triggered();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
