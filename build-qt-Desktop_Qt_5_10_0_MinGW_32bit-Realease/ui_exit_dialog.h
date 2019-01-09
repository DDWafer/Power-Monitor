/********************************************************************************
** Form generated from reading UI file 'exit_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXIT_DIALOG_H
#define UI_EXIT_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_Exit_Dialog
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;

    void setupUi(QDialog *Exit_Dialog)
    {
        if (Exit_Dialog->objectName().isEmpty())
            Exit_Dialog->setObjectName(QStringLiteral("Exit_Dialog"));
        Exit_Dialog->resize(268, 143);
        buttonBox = new QDialogButtonBox(Exit_Dialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(50, 80, 161, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label = new QLabel(Exit_Dialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(60, 30, 151, 41));
        QFont font;
        font.setFamily(QStringLiteral("Arial Unicode MS"));
        font.setPointSize(11);
        label->setFont(font);
        label->setLayoutDirection(Qt::LeftToRight);

        retranslateUi(Exit_Dialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), Exit_Dialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Exit_Dialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(Exit_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Exit_Dialog)
    {
        Exit_Dialog->setWindowTitle(QApplication::translate("Exit_Dialog", "Dialog", nullptr));
        label->setText(QApplication::translate("Exit_Dialog", "   \346\202\250\346\203\263\350\246\201\344\277\235\345\255\230\345\220\227\357\274\237", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Exit_Dialog: public Ui_Exit_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXIT_DIALOG_H
