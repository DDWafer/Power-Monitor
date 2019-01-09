/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "qwt_plot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionExit;
    QAction *actionSetting;
    QAction *actionsave_all;
    QAction *actionRestore_All;
    QAction *actionBackup_ALL;
    QAction *actionLoad_ALL;
    QAction *actionAuto;
    QAction *actionSetting_2;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;

    QWidget *tab;
    QGridLayout *gridLayout_5;
    QTableView *tableView;
    QWidget *widget;
    QGridLayout *gridLayout_3;
    QwtPlot *data_plot;
    QWidget *widget_2;
    QGridLayout *gridLayout_4;
    QwtPlot *error_plot;
    QGridLayout *gridLayout_2;
    QPushButton *clear;
    QPushButton *download;
    QPushButton *load;
    QPushButton *save;
    QPushButton *excute;

    QWidget *tab_2;
    QGridLayout *gridLayout_9;
    QTableView *tableView_2;
    QWidget *widget_3;
    QGridLayout *gridLayout_6;
    QwtPlot *data_plot_2;
    QWidget *widget_4;
    QGridLayout *gridLayout_8;
    QwtPlot *error_plot_2;
    QGridLayout *gridLayout_7;
    QPushButton *clear_2;
    QPushButton *download_2;
    QPushButton *load_2;
    QPushButton *save_2;
    QPushButton *excute_2;
//
    QWidget *tab_3;
    QGridLayout *gridLayout_13;
    QTableView *tableView_3;
    QWidget *widget_5;
    QGridLayout *gridLayout_10;
    QwtPlot *data_plot_3;
    QWidget *widget_6;
    QGridLayout *gridLayout_12;
    QwtPlot *error_plot_3;
    QGridLayout *gridLayout_11;
    QPushButton *clear_3;
    QPushButton *download_3;
    QPushButton *load_3;
    QPushButton *save_3;
    QPushButton *excute_3;

//
    QWidget *tab_4;
    QGridLayout *gridLayout_17;
    QTableView *tableView_4;
    QWidget *widget_7;
    QGridLayout *gridLayout_14;
    QwtPlot *data_plot_4;
    QWidget *widget_8;
    QGridLayout *gridLayout_16;
    QwtPlot *error_plot_4;
    QGridLayout *gridLayout_15;
    QPushButton *clear_4;
    QPushButton *download_4;
    QPushButton *load_4;
    QPushButton *save_4;
    QPushButton *excute_4;
    //
    QWidget *tab_5;
    QGridLayout *gridLayout_21;
    QTableView *tableView_5;
    QWidget *widget_9;
    QGridLayout *gridLayout_18;
    QwtPlot *data_plot_5;
    QWidget *widget_10;
    QGridLayout *gridLayout_20;
    QwtPlot *error_plot_5;
    QGridLayout *gridLayout_19;
    QPushButton *clear_5;
    QPushButton *download_5;
    QPushButton *load_5;
    QPushButton *save_5;
    QPushButton *excute_5;


    QMenuBar *menuBar;
    QMenu *menufile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setEnabled(true);
        MainWindow->resize(824, 622);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionSetting = new QAction(MainWindow);
        actionSetting->setObjectName(QStringLiteral("actionSetting"));
        actionsave_all = new QAction(MainWindow);
        actionsave_all->setObjectName(QStringLiteral("actionsave_all"));
        actionRestore_All = new QAction(MainWindow);
        actionRestore_All->setObjectName(QStringLiteral("actionRestore_All"));
        actionBackup_ALL = new QAction(MainWindow);
        actionBackup_ALL->setObjectName(QStringLiteral("actionBackup_ALL"));
        actionLoad_ALL = new QAction(MainWindow);
        actionLoad_ALL->setObjectName(QStringLiteral("actionLoad_ALL"));
        actionAuto = new QAction(MainWindow);
        actionAuto->setObjectName(QStringLiteral("actionAuto"));
        actionAuto->setCheckable(true);
        actionSetting_2 = new QAction(MainWindow);
        actionSetting_2->setObjectName(QStringLiteral("actionSetting_2"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
     //
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        gridLayout_5 = new QGridLayout(tab);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        tableView = new QTableView(tab);
        tableView->setObjectName(QStringLiteral("tableView"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tableView->sizePolicy().hasHeightForWidth());
        tableView->setSizePolicy(sizePolicy1);

        gridLayout_5->addWidget(tableView, 0, 0, 3, 1);

        widget = new QWidget(tab);
        widget->setObjectName(QStringLiteral("widget"));
        gridLayout_3 = new QGridLayout(widget);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        data_plot = new QwtPlot(widget);
        data_plot->setObjectName(QStringLiteral("data_plot"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(data_plot->sizePolicy().hasHeightForWidth());
        data_plot->setSizePolicy(sizePolicy2);

        gridLayout_3->addWidget(data_plot, 0, 0, 1, 1);


        gridLayout_5->addWidget(widget, 0, 1, 1, 1);

        widget_2 = new QWidget(tab);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        gridLayout_4 = new QGridLayout(widget_2);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        error_plot = new QwtPlot(widget_2);
        error_plot->setObjectName(QStringLiteral("error_plot"));
        sizePolicy2.setHeightForWidth(error_plot->sizePolicy().hasHeightForWidth());
        error_plot->setSizePolicy(sizePolicy2);

        gridLayout_4->addWidget(error_plot, 0, 0, 1, 1);


        gridLayout_5->addWidget(widget_2, 1, 1, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        clear = new QPushButton(tab);
        clear->setObjectName(QStringLiteral("clear"));

        gridLayout_2->addWidget(clear, 0, 0, 1, 1);

        download = new QPushButton(tab);
        download->setObjectName(QStringLiteral("download"));

        gridLayout_2->addWidget(download, 0, 1, 1, 1);

        load = new QPushButton(tab);
        load->setObjectName(QStringLiteral("load"));

        gridLayout_2->addWidget(load, 0, 2, 1, 1);

        save = new QPushButton(tab);
        save->setObjectName(QStringLiteral("save"));

        gridLayout_2->addWidget(save, 0, 3, 1, 1);

        excute = new QPushButton(tab);
        excute->setObjectName(QStringLiteral("excute"));

        gridLayout_2->addWidget(excute, 0, 4, 1, 1);


        gridLayout_5->addLayout(gridLayout_2, 2, 1, 1, 1);

        tabWidget->addTab(tab, QString());
//
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        gridLayout_9 = new QGridLayout(tab_2);
        gridLayout_9->setSpacing(6);
        gridLayout_9->setContentsMargins(11, 11, 11, 11);
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        tableView_2 = new QTableView(tab_2);
        tableView_2->setObjectName(QStringLiteral("tableView_2"));
        sizePolicy1.setHeightForWidth(tableView_2->sizePolicy().hasHeightForWidth());
        tableView_2->setSizePolicy(sizePolicy1);

        gridLayout_9->addWidget(tableView_2, 0, 0, 3, 1);

        widget_3 = new QWidget(tab_2);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        gridLayout_6 = new QGridLayout(widget_3);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        data_plot_2 = new QwtPlot(widget_3);
        data_plot_2->setObjectName(QStringLiteral("data_plot_2"));
        sizePolicy2.setHeightForWidth(data_plot_2->sizePolicy().hasHeightForWidth());
        data_plot_2->setSizePolicy(sizePolicy2);

        gridLayout_6->addWidget(data_plot_2, 0, 0, 1, 1);


        gridLayout_9->addWidget(widget_3, 0, 1, 1, 1);

        widget_4 = new QWidget(tab_2);
        widget_4->setObjectName(QStringLiteral("widget_4"));
        gridLayout_8 = new QGridLayout(widget_4);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        error_plot_2 = new QwtPlot(widget_4);
        error_plot_2->setObjectName(QStringLiteral("error_plot_2"));
        sizePolicy2.setHeightForWidth(error_plot_2->sizePolicy().hasHeightForWidth());
        error_plot_2->setSizePolicy(sizePolicy2);

        gridLayout_8->addWidget(error_plot_2, 0, 0, 1, 1);


        gridLayout_9->addWidget(widget_4, 1, 1, 1, 1);

        gridLayout_7 = new QGridLayout();
        gridLayout_7->setSpacing(6);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        clear_2 = new QPushButton(tab_2);
        clear_2->setObjectName(QStringLiteral("clear_2"));

        gridLayout_7->addWidget(clear_2, 0, 0, 1, 1);

        download_2 = new QPushButton(tab_2);
        download_2->setObjectName(QStringLiteral("download_2"));

        gridLayout_7->addWidget(download_2, 0, 1, 1, 1);

        load_2 = new QPushButton(tab_2);
        load_2->setObjectName(QStringLiteral("load_2"));

        gridLayout_7->addWidget(load_2, 0, 2, 1, 1);

        save_2 = new QPushButton(tab_2);
        save_2->setObjectName(QStringLiteral("save_2"));

        gridLayout_7->addWidget(save_2, 0, 3, 1, 1);

        excute_2 = new QPushButton(tab_2);
        excute_2->setObjectName(QStringLiteral("excute_2"));

        gridLayout_7->addWidget(excute_2, 0, 4, 1, 1);


        gridLayout_9->addLayout(gridLayout_7, 2, 1, 1, 1);

        tabWidget->addTab(tab_2, QString());
//
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        gridLayout_13 = new QGridLayout(tab_3);
        gridLayout_13->setSpacing(6);
        gridLayout_13->setContentsMargins(11, 11, 11, 11);
        gridLayout_13->setObjectName(QStringLiteral("gridLayout_13"));
        tableView_3 = new QTableView(tab_3);
        tableView_3->setObjectName(QStringLiteral("tableView_3"));
        sizePolicy1.setHeightForWidth(tableView_3->sizePolicy().hasHeightForWidth());
        tableView_3->setSizePolicy(sizePolicy1);

        gridLayout_13->addWidget(tableView_3, 0, 0, 3, 1);

        widget_5 = new QWidget(tab_3);
        widget_5->setObjectName(QStringLiteral("widget_5"));
        gridLayout_10 = new QGridLayout(widget_5);
        gridLayout_10->setSpacing(6);
        gridLayout_10->setContentsMargins(11, 11, 11, 11);
        gridLayout_10->setObjectName(QStringLiteral("gridLayout_10"));
        data_plot_3 = new QwtPlot(widget_5);
        data_plot_3->setObjectName(QStringLiteral("data_plot_3"));
        sizePolicy2.setHeightForWidth(data_plot_3->sizePolicy().hasHeightForWidth());
        data_plot_3->setSizePolicy(sizePolicy2);

        gridLayout_10->addWidget(data_plot_3, 0, 0, 1, 1);


        gridLayout_13->addWidget(widget_5, 0, 1, 1, 1);

        widget_6 = new QWidget(tab_3);
        widget_6->setObjectName(QStringLiteral("widget_6"));
        gridLayout_12 = new QGridLayout(widget_6);
        gridLayout_12->setSpacing(6);
        gridLayout_12->setContentsMargins(11, 11, 11, 11);
        gridLayout_12->setObjectName(QStringLiteral("gridLayout_12"));
        error_plot_3 = new QwtPlot(widget_6);
        error_plot_3->setObjectName(QStringLiteral("error_plot_3"));
        sizePolicy2.setHeightForWidth(error_plot_3->sizePolicy().hasHeightForWidth());
        error_plot_3->setSizePolicy(sizePolicy2);

        gridLayout_12->addWidget(error_plot_3, 0, 0, 1, 1);


        gridLayout_13->addWidget(widget_6, 1, 1, 1, 1);

        gridLayout_11 = new QGridLayout();
        gridLayout_11->setSpacing(6);
        gridLayout_11->setObjectName(QStringLiteral("gridLayout_11"));
        clear_3 = new QPushButton(tab_3);
        clear_3->setObjectName(QStringLiteral("clear_3"));

        gridLayout_11->addWidget(clear_3, 0, 0, 1, 1);

        download_3 = new QPushButton(tab_3);
        download_3->setObjectName(QStringLiteral("download_3"));

        gridLayout_11->addWidget(download_3, 0, 1, 1, 1);

        load_3 = new QPushButton(tab_3);
        load_3->setObjectName(QStringLiteral("load_3"));

        gridLayout_11->addWidget(load_3, 0, 2, 1, 1);

        save_3 = new QPushButton(tab_3);
        save_3->setObjectName(QStringLiteral("save_3"));

        gridLayout_11->addWidget(save_3, 0, 3, 1, 1);

        excute_3 = new QPushButton(tab_3);
        excute_3->setObjectName(QStringLiteral("excute_3"));

        gridLayout_11->addWidget(excute_3, 0, 4, 1, 1);


        gridLayout_13->addLayout(gridLayout_11, 2, 1, 1, 1);

        tabWidget->addTab(tab_3, QString());
//
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        gridLayout_17 = new QGridLayout(tab_4);
        gridLayout_17->setSpacing(6);
        gridLayout_17->setContentsMargins(11, 11, 11, 11);
        gridLayout_17->setObjectName(QStringLiteral("gridLayout_17"));
        tableView_4 = new QTableView(tab_4);
        tableView_4->setObjectName(QStringLiteral("tableView_4"));
        sizePolicy1.setHeightForWidth(tableView_4->sizePolicy().hasHeightForWidth());
        tableView_4->setSizePolicy(sizePolicy1);

        gridLayout_17->addWidget(tableView_4, 0, 0, 3, 1);

        widget_7 = new QWidget(tab_4);
        widget_7->setObjectName(QStringLiteral("widget_7"));
        gridLayout_14 = new QGridLayout(widget_7);
        gridLayout_14->setSpacing(6);
        gridLayout_14->setContentsMargins(11, 11, 11, 11);
        gridLayout_14->setObjectName(QStringLiteral("gridLayout_14"));
        data_plot_4 = new QwtPlot(widget_7);
        data_plot_4->setObjectName(QStringLiteral("data_plot_4"));
        sizePolicy2.setHeightForWidth(data_plot_4->sizePolicy().hasHeightForWidth());
        data_plot_4->setSizePolicy(sizePolicy2);

        gridLayout_14->addWidget(data_plot_4, 0, 0, 1, 1);


        gridLayout_17->addWidget(widget_7, 0, 1, 1, 1);

        widget_8 = new QWidget(tab_4);
        widget_8->setObjectName(QStringLiteral("widget_8"));
        gridLayout_16 = new QGridLayout(widget_8);
        gridLayout_16->setSpacing(6);
        gridLayout_16->setContentsMargins(11, 11, 11, 11);
        gridLayout_16->setObjectName(QStringLiteral("gridLayout_16"));
        error_plot_4 = new QwtPlot(widget_8);
        error_plot_4->setObjectName(QStringLiteral("error_plot_4"));
        sizePolicy2.setHeightForWidth(error_plot_4->sizePolicy().hasHeightForWidth());
        error_plot_4->setSizePolicy(sizePolicy2);

        gridLayout_16->addWidget(error_plot_4, 0, 0, 1, 1);


        gridLayout_17->addWidget(widget_8, 1, 1, 1, 1);

        gridLayout_15 = new QGridLayout();
        gridLayout_15->setSpacing(6);
        gridLayout_15->setObjectName(QStringLiteral("gridLayout_15"));
        clear_4 = new QPushButton(tab_4);
        clear_4->setObjectName(QStringLiteral("clear_4"));

        gridLayout_15->addWidget(clear_4, 0, 0, 1, 1);

        download_4 = new QPushButton(tab_4);
        download_4->setObjectName(QStringLiteral("download_4"));

        gridLayout_15->addWidget(download_4, 0, 1, 1, 1);

        load_4 = new QPushButton(tab_4);
        load_4->setObjectName(QStringLiteral("load_4"));

        gridLayout_15->addWidget(load_4, 0, 2, 1, 1);

        save_4 = new QPushButton(tab_4);
        save_4->setObjectName(QStringLiteral("save_4"));

        gridLayout_15->addWidget(save_4, 0, 3, 1, 1);

        excute_4 = new QPushButton(tab_4);
        excute_4->setObjectName(QStringLiteral("excute_4"));

        gridLayout_15->addWidget(excute_4, 0, 4, 1, 1);


        gridLayout_17->addLayout(gridLayout_15, 2, 1, 1, 1);

        tabWidget->addTab(tab_4, QString());
//
        tab_5 = new QWidget();
        tab_5->setObjectName(QStringLiteral("tab_5"));
        gridLayout_21 = new QGridLayout(tab_5);
        gridLayout_21->setSpacing(6);
        gridLayout_21->setContentsMargins(11, 11, 11, 11);
        gridLayout_21->setObjectName(QStringLiteral("gridLayout_21"));
        tableView_5 = new QTableView(tab_5);
        tableView_5->setObjectName(QStringLiteral("tableView_5"));
        sizePolicy1.setHeightForWidth(tableView_5->sizePolicy().hasHeightForWidth());
        tableView_5->setSizePolicy(sizePolicy1);

        gridLayout_21->addWidget(tableView_5, 0, 0, 3, 1);

        widget_9 = new QWidget(tab_5);
        widget_9->setObjectName(QStringLiteral("widget_9"));
        gridLayout_18 = new QGridLayout(widget_9);
        gridLayout_18->setSpacing(6);
        gridLayout_18->setContentsMargins(11, 11, 11, 11);
        gridLayout_18->setObjectName(QStringLiteral("gridLayout_18"));
        data_plot_5 = new QwtPlot(widget_9);
        data_plot_5->setObjectName(QStringLiteral("data_plot_4"));
        sizePolicy2.setHeightForWidth(data_plot_5->sizePolicy().hasHeightForWidth());
        data_plot_5->setSizePolicy(sizePolicy2);

        gridLayout_18->addWidget(data_plot_5, 0, 0, 1, 1);


        gridLayout_21->addWidget(widget_9, 0, 1, 1, 1);

        widget_10 = new QWidget(tab_5);
        widget_10->setObjectName(QStringLiteral("widget_10"));
        gridLayout_20 = new QGridLayout(widget_10);
        gridLayout_20->setSpacing(6);
        gridLayout_20->setContentsMargins(11, 11, 11, 11);
        gridLayout_20->setObjectName(QStringLiteral("gridLayout_20"));
        error_plot_5 = new QwtPlot(widget_10);
        error_plot_5->setObjectName(QStringLiteral("error_plot_4"));
        sizePolicy2.setHeightForWidth(error_plot_5->sizePolicy().hasHeightForWidth());
        error_plot_5->setSizePolicy(sizePolicy2);

        gridLayout_20->addWidget(error_plot_5, 0, 0, 1, 1);


        gridLayout_21->addWidget(widget_10, 1, 1, 1, 1);

        gridLayout_19 = new QGridLayout();
        gridLayout_19->setSpacing(6);
        gridLayout_19->setObjectName(QStringLiteral("gridLayout_19"));
        clear_5 = new QPushButton(tab_5);
        clear_5->setObjectName(QStringLiteral("clear_5"));

        gridLayout_19->addWidget(clear_5, 0, 0, 1, 1);

        download_5 = new QPushButton(tab_5);
        download_5->setObjectName(QStringLiteral("download_5"));

        gridLayout_19->addWidget(download_5, 0, 1, 1, 1);

        load_5 = new QPushButton(tab_5);
        load_5->setObjectName(QStringLiteral("load_5"));

        gridLayout_19->addWidget(load_5, 0, 2, 1, 1);

        save_5 = new QPushButton(tab_5);
        save_5->setObjectName(QStringLiteral("save_5"));

        gridLayout_19->addWidget(save_5, 0, 3, 1, 1);

        excute_5 = new QPushButton(tab_5);
        excute_5->setObjectName(QStringLiteral("excute_5"));

        gridLayout_19->addWidget(excute_5, 0, 4, 1, 1);


        gridLayout_21->addLayout(gridLayout_19, 2, 1, 1, 1);

        tabWidget->addTab(tab_5, QString());
        //
        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 824, 23));
        menufile = new QMenu(menuBar);
        menufile->setObjectName(QStringLiteral("menufile"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menufile->menuAction());
        menufile->addAction(actionExit);
        menufile->addAction(actionSetting);
        menufile->addAction(actionsave_all);
        menufile->addAction(actionRestore_All);
        menufile->addAction(actionBackup_ALL);
        menufile->addAction(actionLoad_ALL);
        menufile->addAction(actionAuto);
        menufile->addAction(actionSetting_2);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", nullptr));
        actionSetting->setText(QApplication::translate("MainWindow", "Serial Setting", nullptr));
        actionsave_all->setText(QApplication::translate("MainWindow", "Save ALL", nullptr));
        actionRestore_All->setText(QApplication::translate("MainWindow", "Restore ALL", nullptr));
        actionBackup_ALL->setText(QApplication::translate("MainWindow", "Backup", nullptr));
        actionLoad_ALL->setText(QApplication::translate("MainWindow", "Load ALL", nullptr));
        actionAuto->setText(QApplication::translate("MainWindow", "Auto", nullptr));
        actionSetting_2->setText(QApplication::translate("MainWindow", "Setting", nullptr));

        clear->setText(QApplication::translate("MainWindow", "\346\270\205\345\261\217", nullptr));
        download->setText(QApplication::translate("MainWindow", "\344\270\213\350\275\275", nullptr));
        load->setText(QApplication::translate("MainWindow", "\350\257\273\345\217\226\346\234\254\345\234\260\346\226\207\344\273\266", nullptr));
        save->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230\345\210\260\346\234\254\345\234\260", nullptr));
        excute->setText(QApplication::translate("MainWindow", "\346\211\247\350\241\214", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Tab 1", nullptr));

        clear_2->setText(QApplication::translate("MainWindow", "\346\270\205\345\261\217", nullptr));
        download_2->setText(QApplication::translate("MainWindow", "\344\270\213\350\275\275", nullptr));
        load_2->setText(QApplication::translate("MainWindow",  "\350\257\273\345\217\226\346\234\254\345\234\260\346\226\207\344\273\266", nullptr));
        save_2->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230\345\210\260\346\234\254\345\234\260", nullptr));
        excute_2->setText(QApplication::translate("MainWindow",  "\346\211\247\350\241\214", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "tab_2", nullptr));

        clear_3->setText(QApplication::translate("MainWindow", "\346\270\205\345\261\217", nullptr));
        download_3->setText(QApplication::translate("MainWindow", "\344\270\213\350\275\275", nullptr));
        load_3->setText(QApplication::translate("MainWindow",  "\350\257\273\345\217\226\346\234\254\345\234\260\346\226\207\344\273\266", nullptr));
        save_3->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230\345\210\260\346\234\254\345\234\260", nullptr));
        excute_3->setText(QApplication::translate("MainWindow",  "\346\211\247\350\241\214", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainWindow", "tab_3", nullptr));

        clear_4->setText(QApplication::translate("MainWindow", "\346\270\205\345\261\217", nullptr));
        download_4->setText(QApplication::translate("MainWindow", "\344\270\213\350\275\275", nullptr));
        load_4->setText(QApplication::translate("MainWindow", "\350\257\273\345\217\226\346\234\254\345\234\260\346\226\207\344\273\266", nullptr));
        save_4->setText(QApplication::translate("MainWindow","\344\277\235\345\255\230\345\210\260\346\234\254\345\234\260", nullptr));
        excute_4->setText(QApplication::translate("MainWindow",  "\346\211\247\350\241\214", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("MainWindow", "tab_4", nullptr));

        clear_5->setText(QApplication::translate("MainWindow", "\346\270\205\345\261\217", nullptr));
        download_5->setText(QApplication::translate("MainWindow", "\344\270\213\350\275\275", nullptr));
        load_5->setText(QApplication::translate("MainWindow",  "\350\257\273\345\217\226\346\234\254\345\234\260\346\226\207\344\273\266", nullptr));
        save_5->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230\345\210\260\346\234\254\345\234\260", nullptr));
        excute_5->setText(QApplication::translate("MainWindow",  "\346\211\247\350\241\214", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_5), QApplication::translate("MainWindow", "tab_5", nullptr));

        menufile->setTitle(QApplication::translate("MainWindow", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
