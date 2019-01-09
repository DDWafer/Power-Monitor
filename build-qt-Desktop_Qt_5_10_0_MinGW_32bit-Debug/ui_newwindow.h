/********************************************************************************
** Form generated from reading UI file 'newwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWWINDOW_H
#define UI_NEWWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_newwindow
{
public:
    QWidget *centralwidget;
    QComboBox *BaudBox;
    QComboBox *BitBox;
    QComboBox *PortBox;
    QPushButton *OpenSerialButton;
    QComboBox *FlowBox;
    QComboBox *ParityBox;
    QLabel *label;
    QLabel *label_6;
    QLabel *label_3;
    QLabel *label_7;
    QLabel *label_5;
    QLabel *label_2;
    QComboBox *FlowBox_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *newwindow)
    {
        if (newwindow->objectName().isEmpty())
            newwindow->setObjectName(QStringLiteral("newwindow"));
        newwindow->resize(273, 364);
        centralwidget = new QWidget(newwindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        BaudBox = new QComboBox(centralwidget);
        BaudBox->addItem(QString());
        BaudBox->addItem(QString());
        BaudBox->addItem(QString());
        BaudBox->setObjectName(QStringLiteral("BaudBox"));
        BaudBox->setGeometry(QRect(130, 60, 69, 22));
        BitBox = new QComboBox(centralwidget);
        BitBox->addItem(QString());
        BitBox->setObjectName(QStringLiteral("BitBox"));
        BitBox->setGeometry(QRect(130, 90, 69, 22));
        PortBox = new QComboBox(centralwidget);
        PortBox->setObjectName(QStringLiteral("PortBox"));
        PortBox->setGeometry(QRect(130, 30, 69, 22));
        OpenSerialButton = new QPushButton(centralwidget);
        OpenSerialButton->setObjectName(QStringLiteral("OpenSerialButton"));
        OpenSerialButton->setGeometry(QRect(80, 250, 91, 23));
        FlowBox = new QComboBox(centralwidget);
        FlowBox->addItem(QString());
        FlowBox->setObjectName(QStringLiteral("FlowBox"));
        FlowBox->setGeometry(QRect(130, 150, 69, 22));
        ParityBox = new QComboBox(centralwidget);
        ParityBox->addItem(QString());
        ParityBox->setObjectName(QStringLiteral("ParityBox"));
        ParityBox->setGeometry(QRect(130, 120, 69, 22));
        label = new QLabel(centralwidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(60, 30, 54, 16));
        label_6 = new QLabel(centralwidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(60, 180, 51, 21));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(50, 90, 71, 20));
        label_7 = new QLabel(centralwidget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(40, 150, 81, 20));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(50, 120, 61, 20));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(50, 60, 61, 21));
        FlowBox_2 = new QComboBox(centralwidget);
        FlowBox_2->addItem(QString());
        FlowBox_2->setObjectName(QStringLiteral("FlowBox_2"));
        FlowBox_2->setGeometry(QRect(130, 180, 69, 22));
        newwindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(newwindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 273, 26));
        newwindow->setMenuBar(menubar);
        statusbar = new QStatusBar(newwindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        newwindow->setStatusBar(statusbar);

        retranslateUi(newwindow);

        QMetaObject::connectSlotsByName(newwindow);
    } // setupUi

    void retranslateUi(QMainWindow *newwindow)
    {
        newwindow->setWindowTitle(QApplication::translate("newwindow", "MainWindow", nullptr));
        BaudBox->setItemText(0, QApplication::translate("newwindow", "9600", nullptr));
        BaudBox->setItemText(1, QApplication::translate("newwindow", "4800", nullptr));
        BaudBox->setItemText(2, QApplication::translate("newwindow", "115200", nullptr));

        BitBox->setItemText(0, QApplication::translate("newwindow", "8", nullptr));

        OpenSerialButton->setText(QApplication::translate("newwindow", "\346\211\223\345\274\200\344\270\262\345\217\243", nullptr));
        FlowBox->setItemText(0, QApplication::translate("newwindow", "0", nullptr));

        ParityBox->setItemText(0, QApplication::translate("newwindow", "0", nullptr));

        label->setText(QApplication::translate("newwindow", "<html><head/><body><p><span style=\" font-size:12pt;\">\344\270\262\345\217\243\357\274\232</span></p></body></html>", nullptr));
        label_6->setText(QApplication::translate("newwindow", "<html><head/><body><p><span style=\" font-size:12pt;\">\346\265\201 \346\216\247\357\274\232</span></p></body></html>", nullptr));
        label_3->setText(QApplication::translate("newwindow", "<html><head/><body><p><span style=\" font-size:12pt;\">\346\225\260\346\215\256\344\275\215\357\274\232</span></p></body></html>", nullptr));
        label_7->setText(QApplication::translate("newwindow", "<html><head/><body><p><span style=\" font-size:12pt;\">\345\245\207\345\201\266\346\240\241\351\252\214\357\274\232</span></p></body></html>", nullptr));
        label_5->setText(QApplication::translate("newwindow", "<html><head/><body><p><span style=\" font-size:12pt;\">\345\201\234\346\255\242\344\275\215\357\274\232</span></p></body></html>", nullptr));
        label_2->setText(QApplication::translate("newwindow", "<html><head/><body><p><span style=\" font-size:12pt;\">\346\263\242\347\211\271\347\216\207\357\274\232</span></p></body></html>", nullptr));
        FlowBox_2->setItemText(0, QApplication::translate("newwindow", "0", nullptr));

    } // retranslateUi

};

namespace Ui {
    class newwindow: public Ui_newwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWWINDOW_H
