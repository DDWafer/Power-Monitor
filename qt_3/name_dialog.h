#ifndef NAME_DIALOG_H
#define NAME_DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent ,QString *tab_name);
    ~Dialog();
    QString *m_tab_name;
signals:
    void load();//=========================================
private slots:

    //void on_my_goded();//=-====================================================


    void on_buttonBox_accepted();

private:
    Ui::Dialog *ui;
};

#endif // NAME_DIALOG_H
