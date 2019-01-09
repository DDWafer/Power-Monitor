/********************************************************************************
** Form generated from reading UI file 'acc.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ACC_H
#define UI_ACC_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_acc
{
public:
    QPushButton *pushButton;
    QLabel *label;
    QListWidget *listWidget;
    QLabel *label_5;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_6;

    void setupUi(QWidget *acc)
    {
        if (acc->objectName().isEmpty())
            acc->setObjectName(QStringLiteral("acc"));
        acc->resize(497, 300);
        pushButton = new QPushButton(acc);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(270, 260, 93, 28));
        label = new QLabel(acc);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(130, 40, 151, 21));
        QFont font;
        font.setFamily(QString::fromUtf8("\347\255\211\347\272\277"));
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        listWidget = new QListWidget(acc);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(0, 0, 501, 251));
        listWidget->setFrameShape(QFrame::NoFrame);
        label_5 = new QLabel(acc);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(130, 210, 351, 16));
        layoutWidget = new QWidget(acc);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(130, 90, 178, 91));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout->addWidget(label_2);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout->addWidget(label_3);

        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        verticalLayout->addWidget(label_4);

        label_6 = new QLabel(acc);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(30, 50, 61, 61));
        listWidget->raise();
        layoutWidget->raise();
        pushButton->raise();
        label->raise();
        label_5->raise();
        label_6->raise();

        retranslateUi(acc);

        QMetaObject::connectSlotsByName(acc);
    } // setupUi

    void retranslateUi(QWidget *acc)
    {
        acc->setWindowTitle(QApplication::translate("acc", "About", nullptr));
        pushButton->setText(QApplication::translate("acc", "OK", nullptr));
        label->setText(QApplication::translate("acc", "Power_control", nullptr));
        label_5->setText(QApplication::translate("acc", "Provide fully customized software solutions.", nullptr));
        label_2->setText(QApplication::translate("acc", "Version:1.0.0.0", nullptr));
        label_3->setText(QApplication::translate("acc", "Copyright (C) 2018 Seu", nullptr));
        label_4->setText(QApplication::translate("acc", "http://www.seu.edu.cn", nullptr));
        label_6->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class acc: public Ui_acc {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ACC_H
