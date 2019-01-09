#include "name_dialog.h"
#include "ui_name_dialog.h"

Dialog::Dialog(QWidget *parent,QString *tab_name) :
    QDialog(parent),
    ui(new Ui::Dialog)//====================================================
{
    ui->setupUi(this);
    m_tab_name=tab_name;
}

Dialog::~Dialog()
{
    delete ui;
}





void Dialog::on_buttonBox_accepted()
{

    QString tap_name;
   tap_name= ui->lineEdit->text();
   *m_tab_name=tap_name;
   emit load();
}
