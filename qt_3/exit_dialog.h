#ifndef EXIT_DIALOG_H
#define EXIT_DIALOG_H

#include <QDialog>

namespace Ui {
class Exit_Dialog;
}

class Exit_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Exit_Dialog(QWidget *parent = 0);
    ~Exit_Dialog();
signals:
    void save();
private slots:

    void on_buttonBox_accepted();

private:
    Ui::Exit_Dialog *ui;
};

#endif // EXIT_DIALOG_H
